"""
simulate_adaptive.py — GaggiMate Adaptive Brew Engine Simulation
================================================================
Runs the PI pressure controller + channeling detection FSM against a
synthetic 30-second espresso shot and prints a per-tick table, so you can
verify the engine behaviour without hardware.

Usage:
    python scripts/simulate_adaptive.py [--channel-at <second>]

Arguments:
    --channel-at  Second at which to inject a channeling event (default: 18).
                  Set to -1 to suppress the channel injection.

Output columns:
    t     elapsed seconds
    tgt   profile target pressure (bar) — what the profile asks for
    act   actual puck pressure (bar)    — simulated sensor reading
    flow  actual flow (mL/s)            — simulated sensor reading
    corr  engine correction (bar)
    cmd   commanded pressure sent to pump (= tgt + corr, clamped)
    conf  confidence 0-100%
    chan  channeling state (., SUSPECT, CHANNELING)
"""

import argparse
import sys

# ------- Engine constants (must match AdaptiveBrewEngine.h) -----------------
KP                     = 0.30
KI                     = 0.05
MAX_CORRECTION         = 1.2
INTEGRAL_CAP           = 2.5
RAMP_UP_SECONDS        = 6.0
CONFIDENCE_WINDOW      = 10.0
DEFAULT_DT             = 0.1

CHANNEL_MIN_HISTORY    = 4
CHANNEL_FLOW_EXCESS    = 1.2   # mL/s above rolling avg
CHANNEL_PRESSURE_DROP  = 0.7   # bar below target
CHANNEL_CONFIRM        = 2     # consecutive suspect ticks
CHANNEL_RECOVER        = 5     # consecutive normal ticks
CHANNEL_EXTRA_BAR      = -1.0  # additional negative correction when channeling
MAX_PRESSURE_SCALING   = 12.0  # equivalent to settings.getPressureScaling()

# ------- Shot profile definition ---------------------------------------------
# Mimics a simple 9-bar profile: 6 s pre-infusion ramp then hold.
def profile_target(t: float) -> float:
    if t < 6.0:
        return 4.0 + (9.0 - 4.0) * (t / 6.0)   # ramp 4→9 bar
    return 9.0

# ------- Simulated sensor (plant model) -------------------------------------
# Very simple first-order system: actual pressure lags target with time
# constant TAU, plus a channeling spike when injected.
TAU = 0.8   # seconds

class PlantModel:
    def __init__(self, channel_at: float):
        self.actual_pressure = 0.0
        self.flow = 0.8          # baseline flow mL/s
        self.channel_at = channel_at
        self.channeling = False

    def step(self, commanded: float, t: float, dt: float) -> tuple:
        """Returns (actual_pressure, actual_flow)."""
        alpha = dt / (TAU + dt)
        self.actual_pressure += alpha * (commanded - self.actual_pressure)

        # Inject channeling at the specified second
        if self.channel_at > 0 and t >= self.channel_at and t < self.channel_at + 4.0:
            self.channeling = True
            # Channel: flow spikes, pressure drops
            self.flow = 0.8 + 2.0 * min(1.0, (t - self.channel_at) / 0.5)
            self.actual_pressure *= 0.75  # pressure collapses through channel
        else:
            self.channeling = False
            target_flow = 1.0 + (commanded / 9.0) * 0.5
            self.flow = self.flow * 0.8 + target_flow * 0.2

        return self.actual_pressure, self.flow

# ------- FSM helper ----------------------------------------------------------
NORMAL    = "NORMAL"
SUSPECT   = "SUSPECT"
CHANNELING = "CHANNELING"

class ChannelFSM:
    def __init__(self):
        self.state = NORMAL
        self.suspect_count = 0
        self.recover_count = 0

    def update(self, signal: bool) -> str:
        if self.state == NORMAL:
            if signal:
                self.suspect_count += 1
                if self.suspect_count >= CHANNEL_CONFIRM:
                    self.state = CHANNELING
                    self.recover_count = 0
                else:
                    self.state = SUSPECT
            else:
                self.suspect_count = 0
        elif self.state == SUSPECT:
            if signal:
                self.suspect_count += 1
                if self.suspect_count >= CHANNEL_CONFIRM:
                    self.state = CHANNELING
                    self.recover_count = 0
            else:
                self.state = NORMAL
                self.suspect_count = 0
        elif self.state == CHANNELING:
            if not signal:
                self.recover_count += 1
                if self.recover_count >= CHANNEL_RECOVER:
                    self.state = NORMAL
                    self.suspect_count = 0
            else:
                self.recover_count = 0
        return self.state

