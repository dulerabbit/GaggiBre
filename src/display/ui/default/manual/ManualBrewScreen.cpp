#include "ManualBrewScreen.h"
#include "ManualBrewTimeline.h"

#include <Arduino.h>
#include <algorithm>
#include <cmath>
#include <display/core/Controller.h>
#include <display/core/ProfileManager.h>
#include <display/core/constants.h>
#include <display/core/process/BrewProcess.h>
#include <display/core/process/Process.h>
#include <display/ui/default/eez/images.h>
#include <display/ui/default/eez/screens.h>
#include <vector>

namespace ManualBrewScreen {
namespace {

Controller *s_controller = nullptr;
lv_obj_t *s_screen = nullptr;
lv_obj_t *s_title = nullptr;
lv_obj_t *s_tempLabel = nullptr;
lv_obj_t *s_pressureLabel = nullptr;
lv_obj_t *s_targetPressureLabel = nullptr;
lv_obj_t *s_elapsedLabel = nullptr;
lv_obj_t *s_flowLabel = nullptr;
lv_obj_t *s_weightLabel = nullptr;
lv_obj_t *s_tempZone = nullptr;
lv_obj_t *s_pressureZone = nullptr;
lv_obj_t *s_pressureFill = nullptr;
lv_obj_t *s_startBtn = nullptr;
lv_obj_t *s_backBtn = nullptr;
lv_obj_t *s_chart = nullptr;
lv_chart_series_t *s_chartPressure = nullptr;
lv_chart_series_t *s_chartTemp = nullptr;
lv_chart_series_t *s_chartFlow = nullptr;
lv_obj_t *s_savePanel = nullptr;
lv_obj_t *s_saveNameLabel = nullptr;

Profile s_profileBackup{};
bool s_profileBackupValid = false;
int s_tempPixels = 0;
uint32_t s_shotStartMs = 0;
uint32_t s_shotDurationMs = 0;
bool s_shotInProgress = false;
bool s_lastActive = false;
uint32_t s_lastChartMs = 0;
std::vector<std::pair<uint32_t, float>> s_timeline;

static constexpr int MANUAL_TEMP_PIXELS_PER_STEP = 50;
static constexpr int UI_W = 480;
static constexpr int UI_H = 480;

static void recordPressurePoint(float pressure) {
    if (!s_shotInProgress) {
        return;
    }
    const uint32_t nowMs = millis();
    const uint32_t elapsedMs = nowMs >= s_shotStartMs ? (nowMs - s_shotStartMs) : 0;
    if (!s_timeline.empty() && fabsf(s_timeline.back().second - pressure) < 0.01f) {
        return;
    }
    s_timeline.emplace_back(elapsedMs, pressure);
}

static int parseManualProfileNumber(const String &label) {
    if (label == "Manual") {
        return 1;
    }
    if (!label.startsWith("Manual ")) {
        return 0;
    }
    return label.substring(7).toInt();
}

static int findNextManualProfileNumber() {
    auto *pm = s_controller->getProfileManager();
    if (!pm) {
        return 1;
    }
    int maxN = 0;
    for (const auto &id : s_controller->getSettings().getProfileOrder()) {
        Profile p{};
        if (!pm->loadProfile(id, p)) {
            continue;
        }
        maxN = std::max(maxN, parseManualProfileNumber(p.label));
    }
    return maxN + 1;
}

static void updatePressureFill(float targetBar) {
    if (!s_pressureFill || !s_pressureZone) {
        return;
    }
    const float scaling = std::min(s_controller->getSettings().getPressureScaling(), 12.0f);
    const float t = scaling > 0.0f ? constrain(targetBar / scaling, 0.0f, 1.0f) : 0.0f;
    const lv_coord_t zoneH = lv_obj_get_height(s_pressureZone);
    const lv_coord_t fillH = static_cast<lv_coord_t>(lroundf(t * static_cast<float>(zoneH)));
    lv_obj_set_height(s_pressureFill, fillH);
    lv_obj_align(s_pressureFill, LV_ALIGN_BOTTOM_MID, 0, 0);
}

static void onTempZone(lv_event_t *e) {
    const lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST || code == LV_EVENT_PRESSED) {
        s_tempPixels = 0;
        return;
    }
    if (code != LV_EVENT_PRESSING) {
        return;
    }
    lv_indev_t *indev = lv_indev_get_act();
    if (!indev) {
        return;
    }
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
    s_tempPixels += -vect.y;
    const int steps = s_tempPixels / MANUAL_TEMP_PIXELS_PER_STEP;
    if (steps != 0) {
        s_tempPixels -= steps * MANUAL_TEMP_PIXELS_PER_STEP;
        const float cur = s_controller->getTargetTemp();
        const float next = constrain(cur + static_cast<float>(steps), static_cast<float>(MIN_TEMP), static_cast<float>(MAX_TEMP));
        s_controller->setTargetTemp(next);
        s_controller->getUI()->markDirty();
    }
}

static void onPressureZone(lv_event_t *e) {
    const lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_PRESSING && code != LV_EVENT_PRESSED) {
        return;
    }
    lv_indev_t *indev = lv_indev_get_act();
    if (!indev) {
        return;
    }
    lv_point_t point;
    lv_indev_get_point(indev, &point);
    lv_area_t area;
    lv_obj_get_coords(s_pressureZone, &area);
    const float localY = static_cast<float>(point.y - area.y1);
    const float height = static_cast<float>(lv_area_get_height(&area));
    const float scaling = std::min(s_controller->getSettings().getPressureScaling(), 12.0f);
    const float t = height > 0.0f ? constrain(localY / height, 0.0f, 1.0f) : 0.0f;
    const float newPressure = constrain(scaling * (1.0f - t), 0.0f, scaling);
    s_controller->setManualPressureTarget(newPressure);
    recordPressurePoint(newPressure);
    updatePressureFill(newPressure);
    s_controller->getUI()->markDirty();
}

