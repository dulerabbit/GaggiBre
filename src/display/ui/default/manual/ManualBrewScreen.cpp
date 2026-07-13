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
#include <display/ui/default/eez/ui.h>
#include <display/ui/default/wide/WideLayout.h>
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
        hideSavePanel();
        s_controller->activate();
        s_shotStartMs = millis();
        s_shotDurationMs = 0;
        s_shotInProgress = true;
        s_timeline.clear();
        recordPressurePoint(s_controller->getManualPressureTarget());
        if (s_chart && s_chartPressure && s_chartFlow) {
            lv_chart_set_all_value(s_chart, s_chartPressure, LV_CHART_POINT_NONE);
            lv_chart_set_all_value(s_chart, s_chartFlow, LV_CHART_POINT_NONE);
        }
    }
    s_controller->getUI()->markDirty();
}

static void onBack(lv_event_t *e) {
    const lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_GESTURE) {
        if (lv_indev_get_gesture_dir(lv_indev_get_act()) != LV_DIR_TOP) {
            return;
        }
        // EEZ-style screen swipe, limited to play ±1cm (vertical swipe keeps X near start).
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

    const int W = uiW();
    const int H = uiH();
    const bool wide = isWide();

    const int colW = wide ? 150 : 72;
    const int colH = wide ? H : 280;
    const int colPadX = wide ? 4 : 16;
    const int colTop = wide ? 0 : (H - colH) / 2;
    const int centerLeft = wide ? 220 : (colPadX + colW + 16);
    const int centerRight = wide ? 580 : (W - colPadX - colW - 16);
    const int centerW = centerRight - centerLeft;

    s_screen = lv_obj_create(nullptr);
    lv_obj_set_size(s_screen, W, H);
    lv_obj_set_style_bg_color(s_screen, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);

    if (wide) {
        WideLayout::addStandaloneTicks(s_screen);
        WideLayout::setTickLength(WideLayout::kTickLong);
    }

    // No title on wide — shell matches other screens. Keep title on circular.
    s_title = wide ? nullptr : makeLabel(s_screen, "Manual Brew", centerLeft, 18, &lv_font_montserrat_20);

    // Left temp swipe zone
    s_tempZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_tempZone, wide ? colW : 140, colH);
    lv_obj_set_pos(s_tempZone, colPadX, colTop);
    lv_obj_set_style_bg_opa(s_tempZone, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_tempZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_tempZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(s_tempZone, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(s_tempZone, onTempZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_tempZone, 20);

    if (wide) {
        const uint32_t tempColor = theme_colors[eez_flow_get_selected_theme_index()][6];
        s_tempLabel = makeLabel(s_screen, "93 C", 155, 10, &lv_font_montserrat_24);
        lv_obj_set_style_text_color(s_tempLabel, lv_color_hex(tempColor), LV_PART_MAIN);
        lv_obj_t *tempIcon = lv_img_create(s_screen);
        lv_img_set_src(tempIcon, &img_thermometer_half_40x40);
        lv_img_set_zoom(tempIcon, 150);
        lv_obj_set_style_img_recolor(tempIcon, lv_color_hex(tempColor), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(tempIcon, LV_OPA_COVER, LV_PART_MAIN);
        // Match WideLayout: bottom-center of left tick column
        lv_obj_set_pos(tempIcon, colPadX + colW / 2 - 20, H - 52);
    } else {
        s_tempLabel = makeLabel(s_tempZone, "93 C", 20, colH / 2 - 20, &lv_font_montserrat_24);
        makeLabel(s_tempZone, "TEMP", 36, 20, &lv_font_montserrat_14);
        makeLabel(s_tempZone, "swipe", 34, colH - 40, &lv_font_montserrat_14);
    }

    // Right pressure swipe zone (+ fill for non-wide)
    s_pressureZone = lv_obj_create(s_screen);
    lv_obj_set_size(s_pressureZone, colW, colH);
    lv_obj_set_pos(s_pressureZone, W - colPadX - colW, colTop);
    lv_obj_set_style_bg_opa(s_pressureZone, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_pressureZone, wide ? 0 : 12, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_pressureZone, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_pressureZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(s_pressureZone, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(s_pressureZone, onPressureZone, LV_EVENT_ALL, nullptr);
    lv_obj_set_ext_click_area(s_pressureZone, 20);

    s_pressureFill = lv_obj_create(s_pressureZone);
    lv_obj_set_width(s_pressureFill, colW);
    lv_obj_set_style_bg_color(s_pressureFill, lv_color_hex(0x2CA4F6), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_pressureFill, wide ? LV_OPA_TRANSP : LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_pressureFill, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(s_pressureFill, wide ? 0 : 12, LV_PART_MAIN);
    lv_obj_clear_flag(s_pressureFill, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    if (wide) {
        const uint32_t pressColor = theme_colors[eez_flow_get_selected_theme_index()][7];
        // Target only at top-right; live pressure goes in the stats row under the graph
        s_targetPressureLabel = makeLabel(s_screen, "0.0 bar", 560, 10, &lv_font_montserrat_24);
        lv_obj_set_style_text_color(s_targetPressureLabel, lv_color_hex(pressColor), LV_PART_MAIN);
        lv_obj_t *pressIcon = lv_img_create(s_screen);
        lv_img_set_src(pressIcon, &img_tachometer_fast_40x40);
        lv_img_set_zoom(pressIcon, 150);
        lv_obj_set_style_img_recolor(pressIcon, lv_color_hex(pressColor), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(pressIcon, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_pos(pressIcon, W - colPadX - colW / 2 - 20, H - 52);
    } else {
        s_targetPressureLabel = makeLabel(s_screen, "0.0 bar", centerLeft + 20, 70, &lv_font_montserrat_18);
        s_pressureLabel = makeLabel(s_screen, "0.0", centerLeft + 20, 100, &lv_font_montserrat_24);
        makeLabel(s_screen, "PRESS", centerLeft + 20, 140, &lv_font_montserrat_14);
    }

    // Center chart + live stats (Gaggiuino-style: pressure + flow, no grid)
    const int chartW = wide ? centerW : 200;
    const int chartH = wide ? 230 : 140;
    s_chart = lv_chart_create(s_screen);
    lv_obj_set_size(s_chart, chartW, chartH);
    lv_obj_set_pos(s_chart, centerLeft + (centerW - chartW) / 2, wide ? 52 : 70);
    lv_chart_set_type(s_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(s_chart, wide ? 120 : 60);
    lv_chart_set_range(s_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 120);
    if (wide) {
        lv_chart_set_div_line_count(s_chart, 0, 0);
        lv_obj_set_style_line_width(s_chart, 3, LV_PART_ITEMS);
        lv_obj_set_style_line_opa(s_chart, LV_OPA_COVER, LV_PART_ITEMS);
    }
    lv_obj_set_style_bg_opa(s_chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_opa(s_chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_line_opa(s_chart, LV_OPA_TRANSP, LV_PART_TICKS);
    s_chartPressure = lv_chart_add_series(s_chart, lv_color_hex(0x2CA4F6), LV_CHART_AXIS_PRIMARY_Y);
    s_chartFlow = lv_chart_add_series(s_chart, lv_color_hex(0xF0A030), LV_CHART_AXIS_PRIMARY_Y);

    const int statsY = wide ? 292 : 230;
    if (wide) {
        const uint32_t pressColor = theme_colors[eez_flow_get_selected_theme_index()][7];
        // Full gap between tick columns; flex spreads the four live readouts.
        const int statsLeft = 168;
        const int statsW = 632 - 168;
        lv_obj_t *statsRow = lv_obj_create(s_screen);
        lv_obj_set_size(statsRow, statsW, 36);
        lv_obj_set_pos(statsRow, statsLeft, statsY);
        lv_obj_set_style_bg_opa(statsRow, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_border_width(statsRow, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(statsRow, 0, LV_PART_MAIN);
        lv_obj_clear_flag(statsRow, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_flex_flow(statsRow, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(statsRow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        auto makeStat = [&](const char *text, const lv_font_t *font) -> lv_obj_t * {
            lv_obj_t *label = lv_label_create(statsRow);
            lv_label_set_text(label, text);
            lv_obj_set_style_text_color(label, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
            if (font) {
                lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
            }
            return label;
        };
        s_elapsedLabel = makeStat("0:00", &lv_font_montserrat_20);
        s_flowLabel = makeStat("-- ml/s", &lv_font_montserrat_16);
        s_pressureLabel = makeStat("-- bar", &lv_font_montserrat_16);
        lv_obj_set_style_text_color(s_pressureLabel, lv_color_hex(pressColor), LV_PART_MAIN);
        s_weightLabel = makeStat("-- g", &lv_font_montserrat_16);
    } else {
        const int statsX = centerLeft + 20;
        s_elapsedLabel = makeLabel(s_screen, "0:00", statsX, statsY, &lv_font_montserrat_20);
        s_flowLabel = makeLabel(s_screen, "-- ml/s", statsX, statsY + 35, &lv_font_montserrat_16);
        s_weightLabel = makeLabel(s_screen, "-- g", statsX, statsY + 65, &lv_font_montserrat_16);
    }

    // Play under stats. Wide: sharp native 40px — shared pos with brew/water/grind.
    // Swipe-up back uses screen gesture (EEZ-style) filtered to play ±1cm.
    if (wide) {
        s_startBtn = lv_img_create(s_screen);
        lv_img_set_src(s_startBtn, &img_play_40x40);
        lv_obj_add_flag(s_startBtn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_ext_click_area(s_startBtn, 28);
        lv_obj_set_style_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_add_event_cb(s_startBtn, onToggle, LV_EVENT_CLICKED, nullptr);
        lv_obj_set_pos(s_startBtn, WideLayout::kPlayX, WideLayout::kPlayY);
        lv_obj_move_foreground(s_startBtn);

        lv_obj_add_event_cb(s_screen, onBack, LV_EVENT_GESTURE, nullptr);
    } else {
        s_startBtn = lv_btn_create(s_screen);
        lv_obj_set_size(s_startBtn, 64, 64);
        lv_obj_set_style_bg_opa(s_startBtn, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_shadow_width(s_startBtn, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(s_startBtn, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(s_startBtn, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_img_src(s_startBtn, &img_play_40x40, LV_PART_MAIN);
        lv_obj_set_style_bg_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
        lv_obj_set_style_bg_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_add_event_cb(s_startBtn, onToggle, LV_EVENT_CLICKED, nullptr);
        lv_obj_set_ext_click_area(s_startBtn, 20);
        lv_obj_align(s_startBtn, LV_ALIGN_BOTTOM_MID, 0, -72);
    }

    s_backBtn = nullptr;
    if (!wide) {
        s_backBtn = lv_img_create(s_screen);
        lv_img_set_src(s_backBtn, &img_angle_up_40x40);
        lv_img_set_zoom(s_backBtn, 150);
        lv_obj_add_flag(s_backBtn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_ext_click_area(s_backBtn, 20);
        lv_obj_set_style_img_recolor(s_backBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(s_backBtn, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_add_event_cb(s_backBtn, onBack, LV_EVENT_CLICKED, nullptr);
        lv_obj_align(s_backBtn, LV_ALIGN_BOTTOM_MID, 0, -16);
    }

    // EEZ-style prompt: theme card + icon actions (check / "x"), not filled text buttons.
    s_savePanel = lv_obj_create(s_screen);
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
    // ASCII "X" — Montserrat builds lack U+00D7 (×), which renders as a tofu rectangle.
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
    if (wide) {
        lv_label_set_text_fmt(s_tempLabel, "%d C", static_cast<int>(s_controller->getTargetTemp()));
        lv_label_set_text_fmt(s_targetPressureLabel, "%.1f bar", s_controller->getManualPressureTarget());
        if (s_pressureLabel) {
            lv_label_set_text_fmt(s_pressureLabel, "%.1f bar", s_controller->getCurrentPressure());
        }
    } else {
        lv_label_set_text_fmt(s_tempLabel, "%d C", static_cast<int>(s_controller->getTargetTemp()));
        lv_label_set_text_fmt(s_pressureLabel, "%.1f", s_controller->getCurrentPressure());
        lv_label_set_text_fmt(s_targetPressureLabel, "%.1f bar", s_controller->getManualPressureTarget());
    }
    updatePressureFill(s_controller->getManualPressureTarget());

    if (s_startBtn) {
        if (wide) {
            lv_img_set_src(s_startBtn, active ? &img_pause_40x40 : &img_play_40x40);
            lv_obj_set_style_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
        } else {
            lv_obj_set_style_bg_img_src(s_startBtn, active ? &img_pause_40x40 : &img_play_40x40, LV_PART_MAIN);
            lv_obj_set_style_bg_img_recolor(s_startBtn, lv_color_hex(0xE8E8E8), LV_PART_MAIN);
            lv_obj_set_style_bg_img_recolor_opa(s_startBtn, LV_OPA_COVER, LV_PART_MAIN);
        }
    }

    // Show save panel on trailing edge of a finished shot.
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
        if (now - s_lastChartMs >= (wide ? 250u : 1000u) && s_chart) {
            s_lastChartMs = now;
            lv_chart_set_next_value(s_chart, s_chartPressure, static_cast<lv_coord_t>(s_controller->getCurrentPressure() * 10.0f));
            lv_chart_set_next_value(s_chart, s_chartFlow, static_cast<lv_coord_t>(bp->currentFlow * 10.0f));
            recordPressurePoint(s_controller->getManualPressureTarget());
        }
    } else if (!active && !savePanelVisible()) {
        lv_label_set_text(s_elapsedLabel, "0:00");
        lv_label_set_text(s_flowLabel, "-- ml/s");
        if (wide && s_pressureLabel) {
            lv_label_set_text(s_pressureLabel, "-- bar");
        }
        lv_label_set_text(s_weightLabel, "-- g");
    }
}

} // namespace ManualBrewScreen
