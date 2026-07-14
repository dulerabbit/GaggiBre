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
#include <display/ui/default/eez/actions.h>
#include <display/ui/default/eez/images.h>
#include <display/ui/default/eez/screens.h>
#include <display/ui/default/eez/ui.h>
#include <display/ui/default/wide/WideLayout.h>
#include <vector>

namespace ManualBrewScreen {
namespace {

Controller *s_controller = nullptr;
lv_obj_t *s_screen = nullptr;
lv_obj_t *s_title = nullptr;
lv_obj_t *s_tempLabel = nullptr;          // live temp (top) / wide set-temp
lv_obj_t *s_pressureLabel = nullptr;      // live pressure (top) / wide live bar
lv_obj_t *s_targetPressureLabel = nullptr;
lv_obj_t *s_setTempLabel = nullptr;       // circular: red set-temp satellite
lv_obj_t *s_elapsedLabel = nullptr;
lv_obj_t *s_flowLabel = nullptr;
lv_obj_t *s_weightLabel = nullptr;
lv_obj_t *s_tempZone = nullptr;
lv_obj_t *s_pressureZone = nullptr;
lv_obj_t *s_pressureFill = nullptr;
lv_obj_t *s_startBtn = nullptr;
lv_obj_t *s_backBtn = nullptr;
lv_obj_t *s_chart = nullptr; // wide only
lv_chart_series_t *s_chartPressure = nullptr;
lv_chart_series_t *s_chartTemp = nullptr;
lv_chart_series_t *s_chartFlow = nullptr;
lv_obj_t *s_savePanel = nullptr;
lv_obj_t *s_saveNameLabel = nullptr;

// Circular dial widgets (LilyGo 480×480 race-car cluster)
lv_obj_t *s_tempMeter = nullptr;
lv_obj_t *s_pressureMeter = nullptr;
lv_meter_indicator_t *s_tempNeedle = nullptr;
lv_meter_indicator_t *s_tempFill = nullptr;
lv_meter_indicator_t *s_pressureNeedle = nullptr;
lv_meter_indicator_t *s_pressureArcFill = nullptr;
lv_obj_t *s_contentPanel = nullptr;
lv_obj_t *s_pressureCircle = nullptr;

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
// Match EEZ circular brew/dials control anchors (screen-center offsets).
static constexpr lv_coord_t kCircularPlayY = 130;
static constexpr lv_coord_t kCircularBackY = 210;

struct RaceClusterLayout {
    lv_coord_t panelW;
    lv_coord_t panelH;
    lv_coord_t gaugeSize;
    lv_coord_t gaugeY;
    lv_coord_t satSize;
    lv_coord_t satX[4];
    lv_coord_t satY[4];
};

// LilyGo 480×480
static constexpr RaceClusterLayout kCircularCluster = {
    360, 360, 156, -92, 72, {-118, -40, 40, 118}, {11, 41, 41, 11},
};
// Waveshare 800×480 — arc under gauge (LilyGo pattern scaled), above play
static constexpr RaceClusterLayout kWideCluster = {
    400, 360, 180, -70, 84, {-138, -47, 47, 138}, {75, 110, 110, 75},
};

// Wide tick columns (match WideLayout)
static constexpr int kWideColW = 150;
static constexpr int kWideColPadX = 4;
static constexpr int kWideLabelGap = 8;
static constexpr int kWideIconBandH = 52; // bottom thermometer/tach row
static constexpr int kWideSwipeTopY = 16; // below live readout band

static int uiW() {
    lv_disp_t *disp = lv_disp_get_default();
    return disp ? lv_disp_get_hor_res(disp) : 480;
}

static int uiH() {
    lv_disp_t *disp = lv_disp_get_default();
    return disp ? lv_disp_get_ver_res(disp) : 480;
}

static bool isWide() { return uiW() > 481; }

static uint32_t themeColor(int index) { return theme_colors[eez_flow_get_selected_theme_index()][index]; }

static bool savePanelVisible() { return s_savePanel && !lv_obj_has_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN); }

static void hideSavePanel() {
    if (s_savePanel) {
        lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
    }
}

static void resetShotPromptState() {
    hideSavePanel();
    s_lastActive = false;
    s_shotInProgress = false;
    s_shotDurationMs = 0;
    s_timeline.clear();
}

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
    if (!s_pressureFill || !s_pressureZone || !isWide()) {
        return;
    }
    const float scaling = std::min(s_controller->getSettings().getPressureScaling(), 12.0f);
    const float t = scaling > 0.0f ? constrain(targetBar / scaling, 0.0f, 1.0f) : 0.0f;
    const lv_coord_t zoneH = lv_obj_get_height(s_pressureZone);
    const lv_coord_t fillH = static_cast<lv_coord_t>(lroundf(t * static_cast<float>(zoneH)));
    lv_obj_set_height(s_pressureFill, fillH);
    lv_obj_align(s_pressureFill, LV_ALIGN_BOTTOM_MID, 0, 0);
}

