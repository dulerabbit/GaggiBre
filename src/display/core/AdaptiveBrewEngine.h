#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

namespace adaptive {

struct AdaptiveInput {
    float targetPressureBar = 0.0F;
    float actualPressureBar = 0.0F;
    float elapsedSeconds = 0.0F;   // absolute seconds (millis/1000)
    float targetFlowMLps = 0.0F;   // profile target flow (mL/s), 0 when not flow-target
    float actualFlowMLps = 0.0F;   // measured puck/pump flow (mL/s)
};

struct AdaptiveDecision {
    float nextTargetPressureBar = 0.0F;
    float confidence = 0.0F;          // 0.0 – 1.0
    float correctionApplied = 0.0F;   // signed bar delta from original target
    bool  channelingDetected = false;  // engine believes channeling is occurring
};

/// Channeling-detection FSM state.
enum class ChannelState { NORMAL, SUSPECT, CHANNELING };

class AdaptiveBrewEngine {
public:
    AdaptiveBrewEngine() = default;

    static bool isFeatureEnabled();

    AdaptiveDecision decideNext(const AdaptiveInput &input);

    const std::vector<AdaptiveInput> &history()   const;
    AdaptiveDecision                  lastDecision() const { return lastDecision_; }
    ChannelState                      getChannelState() const { return channelState_; }
    void reset();

    // -- PI tuning constants ---------------------------------------------------
    static constexpr float KP = 0.30F;
    static constexpr float KI = 0.05F;
    static constexpr float MAX_CORRECTION          = 1.2F;
    static constexpr float INTEGRAL_CAP            = 2.5F;
    static constexpr float RAMP_UP_SECONDS         = 6.0F;
    static constexpr float CONFIDENCE_WINDOW_SECONDS = 10.0F;
    static constexpr float DEFAULT_DT              = 0.1F;

    // -- Channeling detection constants ----------------------------------------
    // Minimum flow history window (samples) before channeling can be declared.
    static constexpr std::size_t CHANNEL_MIN_HISTORY      = 4U;
    // Flow spike threshold: actual mL/s above rolling average to suspect channel.
    static constexpr float       CHANNEL_FLOW_EXCESS_MLPS = 1.2F;
    // Pressure deficit (bar below target) required to confirm the spike is a channel.
    static constexpr float       CHANNEL_PRESSURE_DROP_BAR = 0.7F;
    // Consecutive SUSPECT samples needed to enter CHANNELING state.
    static constexpr int         CHANNEL_CONFIRM_SAMPLES  = 2;
    // Consecutive non-suspect samples needed to recover from CHANNELING.
    static constexpr int         CHANNEL_RECOVER_SAMPLES  = 5;
    // Additional negative correction (bar) applied while in CHANNELING state.
    static constexpr float       CHANNEL_CORRECTION_BAR   = -1.0F;

private:
    std::vector<AdaptiveInput> history_;
    float          integral_          = 0.0F;
    AdaptiveDecision lastDecision_    = {};
    ChannelState   channelState_      = ChannelState::NORMAL;
    int            channelSuspectCount_ = 0;
    int            channelRecoverCount_ = 0;
};

} // namespace adaptive
