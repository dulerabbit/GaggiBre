#pragma once

#include <Arduino.h>
#include <display/models/profile.h>
#include <utility>
#include <vector>

class Controller;

// Shared Manual Brew session services for procedural ManualBrewScreen and
// experimental NativeManualBrew. Owns profile backup/restore, pressure scaling,
// timeline shot bookkeeping, and screen-exit cleanup — not LVGL widgets.
namespace ManualBrewSession {

constexpr float kMaxPressureBar = 12.0f;
constexpr float kEdgeSnapPixels = 10.0f;
constexpr float kEdgeSnapBar = 0.25f;
constexpr float kMaximumManualFlow = 12.0f;

float pressureScaling(Controller &controller);

// Map a vertical press inside a zone to a pressure target (top = max, bottom = 0)
// with the same edge-snap rules as production ManualBrewScreen.
float pressureFromZoneY(float localY, float zoneHeight, float scaling);

int parseManualProfileNumber(const String &label);
int nextManualProfileNumber(Controller *controller);

void backupSelectedProfile(Controller *controller);
void restoreSelectedProfile(Controller *controller);
bool hasProfileBackup();
void invalidateProfileBackup();

struct ShotState {
    uint32_t startMs = 0;
    uint32_t durationMs = 0;
    bool inProgress = false;
    bool lastActive = false;
    std::vector<std::pair<uint32_t, float>> timeline;

    void reset();
    void begin(uint32_t nowMs, float initialPressure);
    void finish(uint32_t nowMs);
    void recordPressure(uint32_t nowMs, float pressure);
    bool hasShotData() const;
};

// Stop an active Manual Brew process (deactivate + clear) then restore the
// profile backup. Callers still own screen navigation and local UI reset.
void exitCleanup(Controller *controller);

} // namespace ManualBrewSession