static void clearChart() {
    if (!s_chart) {
        return;
    }
    if (s_chartPressure) {
        lv_chart_set_all_value(s_chart, s_chartPressure, LV_CHART_POINT_NONE);
    }
    if (s_chartTemp) {
        lv_chart_set_all_value(s_chart, s_chartTemp, LV_CHART_POINT_NONE);
    }
    if (s_chartFlow) {
        lv_chart_set_all_value(s_chart, s_chartFlow, LV_CHART_POINT_NONE);
    }
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
    const float scaling = std::min(s_controller->getSettings().getPressureScaling(), 12.0f);
    float newPressure;
    if (isWide()) {
        // Map against the lit tick band (below readout, above bottom icons) so
        // full-height swipes on the bars reach 0 .. scaling.
        const float yTop = static_cast<float>(kWideSwipeTopY);
        const float yBot = static_cast<float>(uiH() - kWideIconBandH);
        const float span = std::max(yBot - yTop, 1.0f);
        const float t = constrain((static_cast<float>(point.y) - yTop) / span, 0.0f, 1.0f);
        newPressure = constrain(scaling * (1.0f - t), 0.0f, scaling);
    } else {
        // SquareLine stable: absolute screen Y / 400 → 0..scaling (top = max)
        const float t = constrain(static_cast<float>(point.y) / 400.0f, 0.0f, 1.0f);
        newPressure = constrain(scaling * (1.0f - t), 0.0f, scaling);
    }
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
        hideSavePanel();
        s_controller->activate();
        s_shotStartMs = millis();
        s_shotDurationMs = 0;
        s_shotInProgress = true;
        s_timeline.clear();
        recordPressurePoint(s_controller->getManualPressureTarget());
        clearChart();
    }
    s_controller->getUI()->markDirty();
}

static void onBack(lv_event_t *e) {
    const lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_GESTURE) {
        if (lv_indev_get_gesture_dir(lv_indev_get_act()) != LV_DIR_TOP) {
            return;
        }
        if (isWide()) {
            lv_point_t p{};
            lv_indev_get_point(lv_indev_get_act(), &p);
            const lv_coord_t left = WideLayout::kPlayX - WideLayout::kSwipeBackMarginPx;
            const lv_coord_t right = WideLayout::kPlayX + WideLayout::kPlaySize + WideLayout::kSwipeBackMarginPx;
            if (p.x < left || p.x > right) {
                return;
            }
        }
        lv_indev_wait_release(lv_indev_get_act());
    } else if (code != LV_EVENT_CLICKED) {
        return;
    }
    if (s_controller->isActive()) {
        s_controller->deactivate();
        s_controller->clear();
    }
    resetShotPromptState();
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
        hideSavePanel();
        s_timeline.clear();
        s_shotDurationMs = 0;
        s_shotInProgress = false;
        s_profileBackupValid = false;
    }
}

static void onDiscard(lv_event_t *e) {
    LV_UNUSED(e);
    hideSavePanel();
    s_timeline.clear();
    s_shotDurationMs = 0;
    s_shotInProgress = false;
}

static void onMeterDraw(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_DRAW_POST) {
        action_on_meter_draw(e);
    }
}

