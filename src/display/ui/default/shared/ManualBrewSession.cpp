#include "ManualBrewSession.h"

#include <algorithm>
#include <cmath>
#include <display/core/Controller.h>
#include <display/core/ProfileManager.h>
#include <display/ui/default/shared/ProcessControl.h>

namespace ManualBrewSession {
namespace {

Profile s_profileBackup{};
bool s_profileBackupValid = false;

} // namespace

float pressureScaling(Controller &controller) {
    return std::min(controller.getSettings().getPressureScaling(), kMaxPressureBar);
}

float pressureFromZoneY(float localY, float zoneHeight, float scaling) {
    if (localY <= kEdgeSnapPixels) {
        return scaling;
    }
    if (localY >= zoneHeight - kEdgeSnapPixels) {
        return 0.0f;
    }
    const float t = zoneHeight > 0.0f ? constrain(localY / zoneHeight, 0.0f, 1.0f) : 0.0f;
    float pressure = constrain(scaling * (1.0f - t), 0.0f, scaling);
    if (pressure <= kEdgeSnapBar) {
        return 0.0f;
    }
    if (pressure >= scaling - kEdgeSnapBar) {
        return scaling;
    }
    return pressure;
}

int parseManualProfileNumber(const String &label) {
    if (label == "Manual") {
        return 1;
    }
    if (!label.startsWith("Manual ")) {
        return 0;
    }
    return label.substring(7).toInt();
}

int nextManualProfileNumber(Controller *controller) {
    if (!controller) {
        return 1;
    }
    ProfileManager *profiles = controller->getProfileManager();
    if (!profiles) {
        return 1;
    }
    int maxNumber = 0;
    for (const String &id : controller->getSettings().getProfileOrder()) {
        Profile profile{};
        if (!profiles->loadProfile(id, profile)) {
            continue;
        }
        maxNumber = std::max(maxNumber, parseManualProfileNumber(profile.label));
    }
    return maxNumber + 1;
}

void backupSelectedProfile(Controller *controller) {
    if (!controller || !controller->getProfileManager()) {
        return;
    }
    s_profileBackup = controller->getProfileManager()->getSelectedProfile();
    s_profileBackupValid = true;
}

void restoreSelectedProfile(Controller *controller) {
    if (!controller || !s_profileBackupValid || !controller->getProfileManager()) {
        return;
    }
    controller->getProfileManager()->getSelectedProfile() = s_profileBackup;
    s_profileBackupValid = false;
    if (controller->getUI()) {
        controller->getUI()->markDirty();
    }
}

bool hasProfileBackup() { return s_profileBackupValid; }

void invalidateProfileBackup() { s_profileBackupValid = false; }

void ShotState::reset() {
    startMs = 0;
    durationMs = 0;
    inProgress = false;
    lastActive = false;
    timeline.clear();
}

void ShotState::begin(uint32_t nowMs, float initialPressure) {
    startMs = nowMs;
    durationMs = 0;
    inProgress = true;
    timeline.clear();
    recordPressure(nowMs, initialPressure);
}

void ShotState::finish(uint32_t nowMs) {
    if (!inProgress) {
        return;
    }
    durationMs = nowMs >= startMs ? nowMs - startMs : 0;
    inProgress = false;
}

void ShotState::recordPressure(uint32_t nowMs, float pressure) {
    if (!inProgress) {
        return;
    }
    const uint32_t elapsed = nowMs >= startMs ? nowMs - startMs : 0;
    if (!timeline.empty() && fabsf(timeline.back().second - pressure) < 0.01f) {
        return;
    }
    timeline.emplace_back(elapsed, pressure);
}

bool ShotState::hasShotData() const { return durationMs > 0 || !timeline.empty(); }

void exitCleanup(Controller *controller) {
    if (!controller) {
        return;
    }
    if (controller->isActive()) {
        ProcessControl::deactivateAndClear(*controller);
    }
    restoreSelectedProfile(controller);
    ProcessControl::deactivate(*controller);
}

} // namespace ManualBrewSession