static void onToggle(lv_event_t *e) {
    LV_UNUSED(e);
    if (s_controller->getMode() != MODE_MANUAL) {
        s_controller->setMode(MODE_MANUAL);
    }

    if (s_controller->isActive()) {
        s_controller->deactivate();
        if (s_shotInProgress) {
            s_shotDurationMs = (millis() >= s_shotStartMs) ? (millis() - s_shotStartMs) : 0;
            s_shotInProgress = false;
        }
    } else {
        if (s_savePanel) {
            lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
        }
        s_controller->activate();
        s_shotStartMs = millis();
        s_shotDurationMs = 0;
        s_shotInProgress = true;
        s_timeline.clear();
        recordPressurePoint(s_controller->getManualPressureTarget());
        if (s_chart && s_chartPressure && s_chartTemp && s_chartFlow) {
            lv_chart_set_all_value(s_chart, s_chartPressure, LV_CHART_POINT_NONE);
            lv_chart_set_all_value(s_chart, s_chartTemp, LV_CHART_POINT_NONE);
            lv_chart_set_all_value(s_chart, s_chartFlow, LV_CHART_POINT_NONE);
        }
    }
    s_controller->getUI()->markDirty();
}

static void onBack(lv_event_t *e) {
    LV_UNUSED(e);
    if (s_controller->isActive()) {
        s_controller->deactivate();
        s_controller->clear();
    }
    restoreProfile();
    s_controller->deactivate();
    s_controller->getUI()->changeScreen(SCREEN_ID_MENU_SCREEN_NEW);
}

