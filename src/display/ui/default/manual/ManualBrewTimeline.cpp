#include "ManualBrewTimeline.h"

#include <algorithm>
#include <cmath>

static constexpr size_t MANUAL_PROFILE_MAX_STEPS = 12;

std::vector<std::pair<uint32_t, float>> compressManualPressureTimeline(const std::vector<std::pair<uint32_t, float>> &timeline) {
    if (timeline.size() <= 2) {
        return timeline;
    }

    std::vector<std::pair<uint32_t, float>> reduced;
    reduced.reserve(timeline.size());
    reduced.push_back(timeline.front());

    for (size_t i = 1; i + 1 < timeline.size(); ++i) {
        const auto &prev = reduced.back();
        const auto &curr = timeline[i];
        const auto &next = timeline[i + 1];

        const float d1 = curr.second - prev.second;
        const float d2 = next.second - curr.second;
        const bool sameDirection = (d1 == 0.0f || d2 == 0.0f || (d1 > 0.0f && d2 > 0.0f) || (d1 < 0.0f && d2 < 0.0f));
        const bool shortTransient = (curr.first - prev.first) < 1200 && fabsf(curr.second - prev.second) < 0.8f;

        bool nearLinear = false;
        if (next.first > prev.first) {
            const float t = static_cast<float>(curr.first - prev.first) / static_cast<float>(next.first - prev.first);
            const float interpolated = prev.second + (next.second - prev.second) * t;
            nearLinear = fabsf(curr.second - interpolated) < 0.25f;
        }

        if (sameDirection && (shortTransient || nearLinear)) {
            continue;
        }
        reduced.push_back(curr);
    }

    reduced.push_back(timeline.back());

    if (reduced.size() <= MANUAL_PROFILE_MAX_STEPS) {
        return reduced;
    }

    std::vector<std::pair<uint32_t, float>> capped;
    capped.reserve(MANUAL_PROFILE_MAX_STEPS);
    capped.push_back(reduced.front());

    const size_t lastIndex = reduced.size() - 1;
    size_t lastAddedIndex = 0;
    for (size_t k = 1; k + 1 < MANUAL_PROFILE_MAX_STEPS; ++k) {
        size_t idx = (k * lastIndex) / (MANUAL_PROFILE_MAX_STEPS - 1);
        if (idx <= lastAddedIndex) {
            idx = std::min(lastIndex - 1, idx + 1);
        }
        capped.push_back(reduced[idx]);
        lastAddedIndex = idx;
    }

    if (capped.back() != reduced.back()) {
        capped.push_back(reduced.back());
    }
    return capped;
}

void applyManualTimelineToProfile(Profile &profile, const std::vector<std::pair<uint32_t, float>> &timeline,
                                  uint32_t shotDurationMs, float temperature, float pressureScaling) {
    if (timeline.empty()) {
        return;
    }

    const auto compressed = compressManualPressureTimeline(timeline);
    if (compressed.empty()) {
        return;
    }

    float shotDurationSec = static_cast<float>(shotDurationMs) / 1000.0f;
    shotDurationSec = std::max(0.5f, shotDurationSec);

    std::vector<Phase> phases;
    phases.reserve(compressed.size());

    for (size_t i = 0; i < compressed.size(); ++i) {
        const float startSec = static_cast<float>(compressed[i].first) / 1000.0f;
        const float endSec =
            (i + 1 < compressed.size()) ? static_cast<float>(compressed[i + 1].first) / 1000.0f : shotDurationSec;
        const float duration = std::max(0.5f, endSec - startSec);

        Phase phase{};
        phase.name = (i == 0) ? "Manual Brew" : ("Manual Brew " + String(static_cast<int>(i + 1)));
        phase.phase = PhaseType::PHASE_TYPE_BREW;
        phase.valve = 1;
        phase.duration = duration;
        phase.pumpIsSimple = false;
        phase.pumpAdvanced.target = PumpTarget::PUMP_TARGET_PRESSURE;
        phase.pumpAdvanced.pressure = constrain(compressed[i].second, 0.0f, pressureScaling);
        phase.pumpAdvanced.flow = 0.0f;
        phase.temperature = temperature;
        phase.transition = Transition{.type = TransitionType::INSTANT, .duration = 0.0f, .adaptive = false};
        phases.push_back(phase);
    }

    if (!phases.empty()) {
        profile.phases = std::move(phases);
    }
}
