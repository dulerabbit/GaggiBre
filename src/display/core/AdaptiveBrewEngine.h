#pragma once

#include <algorithm>
#include <vector>

namespace adaptive {

struct AdaptiveInput {
    float targetPressureBar = 0.0F;
    float actualPressureBar = 0.0F;
    float elapsedSeconds = 0.0F; // absolute seconds (millis/1000)
};

struct AdaptiveDecision {
    float nextTargetPressureBar = 0.0F;
    float confidence = 0.0F; // 0.0 – 1.0
};

class AdaptiveBrewEngine {
public:
    AdaptiveBrewEngine() = default;

    static bool isFeatureEnabled();

    AdaptiveDecision decideNext(const AdaptiveInput &input);

    const std::vector<AdaptiveInput> &history() const;
    void reset();

    // -- Tuning constants (public to allow unit test overrides) ----------------
    // Proportional gain: bar of correction per bar of error.
    static constexpr float KP = 0.30F;
    // Integral gain: bar of correction per bar·second of accumulated error.
    static constexpr float KI = 0.05F;
    // Maximum correction applied to the target in either direction (bar).
    static constexpr float MAX_CORRECTION = 1.2F;
    // Anti-windup cap on the integral accumulator (bar·seconds).
    static constexpr float INTEGRAL_CAP = 2.5F;
    // Ramp-up window: suppress PI authority linearly during pre-infusion (s).
    static constexpr float RAMP_UP_SECONDS = 6.0F;
    // Reference window for 0→1 confidence ramp (s).
    static constexpr float CONFIDENCE_WINDOW_SECONDS = 10.0F;
    // Fallback dt used when the first sample arrives or dt looks implausible.
    static constexpr float DEFAULT_DT = 0.1F;

private:
    std::vector<AdaptiveInput> history_;
    float integral_ = 0.0F;
};

} // namespace adaptive
