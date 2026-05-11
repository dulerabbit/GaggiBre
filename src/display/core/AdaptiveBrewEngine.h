#pragma once

#include <vector>

namespace adaptive {

struct AdaptiveInput {
    float targetPressureBar = 0.0F;
    float actualPressureBar = 0.0F;
    float elapsedSeconds = 0.0F;
};

struct AdaptiveDecision {
    float nextTargetPressureBar = 0.0F;
    float confidence = 0.0F;
};

class AdaptiveBrewEngine {
public:
    AdaptiveBrewEngine() = default;

    static bool isFeatureEnabled();

    AdaptiveDecision decideNext(const AdaptiveInput &input);

    const std::vector<AdaptiveInput> &history() const;
    void reset();

private:
    std::vector<AdaptiveInput> history_;
};

} // namespace adaptive
