#pragma once

#include <Arduino.h>
#include <display/models/profile.h>
#include <utility>
#include <vector>

// Compress a live pressure timeline into a short phase list for "save as profile".
std::vector<std::pair<uint32_t, float>> compressManualPressureTimeline(const std::vector<std::pair<uint32_t, float>> &timeline);

// Apply compressed timeline points onto profile.phases (pressure-target brew phases).
void applyManualTimelineToProfile(Profile &profile, const std::vector<std::pair<uint32_t, float>> &timeline,
                                  uint32_t shotDurationMs, float temperature, float pressureScaling);