static void onSave(lv_event_t *e) {
    LV_UNUSED(e);
    auto *pm = s_controller->getProfileManager();
    if (!pm) {
        return;
    }

    int nextNumber = 0;
    if (s_saveNameLabel) {
        const char *labelText = lv_label_get_text(s_saveNameLabel);
        if (labelText) {
            nextNumber = parseManualProfileNumber(String(labelText));
        }
    }
    if (nextNumber <= 0) {
        nextNumber = findNextManualProfileNumber();
    }

    Profile newProfile = pm->getSelectedProfile();
    newProfile.id = "";
    newProfile.label = "Manual " + String(nextNumber);
    applyManualTimelineToProfile(newProfile, s_timeline, s_shotDurationMs, newProfile.temperature,
                                 s_controller->getSettings().getPressureScaling());

    if (pm->saveProfile(newProfile)) {
        auto profileOrder = s_controller->getSettings().getProfileOrder();
        if (std::find(profileOrder.begin(), profileOrder.end(), newProfile.id) == profileOrder.end()) {
            profileOrder.push_back(newProfile.id);
            s_controller->getSettings().setProfileOrder(std::move(profileOrder));
        }
        pm->addFavoritedProfile(newProfile.id);
        pm->selectProfile(newProfile.id);
        if (s_savePanel) {
            lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
        }
        s_timeline.clear();
        s_shotDurationMs = 0;
        s_profileBackupValid = false;
    }
}

static void onDiscard(lv_event_t *e) {
    LV_UNUSED(e);
    if (s_savePanel) {
        lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
    }
    s_timeline.clear();
    s_shotDurationMs = 0;
}

static lv_obj_t *makeLabel(lv_obj_t *parent, const char *text, lv_coord_t x, lv_coord_t y, const lv_font_t *font) {
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
    if (font) {
        lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
    }
    lv_obj_set_pos(label, x, y);
    return label;
}