# ------- Engine simulation ---------------------------------------------------
def simulate(channel_at: float):
    plant = PlantModel(channel_at)
    fsm   = ChannelFSM()

    integral = 0.0
    history_flow = []     # rolling window of actual flow
    start_t = 0.0
    elapsed_from_start = 0.0
    first_t = None

    dt = DEFAULT_DT
    t  = 0.0
    total = 30.0

    print(f"{'t':>5} {'tgt':>6} {'act':>6} {'flow':>6} {'corr':>7} {'cmd':>6} "
          f"{'conf':>6} {'state':<10}  note")
    print("-" * 75)

    channeling_events = 0
    last_channeling = False
    correction_sum  = 0.0
    correction_max  = 0.0
    samples         = 0

    while t <= total:
        tgt = profile_target(t)
        act, flow = plant.step(tgt, t, dt)  # plant uses raw target for simplicity

        # ---- PI ----
        if first_t is None:
            first_t = t
        elapsed_from_start = t - first_t if first_t is not None else 0.0

        error     = tgt - act
        integral  = max(-INTEGRAL_CAP, min(INTEGRAL_CAP, integral + error * dt))
        ramp      = min(1.0, elapsed_from_start / RAMP_UP_SECONDS)
        raw_pi    = ramp * (KP * error + KI * integral)

        # ---- Channeling detection ----
        history_flow.append(flow)
        channel_extra = 0.0
        state_disp    = "."
        if len(history_flow) >= CHANNEL_MIN_HISTORY:
            window  = list(history_flow[-(CHANNEL_MIN_HISTORY):])
            avg_flow = sum(window[:-1]) / (len(window) - 1)
            spike   = (flow - avg_flow) > CHANNEL_FLOW_EXCESS
            drop    = (tgt - act) > CHANNEL_PRESSURE_DROP
            signal  = spike and drop
            fsm_state = fsm.update(signal)
            if fsm_state == CHANNELING:
                channel_extra = CHANNEL_EXTRA_BAR
                state_disp    = "CHANNELING"
            elif fsm_state == SUSPECT:
                state_disp = "SUSPECT"

        total_raw  = raw_pi + channel_extra
        correction = max(-MAX_CORRECTION, min(MAX_CORRECTION, total_raw))
        commanded  = max(0.0, min(MAX_PRESSURE_SCALING, tgt + correction))
        confidence = min(1.0, elapsed_from_start / CONFIDENCE_WINDOW)

        channel_now = (state_disp == "CHANNELING")
        if channel_now and not last_channeling:
            channeling_events += 1
        last_channeling = channel_now

        correction_sum += correction
        correction_max  = max(correction_max, abs(correction))
        samples += 1

        note = ""
        if channel_now:
            note = "⚠ CH!"
        elif abs(correction) >= 0.2:
            note = f"PI {'+' if correction >= 0 else ''}{correction:.2f}"

        print(f"{t:5.1f} {tgt:6.2f} {act:6.2f} {flow:6.2f} "
              f"{correction:+7.2f} {commanded:6.2f} "
              f"{confidence*100:5.0f}%  {state_disp:<10}  {note}")

        t     = round(t + dt, 6)
        t     = round(t, 1)   # keep clean decimal steps

    avg_corr = correction_sum / samples if samples else 0.0
    print()
    print(f"Summary:")
    print(f"  Channeling events detected : {channeling_events}")
    print(f"  Average correction          : {avg_corr:+.3f} bar")
    print(f"  Max |correction|            : {correction_max:.3f} bar")
    print(f"  Total samples               : {samples}")
    if channeling_events == 0 and channel_at > 0:
        print("  NOTE: No channeling detected — try reducing --channel-at or "
              "adjusting CHANNEL_FLOW_EXCESS / CHANNEL_PRESSURE_DROP thresholds.")
    return channeling_events

# ------- Entry point ---------------------------------------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--channel-at", type=float, default=18.0,
                        help="Inject channeling event at this second (default: 18, -1 = none)")
    args = parser.parse_args()

    events = simulate(args.channel_at)
    sys.exit(0 if events >= 0 else 1)
