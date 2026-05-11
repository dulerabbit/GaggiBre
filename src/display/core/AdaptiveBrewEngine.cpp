#include "AdaptiveBrewEngine.h"

#ifndef GAGGIBRE_ADAPTIVE_V1
#define GAGGIBRE_ADAPTIVE_V1 0
#endif

namespace adaptive {

bool AdaptiveBrewEngine::isFeatureEnabled() {
    return GAGGIBRE_ADAPTIVE_V1 == 1;
}

// ---------------------------------------------------------------------------
// decideNext – PI pressure controller for adaptive espresso extraction.
//
// Approach
// --------
// The machine's downstream PID already stabilises boiler/pump dynamics;  this
// layer operates *above* it by adjusting the commanded target pressure on each
// control tick (~100 ms) based on the measured error between the desired and
// the actual puck pressure.
//
// Ramp suppression:
//   During the first RAMP_UP_SECONDS of a shot (pre-infusion / puck wetting)
//   the correction authority is scaled from 0 → 1 so that the PI integrator
//   cannot wind up before the puck is properly saturated and measurable flow
//   has begun.  This prevents an overshoot spike that would disturb the
//   channelling-free onset of extraction.
//
// Anti-windup:
//   The integral is clamped to ±INTEGRAL_CAP (bar·s) so sustained large
//   errors (e.g. empty portafilter) cannot drive an unbounded correction.
// ---------------------------------------------------------------------------
AdaptiveDecision AdaptiveBrewEngine::decideNext(const AdaptiveInput &input) {
    // Compute dt *before* pushing so we can reference history_.back().
    float dt = DEFAULT_DT;
    if (!history_.empty()) {
        const float rawDt = input.elapsedSeconds - history_.back().elapsedSeconds;
        if (rawDt > 0.01F && rawDt < 5.0F) {
            dt = rawDt;
        }
    }

    history_.push_back(input);

    // Elapsed time since the first sample (per-shot clock).
    const float elapsedFromStart =
        history_.size() > 1U ? input.elapsedSeconds - history_.front().elapsedSeconds : 0.0F;

    // Proportional error: positive when actual < target (under-pressure).
    const float error = input.targetPressureBar - input.actualPressureBar;

    // Integrate error with anti-windup clamp.
    integral_ = std::max(-INTEGRAL_CAP, std::min(INTEGRAL_CAP, integral_ + error * dt));

    // Linearly ramp up PI authority over the pre-infusion window.
    const float rampFactor =
        elapsedFromStart < RAMP_UP_SECONDS ? (elapsedFromStart / RAMP_UP_SECONDS) : 1.0F;

    const float rawCorrection = rampFactor * (KP * error + KI * integral_);
    const float correction = std::max(-MAX_CORRECTION, std::min(MAX_CORRECTION, rawCorrection));

    AdaptiveDecision decision;
    decision.nextTargetPressureBar = input.targetPressureBar + correction;
    decision.confidence = std::min(elapsedFromStart / CONFIDENCE_WINDOW_SECONDS, 1.0F);

    return decision;
}

const std::vector<AdaptiveInput> &AdaptiveBrewEngine::history() const {
    return history_;
}

void AdaptiveBrewEngine::reset() {
    history_.clear();
    integral_ = 0.0F;
}

} // namespace adaptive
