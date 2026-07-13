#include "WideLayout.h"

#include <cmath>
#include <display/ui/default/eez/screens.h>
#include <display/ui/default/eez/ui.h>

namespace WideLayout {
namespace {

constexpr lv_coord_t kWideW = 800;
constexpr lv_coord_t kWideH = 480;
constexpr lv_coord_t kColW = 150;
constexpr lv_coord_t kColPadX = 4;
constexpr uint16_t kTickCount = 25;
constexpr int32_t kTickWidth = 10;
constexpr lv_coord_t kTickPadY = 4; // full-height ticks (tiny edge margin only)
constexpr size_t kMaxDialSets = 12;

// Readouts sit at ~1/4 and 3/4 — numbers (with EEZ units) in gutters outside center disc.
constexpr lv_coord_t kReadoutLeftX = 155;
constexpr lv_coord_t kReadoutRightX = 560;
constexpr lv_coord_t kReadoutTopY = 10;

struct Column {
    lv_obj_t *root = nullptr;
    lv_obj_t *canvas = nullptr;
    float fraction = 0.0f;
    uint32_t activeColor = 0;
    bool ticksFaceRight = true;
};

struct DialSet {
    lv_obj_t *root = nullptr;
    lv_obj_t *tempGauge = nullptr;
    lv_obj_t *tempGaugeFull = nullptr;
    lv_obj_t *pressureGauge = nullptr;
    lv_obj_t *tempIcon = nullptr;
    lv_obj_t *pressureIcon = nullptr;
    lv_obj_t *tempText = nullptr;
    lv_obj_t *pressureText = nullptr;
    lv_obj_t *tempTextFull = nullptr;
    Column temp;
    Column pressure;
    bool ticksOnly = false;
};

bool s_active = false;
DialSet s_dials[kMaxDialSets];
size_t s_dialCount = 0;
int32_t s_tickLength = kTickShort;
lv_obj_t *s_standaloneParent = nullptr;

static uint32_t themeTick() { return theme_colors[eez_flow_get_selected_theme_index()][5]; }
static uint32_t themeTemp() { return theme_colors[eez_flow_get_selected_theme_index()][6]; }
static uint32_t themePressure() { return theme_colors[eez_flow_get_selected_theme_index()][7]; }

static void expandScreen(lv_obj_t *screen) {
    if (!screen) {
        return;
    }
    lv_obj_set_size(screen, kWideW, kWideH);
}

static void hideMeter(lv_obj_t *meter) {
    if (!meter) {
        return;
    }
    lv_obj_add_flag(meter, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(meter, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(meter, 0, 0);
    lv_obj_set_pos(meter, -1000, -1000);
    lv_obj_set_style_opa(meter, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_opa(meter, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_set_style_opa(meter, LV_OPA_TRANSP, LV_PART_TICKS);
    lv_obj_set_style_opa(meter, LV_OPA_TRANSP, LV_PART_ITEMS);
}

static float clamp01(float v) {
    if (v < 0.0f) {
        return 0.0f;
    }
    if (v > 1.0f) {
        return 1.0f;
    }
    return v;
}

static void placeIconInColumn(lv_obj_t *obj, lv_obj_t *col, lv_coord_t y) {
    if (!obj || !col) {
        return;
    }
    lv_obj_set_parent(obj, col);
    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN);
    lv_obj_set_pos(obj, 0, y);
    lv_obj_move_foreground(obj);
}

// Host for floating labels: the LVGL screen, so center panels cannot cover them.
static lv_obj_t *labelHost(lv_obj_t *dialRoot) {
    if (!dialRoot) {
        return nullptr;
    }
    lv_obj_t *scr = lv_obj_get_screen(dialRoot);
    return scr ? scr : dialRoot;
}

// Numbers live on the screen at ~1/4 and 3/4 — inward of the tick columns.
static void placeReadoutInward(lv_obj_t *obj, lv_obj_t *host, bool left) {
    if (!obj || !host) {
        return;
    }
    lv_obj_set_parent(obj, host);
    lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_pos(obj, left ? kReadoutLeftX : kReadoutRightX, kReadoutTopY);
    lv_obj_move_foreground(obj);
}

static void colorReadouts(DialSet &set) {
    if (set.ticksOnly) {
        return;
    }
    if (set.tempText) {
        lv_obj_set_style_text_color(set.tempText, lv_color_hex(themeTemp()), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(set.tempText, lv_color_hex(themeTemp()), LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_move_foreground(set.tempText);
    }
    if (set.pressureText) {
        lv_obj_set_style_text_color(set.pressureText, lv_color_hex(themePressure()), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(set.pressureText, lv_color_hex(themePressure()), LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_move_foreground(set.pressureText);
    }
}

static void hideBackIcon(lv_obj_t *obj) {
    if (!obj) {
        return;
    }
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(obj, 0, 0);
    lv_obj_set_pos(obj, -1000, -1000);
    lv_obj_set_style_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_img_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
}

static void placeWidePlay(lv_obj_t *btn, lv_obj_t *screen) {
    if (!btn || !screen) {
        return;
    }
    lv_obj_set_parent(btn, screen);
    lv_obj_set_style_align(btn, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(btn, kPlaySize, kPlaySize);
    lv_obj_set_pos(btn, kPlayX, kPlayY);
    lv_obj_set_ext_click_area(btn, 28);
    lv_obj_move_foreground(btn);
}

static void placeAllWidePlay() {
    placeWidePlay(objects.start_button, objects.brew_screen);
    placeWidePlay(objects.water_start_button, objects.water_screen);
    placeWidePlay(objects.grind_start_button, objects.grind_screen);
}

static void hideAllBackIcons() {
    hideBackIcon(objects.brew_dials__menu_icon);
    hideBackIcon(objects.status_dials__menu_icon);
    hideBackIcon(objects.menu_dials__menu_icon);
    hideBackIcon(objects.new_menu_dials__menu_icon);
    hideBackIcon(objects.steam_dials__menu_icon);
    hideBackIcon(objects.water_dials__menu_icon);
    hideBackIcon(objects.profile_dials__menu_icon);
    hideBackIcon(objects.grind_dials__menu_icon);
    hideBackIcon(objects.obj0__menu_icon);
    hideBackIcon(objects.info_menu_icon);
}

static void onColumnDraw(lv_event_t *e) {
    if (lv_event_get_code(e) != LV_EVENT_DRAW_MAIN) {
        return;
    }
    auto *col = static_cast<Column *>(lv_event_get_user_data(e));
    if (!col || !col->canvas) {
        return;
    }

    lv_draw_ctx_t *draw_ctx = lv_event_get_draw_ctx(e);
    if (!draw_ctx) {
        return;
    }

    lv_area_t content;
    lv_obj_get_content_coords(col->canvas, &content);
    const lv_coord_t w = lv_area_get_width(&content);
    const lv_coord_t h = lv_area_get_height(&content);
    if (w <= 0 || h <= 0 || kTickCount < 2) {
        return;
    }

    const int32_t tickLen = s_tickLength > 0 ? s_tickLength : 1;
    const lv_coord_t cap = kTickWidth / 2;
    const lv_coord_t usableH = h - kTickPadY * 2;
    if (usableH <= 0) {
        return;
    }

    const uint16_t litCount =
        static_cast<uint16_t>(lroundf(clamp01(col->fraction) * static_cast<float>(kTickCount - 1))) + 1;

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.width = kTickWidth;
    line_dsc.round_start = 1;
    line_dsc.round_end = 1;
    line_dsc.raw_end = 0;
    line_dsc.opa = LV_OPA_COVER;

    const bool pill = tickLen > kTickWidth;
    lv_draw_rect_dsc_t dot_dsc;
    lv_draw_rect_dsc_init(&dot_dsc);
    dot_dsc.radius = LV_RADIUS_CIRCLE;
    dot_dsc.bg_opa = LV_OPA_COVER;
    const lv_coord_t ri = tickLen / 2;

    const int32_t maxReach = static_cast<int32_t>(w) - 8;
    const int32_t drawLen = tickLen > maxReach ? maxReach : tickLen;

    for (uint16_t i = 0; i < kTickCount; i++) {
        const float t = static_cast<float>(i) / static_cast<float>(kTickCount - 1);
        const lv_coord_t y =
            content.y2 - kTickPadY - static_cast<lv_coord_t>(lroundf(t * static_cast<float>(usableH)));

        const bool lit = i < litCount;
        const lv_color_t color = lv_color_hex(lit ? col->activeColor : themeTick());

        if (pill) {
            lv_point_t a;
            lv_point_t b;
            if (col->ticksFaceRight) {
                a = {static_cast<lv_coord_t>(content.x1 + 4 + cap), y};
                b = {static_cast<lv_coord_t>(content.x1 + 4 + drawLen - cap), y};
            } else {
                a = {static_cast<lv_coord_t>(content.x2 - 4 - cap), y};
                b = {static_cast<lv_coord_t>(content.x2 - 4 - drawLen + cap), y};
            }
            line_dsc.color = color;
            lv_draw_line(draw_ctx, &line_dsc, &a, &b);
        } else {
            lv_coord_t cx;
            if (col->ticksFaceRight) {
                cx = content.x1 + 4 + ri;
            } else {
                cx = content.x2 - 4 - ri;
            }
            dot_dsc.bg_color = color;
            lv_area_t area = {static_cast<lv_coord_t>(cx - ri), static_cast<lv_coord_t>(y - ri),
                              static_cast<lv_coord_t>(cx + ri), static_cast<lv_coord_t>(y + ri)};
            lv_draw_rect(draw_ctx, &dot_dsc, &area);
        }
    }
}

static void invalidateColumns() {
    for (size_t i = 0; i < s_dialCount; i++) {
        if (s_dials[i].temp.canvas) {
            lv_obj_invalidate(s_dials[i].temp.canvas);
        }
        if (s_dials[i].pressure.canvas) {
            lv_obj_invalidate(s_dials[i].pressure.canvas);
        }
    }
}

static void buildColumn(Column &col, lv_obj_t *parent, bool left, uint32_t activeColor) {
    col.root = lv_obj_create(parent);
    lv_obj_set_size(col.root, kColW, kWideH);
    lv_obj_set_style_bg_opa(col.root, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(col.root, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(col.root, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(col.root, 0, LV_PART_MAIN);
    lv_obj_clear_flag(col.root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(col.root, LV_OBJ_FLAG_CLICKABLE);
    if (left) {
        lv_obj_align(col.root, LV_ALIGN_LEFT_MID, kColPadX, 0);
    } else {
        lv_obj_align(col.root, LV_ALIGN_RIGHT_MID, -kColPadX, 0);
    }

    col.canvas = lv_obj_create(col.root);
    lv_obj_set_size(col.canvas, kColW, kWideH);
    lv_obj_set_pos(col.canvas, 0, 0);
    lv_obj_set_style_bg_opa(col.canvas, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(col.canvas, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(col.canvas, 0, LV_PART_MAIN);
    lv_obj_set_style_opa(col.canvas, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(col.canvas, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    col.activeColor = activeColor;
    col.ticksFaceRight = left;
    col.fraction = 0.0f;
    lv_obj_add_event_cb(col.canvas, onColumnDraw, LV_EVENT_DRAW_MAIN, &col);
}

static void adaptDialSet(DialSet &set) {
    if (!set.root) {
        return;
    }

    lv_obj_set_size(set.root, kWideW, kWideH);
    lv_obj_set_pos(set.root, 0, 0);
    lv_obj_set_style_align(set.root, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_align(set.root, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_CHECKED);

    hideMeter(set.tempGauge);
    hideMeter(set.tempGaugeFull);
    hideMeter(set.pressureGauge);

    buildColumn(set.temp, set.root, true, themeTemp());
    buildColumn(set.pressure, set.root, false, themePressure());

    // Inward readouts — color the EEZ text itself (includes °C / bar).
    lv_obj_t *host = labelHost(set.root);
    placeReadoutInward(set.tempText, host, true);
    placeReadoutInward(set.pressureText, host, false);
    colorReadouts(set);

    if (set.tempTextFull) {
        lv_obj_add_flag(set.tempTextFull, LV_OBJ_FLAG_HIDDEN);
    }

    // Icons at bottom edge of outer columns (EEZ zoom 150).
    if (set.tempIcon) {
        lv_img_set_zoom(set.tempIcon, 150);
    }
    if (set.pressureIcon) {
        lv_img_set_zoom(set.pressureIcon, 150);
    }
    placeIconInColumn(set.tempIcon, set.temp.root, kWideH - 52);
    placeIconInColumn(set.pressureIcon, set.pressure.root, kWideH - 52);

    lv_obj_move_background(set.temp.root);
    lv_obj_move_background(set.pressure.root);
}

static void registerDialSet(lv_obj_t *root, lv_obj_t *tempGauge, lv_obj_t *tempGaugeFull, lv_obj_t *pressureGauge,
                            lv_obj_t *tempIcon, lv_obj_t *pressureIcon, lv_obj_t *tempText, lv_obj_t *pressureText,
                            lv_obj_t *tempTextFull) {
    if (s_dialCount >= kMaxDialSets || !root) {
        return;
    }
    DialSet &set = s_dials[s_dialCount++];
    set = {};
    set.root = root;
    set.tempGauge = tempGauge;
    set.tempGaugeFull = tempGaugeFull;
    set.pressureGauge = pressureGauge;
    set.tempIcon = tempIcon;
    set.pressureIcon = pressureIcon;
    set.tempText = tempText;
    set.pressureText = pressureText;
    set.tempTextFull = tempTextFull;
    adaptDialSet(set);
}

static void updateFills(float tempFraction, float pressureFraction) {
    for (size_t i = 0; i < s_dialCount; i++) {
        hideMeter(s_dials[i].tempGauge);
        hideMeter(s_dials[i].tempGaugeFull);
        hideMeter(s_dials[i].pressureGauge);
        s_dials[i].temp.fraction = clamp01(tempFraction);
        s_dials[i].pressure.fraction = clamp01(pressureFraction);
        s_dials[i].temp.activeColor = themeTemp();
        s_dials[i].pressure.activeColor = themePressure();
        if (s_dials[i].temp.canvas) {
            lv_obj_invalidate(s_dials[i].temp.canvas);
        }
        if (s_dials[i].pressure.canvas) {
            lv_obj_invalidate(s_dials[i].pressure.canvas);
        }
    }
}

static void tickAnimCb(void * /*var*/, int32_t v) {
    s_tickLength = v;
    invalidateColumns();
}

} // namespace

bool isActive() { return s_active; }

void apply() {
    lv_disp_t *disp = lv_disp_get_default();
    if (!disp || lv_disp_get_hor_res(disp) <= 481) {
        s_active = false;
        return;
    }
    s_active = true;
    s_dialCount = 0;
    s_standaloneParent = nullptr;
    s_tickLength = kTickShort;

    expandScreen(objects.standby_screen);
    expandScreen(objects.brew_screen);
    expandScreen(objects.status_screen);
    expandScreen(objects.menu_screen);
    expandScreen(objects.menu_screen_new);
    expandScreen(objects.steam_screen);
    expandScreen(objects.water_screen);
    expandScreen(objects.profile_screen);
    expandScreen(objects.grind_screen);
    expandScreen(objects.info_screen);
    expandScreen(objects.new_profile_screen);

    registerDialSet(objects.new_menu_dials, objects.new_menu_dials__temp_gauge, objects.new_menu_dials__temp_gauge_full,
                    objects.new_menu_dials__pressure_gauge, objects.new_menu_dials__temp_icon,
                    objects.new_menu_dials__pressure_icon, objects.new_menu_dials__temp_text,
                    objects.new_menu_dials__pressure_text, objects.new_menu_dials__temp_text_full);

    registerDialSet(objects.brew_dials, objects.brew_dials__temp_gauge, objects.brew_dials__temp_gauge_full,
                    objects.brew_dials__pressure_gauge, objects.brew_dials__temp_icon, objects.brew_dials__pressure_icon,
                    objects.brew_dials__temp_text, objects.brew_dials__pressure_text, objects.brew_dials__temp_text_full);

    registerDialSet(objects.status_dials, objects.status_dials__temp_gauge, objects.status_dials__temp_gauge_full,
                    objects.status_dials__pressure_gauge, objects.status_dials__temp_icon, objects.status_dials__pressure_icon,
                    objects.status_dials__temp_text, objects.status_dials__pressure_text, objects.status_dials__temp_text_full);

    registerDialSet(objects.menu_dials, objects.menu_dials__temp_gauge, objects.menu_dials__temp_gauge_full,
                    objects.menu_dials__pressure_gauge, objects.menu_dials__temp_icon, objects.menu_dials__pressure_icon,
                    objects.menu_dials__temp_text, objects.menu_dials__pressure_text, objects.menu_dials__temp_text_full);

    registerDialSet(objects.steam_dials, objects.steam_dials__temp_gauge, objects.steam_dials__temp_gauge_full,
                    objects.steam_dials__pressure_gauge, objects.steam_dials__temp_icon, objects.steam_dials__pressure_icon,
                    objects.steam_dials__temp_text, objects.steam_dials__pressure_text, objects.steam_dials__temp_text_full);

    registerDialSet(objects.water_dials, objects.water_dials__temp_gauge, objects.water_dials__temp_gauge_full,
                    objects.water_dials__pressure_gauge, objects.water_dials__temp_icon, objects.water_dials__pressure_icon,
                    objects.water_dials__temp_text, objects.water_dials__pressure_text, objects.water_dials__temp_text_full);

    registerDialSet(objects.profile_dials, objects.profile_dials__temp_gauge, objects.profile_dials__temp_gauge_full,
                    objects.profile_dials__pressure_gauge, objects.profile_dials__temp_icon, objects.profile_dials__pressure_icon,
                    objects.profile_dials__temp_text, objects.profile_dials__pressure_text, objects.profile_dials__temp_text_full);

    registerDialSet(objects.grind_dials, objects.grind_dials__temp_gauge, objects.grind_dials__temp_gauge_full,
                    objects.grind_dials__pressure_gauge, objects.grind_dials__temp_icon, objects.grind_dials__pressure_icon,
                    objects.grind_dials__temp_text, objects.grind_dials__pressure_text, objects.grind_dials__temp_text_full);

    registerDialSet(objects.obj0, objects.obj0__temp_gauge, objects.obj0__temp_gauge_full, objects.obj0__pressure_gauge,
                    objects.obj0__temp_icon, objects.obj0__pressure_icon, objects.obj0__temp_text, objects.obj0__pressure_text,
                    objects.obj0__temp_text_full);

    // Wide: swipe-up exits; hard-kill angle-up (EEZ tick re-shows soft hide).
    hideAllBackIcons();
    placeAllWidePlay();

    if (objects.menu_screen_new) {
        if (objects.status_icons) {
            lv_obj_set_pos(objects.status_icons, 0, -200);
        }
        if (objects.standby_btn) {
            lv_obj_set_pos(objects.standby_btn, 0, 200);
        }
        if (objects.info_btn) {
            lv_obj_set_pos(objects.info_btn, 0, 20);
        }
    }
}

void update(float tempFraction, float pressureFraction) {
    if (!s_active) {
        return;
    }
    updateFills(tempFraction, pressureFraction);
    // EEZ tick can re-show / re-layout; keep back icons dead and play locked.
    hideAllBackIcons();
    placeAllWidePlay();

    for (size_t i = 0; i < s_dialCount; i++) {
        colorReadouts(s_dials[i]);
    }
}

void setTickLength(int32_t len) {
    if (!s_active) {
        return;
    }
    s_tickLength = len;
    invalidateColumns();
}

void animateTickLength(int32_t fromLen, int32_t toLen, uint32_t durationMs) {
    if (!s_active) {
        return;
    }
    static int s_animToken = 0;
    lv_anim_del(&s_animToken, tickAnimCb);
    setTickLength(fromLen);
    if (fromLen == toLen) {
        return;
    }
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, &s_animToken);
    lv_anim_set_exec_cb(&a, tickAnimCb);
    lv_anim_set_values(&a, fromLen, toLen);
    lv_anim_set_time(&a, durationMs);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

void addStandaloneTicks(lv_obj_t *parent) {
    if (!s_active || !parent || s_dialCount >= kMaxDialSets) {
        return;
    }
    if (s_standaloneParent == parent) {
        return;
    }
    s_standaloneParent = parent;
    DialSet &set = s_dials[s_dialCount++];
    set = {};
    set.root = parent;
    set.ticksOnly = true;
    buildColumn(set.temp, parent, true, themeTemp());
    buildColumn(set.pressure, parent, false, themePressure());
    lv_obj_move_background(set.temp.root);
    lv_obj_move_background(set.pressure.root);
}

} // namespace WideLayout
