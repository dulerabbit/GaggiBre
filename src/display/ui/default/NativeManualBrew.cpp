#include "NativeManualBrew.h"

#include <algorithm>
#include <cmath>
#include <display/core/Controller.h>
#include <display/core/ProfileManager.h>
#include <display/core/constants.h>
#include <display/core/process/BrewProcess.h>
#include <display/ui/default/GeneratedUI.h>
#include <display/ui/default/manual/ManualBrewTimeline.h>
#include <display/ui/default/shared/ManualBrewSession.h>
#include <display/ui/default/shared/ProcessControl.h>

namespace NativeManualBrew {
#if GAGGIMATE_HAS_NATIVE_MANUAL_BREW
namespace {

Controller *s_controller = nullptr;
lv_chart_series_t *s_pressureSeries = nullptr;
lv_chart_series_t *s_flowSeries = nullptr;
uint32_t s_lastChartUpdate = 0;
ManualBrewSession::ShotState s_shot;

float pointerFraction(lv_event_t *event, bool vertical) {
    lv_obj_t *zone = lv_event_get_target(event);
    lv_indev_t *indev = lv_indev_get_act();
    if (!zone || !indev) {
        return 0.0f;
    }
    lv_point_t point{};
    lv_area_t area{};
    lv_indev_get_point(indev, &point);
    lv_obj_get_coords(zone, &area);
    const float start = vertical ? static_cast<float>(area.y1) : static_cast<float>(area.x1);
    const float extent = vertical ? static_cast<float>(lv_area_get_height(&area)) : static_cast<float>(lv_area_get_width(&area));
    const float coordinate = vertical ? static_cast<float>(point.y) : static_cast<float>(point.x);
    return extent > 0.0f ? constrain((coordinate - start) / extent, 0.0f, 1.0f) : 0.0f;
}

bool savePromptVisible() {
    return objects.manual_brew_save_prompt && !lv_obj_has_flag(objects.manual_brew_save_prompt, LV_OBJ_FLAG_HIDDEN);
}

void hidePrompt() {
    if (objects.manual_brew_save_prompt) {
        lv_obj_add_flag(objects.manual_brew_save_prompt, LV_OBJ_FLAG_HIDDEN);
    }
}

void resetIdleLabels() {
    if (objects.manual_brew_elapsed) {
        lv_label_set_text(objects.manual_brew_elapsed, "0:00");
    }
    if (objects.manual_brew_flow) {
        lv_label_set_text(objects.manual_brew_flow, "-- ml/s");
    }
    if (objects.manual_brew_pressure) {
        lv_label_set_text(objects.manual_brew_pressure, "-- bar");
    }
    if (objects.manual_brew_weight) {
        lv_label_set_text(objects.manual_brew_weight, "-- g");
    }
}

} // namespace

void init(Controller *controller) { s_controller = controller; }

void onLoad() {
    if (!s_controller || !objects.manual_brew_chart) {
        return;
    }
    if (!s_pressureSeries) {
        lv_chart_set_type(objects.manual_brew_chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(objects.manual_brew_chart, 120);
        lv_chart_set_range(objects.manual_brew_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 120);
        s_pressureSeries =
            lv_chart_add_series(objects.manual_brew_chart, lv_color_hex(0x2CA4F6), LV_CHART_AXIS_PRIMARY_Y);
        s_flowSeries = lv_chart_add_series(objects.manual_brew_chart, lv_color_hex(0xF0A030), LV_CHART_AXIS_PRIMARY_Y);
    }
    hidePrompt();
    resetIdleLabels();
    update();
}

void toggle() {
    if (!s_controller) {
        return;
    }
    if (s_controller->getMode() != MODE_MANUAL) {
        s_controller->setMode(MODE_MANUAL);
    }
    if (s_controller->isActive()) {
        ProcessControl::deactivate(*s_controller);
        s_shot.finish(millis());
    } else {
        hidePrompt();
        ProcessControl::activate(*s_controller);
        s_shot.begin(millis(), s_controller->getManualPressureTarget());
        if (objects.manual_brew_chart && s_pressureSeries && s_flowSeries) {
            lv_chart_set_all_value(objects.manual_brew_chart, s_pressureSeries, LV_CHART_POINT_NONE);
            lv_chart_set_all_value(objects.manual_brew_chart, s_flowSeries, LV_CHART_POINT_NONE);
        }
    }
    s_controller->getUI()->markDirty();
}

void setTemperatureFromPointer(lv_event_t *event) {
    if (!s_controller) {
        return;
    }
    const float fraction = pointerFraction(event, true);
    const float temperature = MAX_TEMP - fraction * static_cast<float>(MAX_TEMP - MIN_TEMP);
    s_controller->setTargetTemp(roundf(temperature));
    s_controller->getUI()->markDirty();
}

void setPressureFromPointer(lv_event_t *event) {
    if (!s_controller) {
        return;
    }
    lv_obj_t *zone = lv_event_get_target(event);
    lv_indev_t *indev = lv_indev_get_act();
    if (!zone || !indev) {
        return;
    }
    lv_point_t point{};
    lv_area_t area{};
    lv_indev_get_point(indev, &point);
    lv_obj_get_coords(zone, &area);
    const float localY = static_cast<float>(point.y - area.y1);
    const float height = static_cast<float>(lv_area_get_height(&area));
    const float scaling = ManualBrewSession::pressureScaling(*s_controller);
    const float pressure = ManualBrewSession::pressureFromZoneY(localY, height, scaling);
    s_controller->setManualPressureTarget(pressure);
    s_shot.recordPressure(millis(), s_controller->getManualPressureTarget());
    s_controller->getUI()->markDirty();
}

void setFlowFromPointer(lv_event_t *event) {
    // Intentionally no-op: Controller::setManualFlowTarget rewrites brew phases to
    // PUMP_TARGET_FLOW, which conflicts with pressure-based Manual Brew and there is
    // no separate live flow-control path on the controller for this screen. Keep the
    // EEZ flow zone for future wiring; do not mutate the selected profile here.
    (void)event;
}

void save() {
    if (!s_controller) {
        return;
    }
    ProfileManager *profiles = s_controller->getProfileManager();
    if (!profiles) {
        return;
    }
    Profile profile = profiles->getSelectedProfile();
    profile.id = "";
    profile.label = "Manual " + String(ManualBrewSession::nextManualProfileNumber(s_controller));
    applyManualTimelineToProfile(profile, s_shot.timeline, s_shot.durationMs, profile.temperature,
                                 s_controller->getSettings().getPressureScaling());
    if (profiles->saveProfile(profile)) {
        auto order = s_controller->getSettings().getProfileOrder();
        if (std::find(order.begin(), order.end(), profile.id) == order.end()) {
            order.push_back(profile.id);
            s_controller->getSettings().setProfileOrder(std::move(order));
        }
        profiles->addFavoritedProfile(profile.id);
        profiles->selectProfile(profile.id);
        s_controller->getUI()->markProfileClean();
        hidePrompt();
        s_shot.reset();
        ManualBrewSession::invalidateProfileBackup();
    }
}

void discard() {
    hidePrompt();
    s_shot.reset();
}

void onExit() {
    ManualBrewSession::exitCleanup(s_controller);
    discard();
}

void update() {
    if (!s_controller || !objects.manual_brew_screen ||
        eez_flow_get_current_screen() != SCREEN_ID_MANUAL_BREW_SCREEN) {
        return;
    }

    const bool active = s_controller->isActive();
    if (objects.manual_brew_pressure_target) {
        lv_label_set_text_fmt(objects.manual_brew_pressure_target, "%.1f bar", s_controller->getManualPressureTarget());
    }
    if (objects.manual_brew_pressure) {
        lv_label_set_text_fmt(objects.manual_brew_pressure, "%.1f bar", s_controller->getCurrentPressure());
    }
    if (objects.manual_brew_play_pause_label) {
        lv_label_set_text(objects.manual_brew_play_pause_label, active ? "PAUSE" : "PLAY");
    }

    Process *process = s_controller->getProcess();
    if (active && process && process->getType() == MODE_BREW) {
        auto *brew = static_cast<BrewProcess *>(process);
        const unsigned long elapsed = millis() >= brew->processStarted ? millis() - brew->processStarted : 0;
        if (objects.manual_brew_elapsed) {
            lv_label_set_text_fmt(objects.manual_brew_elapsed, "%lu:%02lu", elapsed / 60000, (elapsed / 1000) % 60);
        }
        if (objects.manual_brew_flow) {
            lv_label_set_text_fmt(objects.manual_brew_flow, "%.1f ml/s", brew->currentFlow);
        }
        if (objects.manual_brew_weight) {
            lv_label_set_text_fmt(objects.manual_brew_weight, "%.1f g", brew->currentVolume);
        }
        const uint32_t now = millis();
        if (now - s_lastChartUpdate >= 250) {
            s_lastChartUpdate = now;
            if (objects.manual_brew_chart && s_pressureSeries && s_flowSeries) {
                lv_chart_set_next_value(objects.manual_brew_chart, s_pressureSeries,
                                        static_cast<lv_coord_t>(s_controller->getCurrentPressure() * 10.0f));
                lv_chart_set_next_value(objects.manual_brew_chart, s_flowSeries,
                                        static_cast<lv_coord_t>(brew->currentFlow * 10.0f));
            }
            s_shot.recordPressure(now, s_controller->getManualPressureTarget());
        }
    } else if (!active && !savePromptVisible()) {
        resetIdleLabels();
    }

    if (s_shot.lastActive && !active) {
        s_shot.finish(millis());
        if (objects.manual_brew_save_prompt && s_shot.hasShotData()) {
            lv_obj_clear_flag(objects.manual_brew_save_prompt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(objects.manual_brew_save_prompt);
        }
    }
    if (!s_shot.lastActive && active) {
        hidePrompt();
    }
    s_shot.lastActive = active;
}

#else

void init(Controller *) {}
void update() {}
void onLoad() {}
void toggle() {}
void setTemperatureFromPointer(lv_event_t *) {}
void setPressureFromPointer(lv_event_t *) {}
void setFlowFromPointer(lv_event_t *) {}
void save() {}
void discard() {}

#endif
} // namespace NativeManualBrew