static void buildUi() {
    if (s_screen) {
        return;
    }

    s_screen = lv_obj_create(nullptr);
    lv_obj_set_size(s_screen, UI_W, UI_H);
    lv_obj_set_style_bg_color(s_screen, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);

    s_title = makeLabel(s_screen, "Manual Brew", 24, 18, &lv_font_montserrat_20);

    s_backBtn = lv_btn_create(s_screen);
    lv_obj_set_size(s_backBtn, 72, 40);
    lv_obj_set_pos(s_backBtn, 24, 420);
    lv_obj_set_style_bg_color(s_backBtn, lv_color_hex(0x2A2A2A), LV_PART_MAIN);
    lv_obj_set_style_radius(s_backBtn, 8, LV_PART_MAIN);
    lv_obj_add_event_cb(s_backBtn, onBack, LV_EVENT_CLICKED, nullptr);
    lv_obj_t *backLbl = lv_label_create(s_backBtn);
    lv_label_set_text(backLbl, "Menu");
    lv_obj_center(backLbl);

    s_startBtn = lv_btn_create(s_screen);
    lv_obj_set_size(s_startBtn, 88, 88);
    lv_obj_set_pos(s_startBtn, 196, 360);
    lv_obj_set_style_radius(s_startBtn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_startBtn, lv_color_hex(0x2CA4F6), LV_PART_MAIN);
    lv_obj_set_style_bg_img_src(s_startBtn, &img_play_40x40, LV_PART_MAIN);
    lv_obj_add_event_cb(s_startBtn, onToggle, LV_EVENT_CLICKED, nullptr);
    lv_obj_set_ext_click_area(s_startBtn, 20);

    // Left temp swipe zone
    s_tempZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_tempZone, 140, 280);
    lv_obj_set_pos(s_tempZone, 16, 60);
    lv_obj_set_style_bg_opa(s_tempZone, LV_OPA_10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_tempZone, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_border_width(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_tempZone, 12, LV_PART_MAIN);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_tempZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(s_tempZone, onTempZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_tempZone, 20);

    s_tempLabel = makeLabel(s_tempZone, "93 C", 20, 110, &lv_font_montserrat_24);
    makeLabel(s_tempZone, "TEMP", 36, 20, &lv_font_montserrat_14);
    makeLabel(s_tempZone, "swipe", 34, 240, &lv_font_montserrat_14);

    // Right pressure absolute strip
    s_pressureZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_pressureZone, 72, 280);
    lv_obj_set_pos(s_pressureZone, 392, 60);
    lv_obj_set_style_bg_color(s_pressureZone, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_pressureZone, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_pressureZone, 12, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_pressureZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(s_pressureZone, onPressureZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_pressureZone, 20);

    s_pressureFill = lv_obj_create(s_pressureZone);
    lv_obj_set_width(s_pressureFill, 72);
    lv_obj_set_style_bg_color(s_pressureFill, lv_color_hex(0x2CA4F6), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_pressureFill, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureFill, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_pressureFill, 12, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureFill, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    s_targetPressureLabel = makeLabel(s_screen, "0.0 bar", 300, 70, &lv_font_montserrat_18);
    s_pressureLabel = makeLabel(s_screen, "0.0", 300, 100, &lv_font_montserrat_24);
    makeLabel(s_screen, "PRESS", 300, 140, &lv_font_montserrat_14);

    // Center chart + live stats
    s_chart = lv_chart_create(s_screen);
    lv_obj_set_size(s_chart, 200, 140);
    lv_obj_set_pos(s_chart, 170, 70);
    lv_chart_set_type(s_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(s_chart, 60);
    lv_chart_set_range(s_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 120);
    lv_obj_set_style_bg_opa(s_chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_chart, 0, LV_PART_MAIN);
    s_chartPressure = lv_chart_add_series(s_chart, lv_color_hex(0x2CA4F6), LV_CHART_AXIS_PRIMARY_Y);
    s_chartTemp = lv_chart_add_series(s_chart, lv_color_hex(0xE07040), LV_CHART_AXIS_PRIMARY_Y);
    s_chartFlow = lv_chart_add_series(s_chart, lv_color_hex(0x70C070), LV_CHART_AXIS_PRIMARY_Y);

    s_elapsedLabel = makeLabel(s_screen, "0:00", 180, 230, &lv_font_montserrat_20);
    s_flowLabel = makeLabel(s_screen, "-- ml/s", 180, 265, &lv_font_montserrat_16);
    s_weightLabel = makeLabel(s_screen, "-- g", 180, 295, &lv_font_montserrat_16);

    // Save panel (shown after shot)
    s_savePanel = lv_obj_create(s_screen);
    lv_obj_set_size(s_savePanel, 320, 140);
    lv_obj_center(s_savePanel);
    lv_obj_set_style_bg_color(s_savePanel, lv_color_hex(0x1E1E1E), LV_PART_MAIN);
    lv_obj_set_style_border_color(s_savePanel, lv_color_hex(0x2CA4F6), LV_PART_MAIN);
    lv_obj_set_style_border_width(s_savePanel, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(s_savePanel, 12, LV_PART_MAIN);
    lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);

    makeLabel(s_savePanel, "Save as profile?", 70, 12, &lv_font_montserrat_16);
    s_saveNameLabel = makeLabel(s_savePanel, "Manual 1", 100, 42, &lv_font_montserrat_20);

    lv_obj_t *saveBtn = lv_btn_create(s_savePanel);
    lv_obj_set_size(saveBtn, 110, 40);
    lv_obj_set_pos(saveBtn, 30, 80);
    lv_obj_set_style_bg_color(saveBtn, lv_color_hex(0x2CA4F6), LV_PART_MAIN);
    lv_obj_add_event_cb(saveBtn, onSave, LV_EVENT_CLICKED, nullptr);
    lv_obj_t *saveLbl = lv_label_create(saveBtn);
    lv_label_set_text(saveLbl, "Save");
    lv_obj_center(saveLbl);

    lv_obj_t *discardBtn = lv_btn_create(s_savePanel);
    lv_obj_set_size(discardBtn, 110, 40);
    lv_obj_set_pos(discardBtn, 170, 80);
    lv_obj_set_style_bg_color(discardBtn, lv_color_hex(0x444444), LV_PART_MAIN);
    lv_obj_add_event_cb(discardBtn, onDiscard, LV_EVENT_CLICKED, nullptr);
    lv_obj_t *discardLbl = lv_label_create(discardBtn);
    lv_label_set_text(discardLbl, "Discard");
    lv_obj_center(discardLbl);
}

} // namespace

void init(Controller *controller) { s_controller = controller; }

void show() {
    buildUi();
    if (s_screen && lv_scr_act() != s_screen) {
        lv_scr_load(s_screen);
    }
    updatePressureFill(s_controller->getManualPressureTarget());
}

void hide() {
    // Screen stays allocated; EEZ will replace the active screen on next load.
}

bool isActive() { return s_screen != nullptr && lv_scr_act() == s_screen; }

void backupProfile() {
    if (!s_controller) {
        return;
    }
    s_profileBackup = s_controller->getProfileManager()->getSelectedProfile();
    s_profileBackupValid = true;
}

void restoreProfile() {
    if (!s_controller || !s_profileBackupValid) {
        return;
    }
    s_controller->getProfileManager()->getSelectedProfile() = s_profileBackup;
    s_profileBackupValid = false;
    s_tempPixels = 0;
    if (s_controller->getUI()) {
        s_controller->getUI()->markDirty();
    }
}

void onMenuIconApply(lv_obj_t *grindBtn) {
    if (!grindBtn || !s_controller) {
        return;
    }
    const int secondary = s_controller->getSettings().getSecondaryAction();
    if (secondary == SECONDARY_ACTION_MANUAL_BREW) {
        lv_obj_set_style_bg_img_src(grindBtn, &img_manual_pressure_80x80, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_recolor(grindBtn, lv_color_hex(0x2CA4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_recolor_opa(grindBtn, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_bg_img_src(grindBtn, &img_coffee_bean_80x80, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_recolor_opa(grindBtn, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void update() {
    if (!isActive() || !s_controller) {
        return;
    }

    const bool active = s_controller->isActive();
    lv_label_set_text_fmt(s_tempLabel, "%d C", static_cast<int>(s_controller->getTargetTemp()));
    lv_label_set_text_fmt(s_pressureLabel, "%.1f", s_controller->getCurrentPressure());
    lv_label_set_text_fmt(s_targetPressureLabel, "%.1f bar", s_controller->getManualPressureTarget());
    updatePressureFill(s_controller->getManualPressureTarget());

    if (s_startBtn) {
        lv_obj_set_style_bg_img_src(s_startBtn, active ? &img_pause_40x40 : &img_play_40x40, LV_PART_MAIN);
    }

    // Show save panel on trailing edge of a finished shot.
    if (s_lastActive && !active && s_savePanel) {
        const int nextNumber = findNextManualProfileNumber();
        if (s_saveNameLabel) {
            lv_label_set_text_fmt(s_saveNameLabel, "Manual %d", nextNumber);
        }
        lv_obj_clear_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
    }
    if (!s_lastActive && active && s_savePanel) {
        lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
    }
    s_lastActive = active;

    Process *proc = s_controller->getProcess();
    if (active && proc && proc->getType() == MODE_BREW) {
        auto *bp = static_cast<BrewProcess *>(proc);
        const unsigned long secs = (millis() - bp->processStarted) / 1000;
        lv_label_set_text_fmt(s_elapsedLabel, "%lu:%02lu", secs / 60, secs % 60);
        lv_label_set_text_fmt(s_flowLabel, "%.1f ml/s", bp->currentFlow);
        if (s_controller->isBluetoothScaleHealthy()) {
            // Weight comes via DefaultUI bluetooth path; show process volume as fallback.
            lv_label_set_text_fmt(s_weightLabel, "%.1f g", bp->currentVolume);
        } else {
            lv_label_set_text_fmt(s_weightLabel, "%.1f g", bp->currentVolume);
        }

        const uint32_t now = millis();
        if (now - s_lastChartMs >= 1000 && s_chart) {
            s_lastChartMs = now;
            lv_chart_set_next_value(s_chart, s_chartPressure, static_cast<lv_coord_t>(s_controller->getCurrentPressure() * 10.0f));
            lv_chart_set_next_value(s_chart, s_chartTemp, static_cast<lv_coord_t>(s_controller->getCurrentTemp()));
            lv_chart_set_next_value(s_chart, s_chartFlow, static_cast<lv_coord_t>(bp->currentFlow * 10.0f));
            recordPressurePoint(s_controller->getManualPressureTarget());
        }
    } else if (!active) {
        lv_label_set_text(s_elapsedLabel, "0:00");
        lv_label_set_text(s_flowLabel, "-- ml/s");
        lv_label_set_text(s_weightLabel, "-- g");
    }
}

} // namespace ManualBrewScreen
