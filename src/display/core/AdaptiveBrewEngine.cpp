#include "AdaptiveBrewEngine.h"

#ifndef GAGGIBRE_ADAPTIVE_V1
#define GAGGIBRE_ADAPTIVE_V1 0
#endif

namespace adaptive {

bool AdaptiveBrewEngine::isFeatureEnabled() {
    return GAGGIBRE_ADAPTIVE_V1 == 1;
}

AdaptiveDecision AdaptiveBrewEngine::decideNext(const AdaptiveInput &input) {
    history_.push_back(input);

    AdaptiveDecision decision;

    // Keep MVP behavior conservative: mirror current target while we wire full strategy.
    decision.nextTargetPressureBar = input.targetPressureBar;

    // Confidence grows with sample count but remains bounded.
    const float samples = static_cast<float>(history_.size());
    decision.confidence = samples < 10.0F ? (samples / 10.0F) : 1.0F;

    return decision;
}

const std::vector<AdaptiveInput> &AdaptiveBrewEngine::history() const {
    return history_;
}

void AdaptiveBrewEngine::reset() {
    history_.clear();
}

} // namespace adaptive
