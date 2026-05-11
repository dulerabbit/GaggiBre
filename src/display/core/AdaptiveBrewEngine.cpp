#include "AdaptiveBrewEngine.h"

#ifndef GAGGIBRE_ADAPTIVE_V1
#define GAGGIBRE_ADAPTIVE_V1 0
#endif

namespace adaptive {

bool AdaptiveBrewEngine::isFeatureEnabled() {
    return GAGGIBRE_ADAPTIVE_V1 == 1;
}

// ---------------------------------------------------------------------------
// decideNext – PI pressure controller + flow-based channeling detection.
//
// PI layer
// --------
// Operates above the machine's own pump PID.  Each control tick (~100 ms) it
// adjusts the commanded target pressure by a PI term computed from the error
// between the desired puck pressure and the measured value.
//
// Ramp suppression:
//   Authority scales 0→1 over the first RAMP_UP_SECONDS so the integrator
//   cannot wind up during pre-infusion before measurable flow has started.
//
// Anti-windup:
//   Integral is clamped to ±INTEGRAL_CAP (bar·s).
//
// Channeling detection (NORMAL → SUSPECT → CHANNELING FSM)
// ---------------------------------------------------------
// A channel is characterised by a sudden FLOW SPIKE (water found a path of
// least resistance) that coincidentally causes a pressure DROP.  The engine
// tracks a short rolling average of recent flow and declares CHANNELING when:
//   • actual flow exceeds the rolling average by CHANNEL_FLOW_EXCESS_MLPS, AND
//   • actual pressure is below the target by CHANNEL_PRESSURE_DROP_BAR.
// Both conditions must persist for CHANNEL_CONFIRM_SAMPLES consecutive ticks.
// On detection an additional CHANNEL_CORRECTION_BAR negative offset is added
// (clamped by MAX_CORRECTION) to reduce pump pressure and encourage the puck
// to re-seal.  Recovery requires CHANNEL_RECOVER_SAMPLES consecutive ticks
// without the signal.
// ---------------------------------------------------------------------------
AdaptiveDecision AdaptiveBrewEngine::decideNext(const AdaptiveInput &input) {
    // Compute dt before pushing so history_.back() is the previous sample.
    float dt = DEFAULT_DT;
    if (!history_.empty()) {
        const float rawDt = input.elapsedSeconds - history_.back().elapsedSeconds;
        if (rawDt > 0.01F && rawDt < 5.0F) {
            dt = rawDt;
        }
    }

    history_.push_back(input);

    const float elapsedFromStart =
        history_.size() > 1U ? input.elapsedSeconds - history_.front().elapsedSeconds : 0.0F;

    // --- PI controller -------------------------------------------------------
    const float error    = input.targetPressureBar - input.actualPressureBar;
    integral_ = std::max(-INTEGRAL_CAP, std::min(INTEGRAL_CAP, integral_ + error * dt));

    const float rampFactor  = elapsedFromStart < RAMP_UP_SECONDS
                              ? (elapsedFromStart / RAMP_UP_SECONDS)
                              : 1.0F;
    const float rawPI = rampFactor * (KP * error + KI * integral_);

    // --- Channeling detection FSM -------------------------------------------
    float channelExtraCorrection = 0.0F;

    if (history_.size() >= CHANNEL_MIN_HISTORY) {
        // Rolling average of actualFlow over the last CHANNEL_MIN_HISTORY-1 samples
        // (everything *before* the sample we just pushed).
        float avgFlow = 0.0F;
        const std::size_t sz = history_.size();
        const std::size_t window = CHANNEL_MIN_HISTORY - 1U;
        for (std::size_t i = sz - 1U - window; i < sz - 1U; ++i) {
            avgFlow += history_[i].actualFlowMLps;
        }
        avgFlow /= static_cast<float>(window);

        const bool flowSpike     = (input.actualFlowMLps - avgFlow) > CHANNEL_FLOW_EXCESS_MLPS;
        const bool pressureDrop  = (input.targetPressureBar - input.actualPressureBar) > CHANNEL_PRESSURE_DROP_BAR;
        const bool channelSignal = flowSpike && pressureDrop;

        switch (channelState_) {
        case ChannelState::NORMAL:
            if (channelSignal) {
                channelSuspectCount_++;
                channelState_ = (channelSuspectCount_ >= CHANNEL_CONFIRM_SAMPLES)
                                ? ChannelState::CHANNELING
                                : ChannelState::SUSPECT;
                if (channelState_ == ChannelState::CHANNELING) channelRecoverCount_ = 0;
            } else {
                channelSuspectCount_ = 0;
            }
            break;

        case ChannelState::SUSPECT:
            if (channelSignal) {
                channelSuspectCount_++;
                if (channelSuspectCount_ >= CHANNEL_CONFIRM_SAMPLES) {
                    channelState_        = ChannelState::CHANNELING;
                    channelRecoverCount_ = 0;
                }
            } else {
                channelState_        = ChannelState::NORMAL;
                channelSuspectCount_ = 0;
            }
            break;

        case ChannelState::CHANNELING:
            if (!channelSignal) {
                if (++channelRecoverCount_ >= CHANNEL_RECOVER_SAMPLES) {
                    channelState_        = ChannelState::NORMAL;
                    channelSuspectCount_ = 0;
                }
            } else {
                channelRecoverCount_ = 0;
            }
            channelExtraCorrection = CHANNEL_CORRECTION_BAR;
            break;
        }
    }

    // --- Final correction & decision ----------------------------------------
    const float totalRaw    = rawPI + channelExtraCorrection;
    const float correction  = std::max(-MAX_CORRECTION, std::min(MAX_CORRECTION, totalRaw));

    AdaptiveDecision decision;
    decision.nextTargetPressureBar = input.targetPressureBar + correction;
    decision.confidence            = std::min(elapsedFromStart / CONFIDENCE_WINDOW_SECONDS, 1.0F);
    decision.correctionApplied     = correction;
    decision.channelingDetected    = (channelState_ == ChannelState::CHANNELING);

    lastDecision_ = decision;
    return decision;
}

const std::vector<AdaptiveInput> &AdaptiveBrewEngine::history() const {
    return history_;
}

void AdaptiveBrewEngine::reset() {
    history_.clear();
    integral_           = 0.0F;
    lastDecision_       = {};
    channelState_       = ChannelState::NORMAL;
    channelSuspectCount_ = 0;
    channelRecoverCount_ = 0;
}

} // namespace adaptive