static void suppressMeterDefaultTicks(lv_obj_t *meter) {
    if (!meter || !lv_obj_check_type(meter, &lv_meter_class)) {
        return;
    }
    auto *m = reinterpret_cast<lv_meter_t *>(meter);
    auto *scale = static_cast<lv_meter_scale_t *>(_lv_ll_get_head(&m->scale_ll));
    if (scale != nullptr && scale->tick_cnt > 0) {
        scale->tick_major_nth = scale->tick_cnt;
        scale->tick_cnt = 0;
    }
}

static void buildCircularDials(lv_obj_t *parent) {
    const uint32_t tickCol = themeColor(5);
    const uint32_t tempCol = themeColor(6);
    const uint32_t pressCol = themeColor(7);

    s_tempMeter = lv_meter_create(parent);
    lv_obj_set_size(s_tempMeter, 500, 500);
    lv_obj_set_style_align(s_tempMeter, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_tempMeter, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_opa(s_tempMeter, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_clear_flag(s_tempMeter, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_meter_scale_t *scale = lv_meter_add_scale(s_tempMeter);
        lv_meter_set_scale_ticks(s_tempMeter, scale, 25, 10, 25, lv_color_hex(tickCol));
        lv_meter_set_scale_major_ticks(s_tempMeter, scale, 0, 0, 0, lv_color_hex(0x000000), 0);
        lv_meter_set_scale_range(s_tempMeter, scale, 0, 160, 120, 120);
        s_tempNeedle = lv_meter_add_needle_img(s_tempMeter, scale, &img_indicator_small, -233, 0);
        s_tempFill = lv_meter_add_scale_lines(s_tempMeter, scale, lv_color_hex(tempCol), lv_color_hex(tempCol), false, 0);
        lv_meter_set_indicator_start_value(s_tempMeter, s_tempFill, 0);
    }
    lv_obj_add_event_cb(s_tempMeter, onMeterDraw, LV_EVENT_DRAW_POST, nullptr);
    suppressMeterDefaultTicks(s_tempMeter);

    s_pressureMeter = lv_meter_create(parent);
    lv_obj_set_size(s_pressureMeter, 500, 500);
    lv_obj_set_style_align(s_pressureMeter, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_pressureMeter, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_opa(s_pressureMeter, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_clear_flag(s_pressureMeter, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    {
        // Match EEZ pressure dial: inverted 160→0 over 120° starting at 300°
        lv_meter_scale_t *scale = lv_meter_add_scale(s_pressureMeter);
        lv_meter_set_scale_ticks(s_pressureMeter, scale, 25, 10, 25, lv_color_hex(tickCol));
        lv_meter_set_scale_major_ticks(s_pressureMeter, scale, 0, 0, 0, lv_color_hex(0x000000), 0);
        lv_meter_set_scale_range(s_pressureMeter, scale, 160, 0, 120, 300);
        s_pressureNeedle = lv_meter_add_needle_img(s_pressureMeter, scale, &img_indicator_small, -233, 0);
        s_pressureArcFill =
            lv_meter_add_scale_lines(s_pressureMeter, scale, lv_color_hex(pressCol), lv_color_hex(pressCol), false, 0);
        lv_meter_set_indicator_start_value(s_pressureMeter, s_pressureArcFill, 0);
    }
    lv_obj_add_event_cb(s_pressureMeter, onMeterDraw, LV_EVENT_DRAW_POST, nullptr);
    suppressMeterDefaultTicks(s_pressureMeter);

    // Top corners: ALWAYS live sensor readouts (never follow swipe)
    const uint32_t tempColLive = themeColor(6);
    const uint32_t pressColLive = themeColor(7);
    s_tempLabel = lv_label_create(parent);
    lv_label_set_text(s_tempLabel, "93°C");
    lv_obj_set_style_text_font(s_tempLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_tempLabel, lv_color_hex(tempColLive), LV_PART_MAIN);
    lv_obj_align(s_tempLabel, LV_ALIGN_CENTER, -50, -205);

    s_pressureLabel = lv_label_create(parent);
    lv_label_set_text(s_pressureLabel, "0.0 bar");
    lv_obj_set_style_text_font(s_pressureLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_pressureLabel, lv_color_hex(pressColLive), LV_PART_MAIN);
    lv_obj_align(s_pressureLabel, LV_ALIGN_CENTER, 50, -205);

    // Bottom icons — same placement as EEZ dials on every other LilyGo screen
    lv_obj_t *tempIcon = lv_img_create(parent);
    lv_obj_set_pos(tempIcon, -85, 200);
    lv_img_set_src(tempIcon, &img_thermometer_half_40x40);
    lv_img_set_zoom(tempIcon, 150);
    lv_obj_set_style_align(tempIcon, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(tempIcon, lv_color_hex(tempColLive), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(tempIcon, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(tempIcon, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *pressIcon = lv_img_create(parent);
    lv_obj_set_pos(pressIcon, 85, 200);
    lv_img_set_src(pressIcon, &img_tachometer_fast_40x40);
    lv_img_set_zoom(pressIcon, 150);
    lv_obj_set_style_align(pressIcon, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(pressIcon, lv_color_hex(pressColLive), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(pressIcon, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(pressIcon, LV_OBJ_FLAG_CLICKABLE);
}

static lv_obj_t *makeSatellite(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t size, uint32_t borderColor,
                               const char *text, lv_obj_t **outLabel) {
    lv_obj_t *ring = lv_obj_create(parent);
    lv_obj_set_size(ring, size, size);
    lv_obj_align(ring, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(ring, size / 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ring, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ring, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(ring, lv_color_hex(borderColor), LV_PART_MAIN);
    lv_obj_set_style_border_width(ring, 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(ring, 0, LV_PART_MAIN);
    lv_obj_clear_flag(ring, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *label = lv_label_create(ring);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(borderColor), LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_center(label);
    if (outLabel) {
        *outLabel = label;
    }
    return ring;
}

static void buildRaceCluster(lv_obj_t *parent, const RaceClusterLayout &layout) {
    // Transparent host for center gauge + satellites (no chart — web UI owns analysis)
    const bool wide = isWide();
    s_contentPanel = lv_obj_create(parent);
    lv_obj_set_size(s_contentPanel, layout.panelW, layout.panelH);
    lv_obj_align(s_contentPanel, LV_ALIGN_CENTER, 0, wide ? -20 : 0);
    // LilyGo: circular clip matches the round bezel. Wide: no clip — sats must not be cut off.
    if (wide) {
        lv_obj_set_style_radius(s_contentPanel, 0, LV_PART_MAIN);
        lv_obj_set_style_clip_corner(s_contentPanel, false, LV_PART_MAIN);
    } else {
        lv_obj_set_style_radius(s_contentPanel, layout.panelW / 2, LV_PART_MAIN);
        lv_obj_set_style_clip_corner(s_contentPanel, true, LV_PART_MAIN);
    }
    lv_obj_set_style_bg_opa(s_contentPanel, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_contentPanel, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_contentPanel, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_contentPanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    s_chart = nullptr;
    s_chartPressure = nullptr;
    s_chartTemp = nullptr;
    s_chartFlow = nullptr;

    // Primary control: set-pressure gauge (follows right swipe)
    s_pressureCircle = lv_obj_create(s_contentPanel);
    lv_obj_set_size(s_pressureCircle, layout.gaugeSize, layout.gaugeSize);
    lv_obj_align(s_pressureCircle, LV_ALIGN_CENTER, 0, layout.gaugeY);
    lv_obj_set_style_radius(s_pressureCircle, layout.gaugeSize / 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_pressureCircle, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_pressureCircle, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(s_pressureCircle, lv_color_hex(0x00FFFF), LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureCircle, 3, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_pressureCircle, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureCircle, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    s_targetPressureLabel = lv_label_create(s_pressureCircle);
    lv_label_set_text(s_targetPressureLabel, "0.0");
    lv_obj_set_style_text_font(s_targetPressureLabel, &lv_font_montserrat_64_digits, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_targetPressureLabel, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
    lv_obj_set_style_text_align(s_targetPressureLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_center(s_targetPressureLabel);

    const uint32_t setTempCol = 0xF62C2C; // red = commanded temp (not live)
    const uint32_t timeCol = 0xE8E8E8;
    const uint32_t flowCol = 0x00CC44;
    const uint32_t weightCol = 0xF0A030;
    makeSatellite(s_contentPanel, layout.satX[0], layout.satY[0], layout.satSize, setTempCol, "93°", &s_setTempLabel);
    makeSatellite(s_contentPanel, layout.satX[1], layout.satY[1], layout.satSize, timeCol, "0:00", &s_elapsedLabel);
    makeSatellite(s_contentPanel, layout.satX[2], layout.satY[2], layout.satSize, flowCol, "--", &s_flowLabel);
    makeSatellite(s_contentPanel, layout.satX[3], layout.satY[3], layout.satSize, weightCol, "--g", &s_weightLabel);
}

static void buildSavePanel(lv_obj_t *parent) {
    s_savePanel = lv_obj_create(parent);
    lv_obj_set_size(s_savePanel, 360, 200);
    lv_obj_center(s_savePanel);
    lv_obj_set_style_bg_color(s_savePanel, lv_color_hex(themeColor(1)), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_savePanel, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_savePanel, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_savePanel, 30, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_savePanel, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(s_savePanel, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_savePanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *promptLabel = lv_label_create(s_savePanel);
    lv_label_set_text(promptLabel, "Save as profile?");
    lv_obj_set_style_text_color(promptLabel, lv_color_hex(themeColor(3)), LV_PART_MAIN);
    lv_obj_set_style_text_font(promptLabel, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_set_style_align(promptLabel, LV_ALIGN_TOP_MID, LV_PART_MAIN);
    lv_obj_set_y(promptLabel, 28);

    s_saveNameLabel = lv_label_create(s_savePanel);
    lv_label_set_text(s_saveNameLabel, "Manual 1");
    lv_obj_set_style_text_color(s_saveNameLabel, lv_color_hex(themeColor(0)), LV_PART_MAIN);
    lv_obj_set_style_text_font(s_saveNameLabel, &lv_font_montserrat_34, LV_PART_MAIN);
    lv_obj_set_style_text_align(s_saveNameLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_align(s_saveNameLabel, LV_ALIGN_TOP_MID, LV_PART_MAIN);
    lv_obj_set_y(s_saveNameLabel, 58);

    lv_obj_t *actionRow = lv_obj_create(s_savePanel);
    lv_obj_set_size(actionRow, 280, 72);
    lv_obj_align(actionRow, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_opa(actionRow, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(actionRow, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(actionRow, 0, LV_PART_MAIN);
    lv_obj_clear_flag(actionRow, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(actionRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(actionRow, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *discardBtn = lv_btn_create(actionRow);
    lv_obj_set_size(discardBtn, 72, 72);
    lv_obj_set_style_bg_opa(discardBtn, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(discardBtn, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(discardBtn, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(discardBtn, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(discardBtn, onDiscard, LV_EVENT_CLICKED, nullptr);
    lv_obj_t *discardLbl = lv_label_create(discardBtn);
    lv_label_set_text(discardLbl, "X");
    lv_obj_set_style_text_color(discardLbl, lv_color_hex(themeColor(0)), LV_PART_MAIN);
    lv_obj_set_style_text_font(discardLbl, &lv_font_montserrat_34, LV_PART_MAIN);
    lv_obj_center(discardLbl);

    lv_obj_t *saveBtn = lv_btn_create(actionRow);
    lv_obj_set_size(saveBtn, 72, 72);
    lv_obj_set_style_bg_opa(saveBtn, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(saveBtn, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(saveBtn, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_img_src(saveBtn, &img_check_40x40, LV_PART_MAIN);
    lv_obj_set_style_bg_img_recolor(saveBtn, lv_color_hex(themeColor(0)), LV_PART_MAIN);
    lv_obj_set_style_bg_img_recolor_opa(saveBtn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_event_cb(saveBtn, onSave, LV_EVENT_CLICKED, nullptr);
}

static void buildCircularUi(int W, int H) {
    LV_UNUSED(W);
    LV_UNUSED(H);

    buildCircularDials(s_screen);

    // SquareLine swipe hit-zones (invisible)
    s_tempZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_tempZone, 200, 360);
    lv_obj_align(s_tempZone, LV_ALIGN_CENTER, -140, 40);
    lv_obj_set_style_bg_opa(s_tempZone, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_tempZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(s_tempZone, onTempZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_tempZone, 20);

    s_pressureZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_pressureZone, 170, 430);
    lv_obj_align(s_pressureZone, LV_ALIGN_CENTER, 115, 0);
    lv_obj_set_style_bg_opa(s_pressureZone, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_pressureZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(s_pressureZone, onPressureZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_pressureZone, 20);

    s_pressureFill = nullptr;
    s_title = nullptr;

    buildRaceCluster(s_screen, kCircularCluster);

    s_startBtn = lv_img_create(s_screen);
    lv_img_set_src(s_startBtn, &img_play_40x40);
    lv_obj_add_flag(s_startBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(s_startBtn, 28);
    lv_obj_set_style_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_event_cb(s_startBtn, onToggle, LV_EVENT_CLICKED, nullptr);
    lv_obj_align(s_startBtn, LV_ALIGN_CENTER, 0, kCircularPlayY);

    s_backBtn = lv_img_create(s_screen);
    lv_img_set_src(s_backBtn, &img_angle_up_40x40);
    lv_obj_add_flag(s_backBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(s_backBtn, 20);
    lv_obj_set_style_img_recolor(s_backBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(s_backBtn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_event_cb(s_backBtn, onBack, LV_EVENT_CLICKED, nullptr);
    lv_obj_align(s_backBtn, LV_ALIGN_CENTER, 0, kCircularBackY);

    buildSavePanel(s_screen);
}

static void buildWideUi(int W, int H) {
    const int colW = kWideColW;
    const int colH = H;
    const int colPadX = kWideColPadX;
    const int colTop = 0;
    const int leftInner = colPadX + colW;          // 154 — right edge of left ticks
    const int rightInner = W - colPadX - colW;     // 646 — left edge of right ticks
    const int iconCxLeft = colPadX + colW / 2;
    const int iconCxRight = W - colPadX - colW / 2;

    WideLayout::addStandaloneTicks(s_screen);
    WideLayout::setTickLength(WideLayout::kTickLong);

    s_title = nullptr;

    s_tempZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_tempZone, colW, colH);
    lv_obj_set_pos(s_tempZone, colPadX, colTop);
    lv_obj_set_style_bg_opa(s_tempZone, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_tempZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(s_tempZone, onTempZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_tempZone, 20);

    const uint32_t tempColor = theme_colors[eez_flow_get_selected_theme_index()][6];
    // Live temp in center gutter, flush to inner edge of left tick column
    s_tempLabel = lv_label_create(s_screen);
    lv_label_set_text(s_tempLabel, "93°C");
    lv_obj_set_style_text_font(s_tempLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_tempLabel, lv_color_hex(tempColor), LV_PART_MAIN);
    lv_obj_set_style_text_align(s_tempLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_obj_set_pos(s_tempLabel, leftInner + kWideLabelGap, 10);

    lv_obj_t *tempIcon = lv_img_create(s_screen);
    lv_img_set_src(tempIcon, &img_thermometer_half_40x40);
    lv_img_set_zoom(tempIcon, 150);
    lv_obj_set_style_img_recolor(tempIcon, lv_color_hex(tempColor), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(tempIcon, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(tempIcon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(tempIcon, iconCxLeft - 20, H - kWideIconBandH);

    s_pressureZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_pressureZone, colW, colH);
    lv_obj_set_pos(s_pressureZone, W - colPadX - colW, colTop);
    lv_obj_set_style_bg_opa(s_pressureZone, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_pressureZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(s_pressureZone, onPressureZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_pressureZone, 20);

    s_pressureFill = lv_obj_create(s_pressureZone);
    lv_obj_set_width(s_pressureFill, colW);
    lv_obj_set_style_bg_color(s_pressureFill, lv_color_hex(0x2CA4F6), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_pressureFill, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureFill, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_pressureFill, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureFill, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    const uint32_t pressColor = theme_colors[eez_flow_get_selected_theme_index()][7];
    // Live pressure in center gutter — right edge flush to inner edge of right ticks
    s_pressureLabel = lv_label_create(s_screen);
    lv_label_set_text(s_pressureLabel, "0.0 bar");
    lv_obj_set_style_text_font(s_pressureLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_pressureLabel, lv_color_hex(pressColor), LV_PART_MAIN);
    lv_obj_set_style_text_align(s_pressureLabel, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
    lv_obj_align(s_pressureLabel, LV_ALIGN_TOP_RIGHT, -(W - rightInner + kWideLabelGap), 10);

    lv_obj_t *pressIcon = lv_img_create(s_screen);
    lv_img_set_src(pressIcon, &img_tachometer_fast_40x40);
    lv_img_set_zoom(pressIcon, 150);
    lv_obj_set_style_img_recolor(pressIcon, lv_color_hex(pressColor), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(pressIcon, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(pressIcon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(pressIcon, iconCxRight - 20, H - kWideIconBandH);

    buildRaceCluster(s_screen, kWideCluster);

    s_startBtn = lv_img_create(s_screen);
    lv_img_set_src(s_startBtn, &img_play_40x40);
    lv_obj_add_flag(s_startBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(s_startBtn, 28);
    lv_obj_set_style_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_event_cb(s_startBtn, onToggle, LV_EVENT_CLICKED, nullptr);
    lv_obj_set_pos(s_startBtn, WideLayout::kPlayX, WideLayout::kPlayY);

    // Swipe columns above cluster chrome so tick-band presses always hit
    lv_obj_move_foreground(s_tempZone);
    lv_obj_move_foreground(s_pressureZone);
    lv_obj_move_foreground(s_startBtn);

    lv_obj_add_event_cb(s_screen, onBack, LV_EVENT_GESTURE, nullptr);
    s_backBtn = nullptr;

    buildSavePanel(s_screen);
}

static void buildUi() {
    if (s_screen) {
        return;
    }

    const int W = uiW();
    const int H = uiH();
    const bool wide = isWide();

    s_screen = lv_obj_create(nullptr);
    lv_obj_set_size(s_screen, W, H);
    lv_obj_set_style_bg_color(s_screen, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);

    if (wide) {
        buildWideUi(W, H);
    } else {
        buildCircularUi(W, H);
    }
}

static void updateCircularDials() {
    const float targetTemp = s_controller->getTargetTemp();
    const float currentTemp = s_controller->getCurrentTemp();
    const float targetBar = s_controller->getManualPressureTarget();
    const float currentBar = s_controller->getCurrentPressure();
    const float scaling = std::min(s_controller->getSettings().getPressureScaling(), 12.0f);
    // Map bar → 0..160 meter domain used by EEZ pressure dial (10× bar, capped)
    const float scaleMax = std::max(scaling * 10.0f, 1.0f);
    const int32_t targetPressVal =
        static_cast<int32_t>(lroundf(constrain(targetBar * 10.0f, 0.0f, scaleMax) * (160.0f / scaleMax)));

    if (s_tempMeter && s_tempNeedle) {
        lv_meter_set_indicator_value(s_tempMeter, s_tempNeedle, static_cast<int32_t>(lroundf(targetTemp)));
    }
    if (s_tempMeter && s_tempFill) {
        lv_meter_set_indicator_end_value(s_tempMeter, s_tempFill, static_cast<int32_t>(lroundf(currentTemp)));
    }
    if (s_pressureMeter && s_pressureNeedle) {
        lv_meter_set_indicator_value(s_pressureMeter, s_pressureNeedle, targetPressVal);
    }
    // Freeze pressure dial graphics on setpoint — live bar only updates the number label.
    if (s_pressureMeter && s_pressureArcFill) {
        lv_meter_set_indicator_end_value(s_pressureMeter, s_pressureArcFill, targetPressVal);
    }

    // Top: live sensors only
    if (s_tempLabel) {
        lv_label_set_text_fmt(s_tempLabel, "%d°C", static_cast<int>(lroundf(currentTemp)));
    }
    if (s_pressureLabel) {
        lv_label_set_text_fmt(s_pressureLabel, "%.1f bar", currentBar);
    }
    // Center: set pressure
    if (s_targetPressureLabel) {
        lv_label_set_text_fmt(s_targetPressureLabel, "%.1f", targetBar);
        lv_obj_center(s_targetPressureLabel);
    }
    // Red satellite: set temp (follows left swipe, not live)
    if (s_setTempLabel) {
        lv_label_set_text_fmt(s_setTempLabel, "%d°", static_cast<int>(lroundf(targetTemp)));
        lv_obj_center(s_setTempLabel);
    }
}

} // namespace

void init(Controller *controller) { s_controller = controller; }

void show() {
    buildUi();
    if (WideLayout::isActive()) {
        WideLayout::setTickLength(WideLayout::kTickLong);
    }
    if (s_screen && lv_scr_act() != s_screen) {
        lv_scr_load(s_screen);
    }
    updatePressureFill(s_controller->getManualPressureTarget());
}

void hide() {
    // Screen stays allocated; EEZ will replace the active screen on next load.
    resetShotPromptState();
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
    const bool wide = isWide();

    // Both layouts use the race-car cluster: live top numbers, set pressure/temp in cluster.
    updateCircularDials();
    if (wide) {
        updatePressureFill(s_controller->getManualPressureTarget());
    }

    if (s_startBtn) {
        lv_img_set_src(s_startBtn, active ? &img_pause_40x40 : &img_play_40x40);
        lv_obj_set_style_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
    }

    if (s_lastActive && !active && s_savePanel) {
        if (s_shotInProgress) {
            s_shotDurationMs = (millis() >= s_shotStartMs) ? (millis() - s_shotStartMs) : 0;
            s_shotInProgress = false;
        }
        const bool hadShot = s_shotDurationMs > 0 || !s_timeline.empty();
        if (hadShot) {
            const int nextNumber = findNextManualProfileNumber();
            if (s_saveNameLabel) {
                lv_label_set_text_fmt(s_saveNameLabel, "Manual %d", nextNumber);
                lv_obj_align(s_saveNameLabel, LV_ALIGN_TOP_MID, 0, 58);
            }
            lv_obj_clear_flag(s_savePanel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(s_savePanel);
        }
    }
    if (!s_lastActive && active && s_savePanel) {
        hideSavePanel();
    }
    s_lastActive = active;

    Process *proc = s_controller->getProcess();
    const bool brewing = active && proc && proc->getType() == MODE_BREW;
    BrewProcess *bp = brewing ? static_cast<BrewProcess *>(proc) : nullptr;

    // Weight: prefer live BLE scale (same as main brew screen); fall back to brew volume.
    auto updateWeightLabel = [&]() {
        if (!s_weightLabel) {
            return;
        }
        if (s_controller->isBluetoothScaleHealthy()) {
            lv_label_set_text_fmt(s_weightLabel, "%.1fg", s_controller->getBluetoothWeight());
            lv_obj_center(s_weightLabel);
        } else if (bp) {
            lv_label_set_text_fmt(s_weightLabel, "%.1fg", bp->currentVolume);
            lv_obj_center(s_weightLabel);
        } else if (!active && !savePanelVisible()) {
            lv_label_set_text(s_weightLabel, "--g");
            lv_obj_center(s_weightLabel);
        }
    };

    if (bp) {
        const unsigned long secs = (millis() - bp->processStarted) / 1000;
        lv_label_set_text_fmt(s_elapsedLabel, "%lu:%02lu", secs / 60, secs % 60);
        lv_obj_center(s_elapsedLabel);
        lv_label_set_text_fmt(s_flowLabel, "%.1f", bp->currentFlow);
        lv_obj_center(s_flowLabel);
        updateWeightLabel();

        const uint32_t now = millis();
        if (now - s_lastChartMs >= 350u) {
            s_lastChartMs = now;
            recordPressurePoint(s_controller->getManualPressureTarget());
        }
    } else {
        updateWeightLabel();
        if (!active && !savePanelVisible()) {
            lv_label_set_text(s_elapsedLabel, "0:00");
            lv_obj_center(s_elapsedLabel);
            lv_label_set_text(s_flowLabel, "--");
            lv_obj_center(s_flowLabel);
        }
    }
}

} // namespace ManualBrewScreen
