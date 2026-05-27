// GaggiMate — 4.3" rectangular display sidebar dials component
// Sidebar container: 180×480 px, dark background, right separator border.
//
// Component child indices are IDENTICAL to the original ui_dials_create() so that
// DefaultUI.cpp's adjustDials() / adjustHeatingIndicator() work without changes.
// TEMPGAUGE and PRESSUREGAUGE are lv_bar widgets instead of lv_arc.
// DefaultUI.cpp must use gauge_set_value() / gauge_set_range() helpers
// (see DefaultUI.cpp) instead of direct lv_arc_set_value calls.

#include "../ui.h"

// ─── layout constants ───────────────────────────────────────────────────────
// All positions expressed as offsets from the container centre (90, 240) unless noted.
// Container is 180 × 480 → centre = (90, 240).
//   adjustDials() repositions tempText to (-50, -205) → absolute (40, 35).
//   So we match that here as the initial position.

lv_obj_t *ui_dials43_create(lv_obj_t *comp_parent) {

    // ── root container (sidebar) ──────────────────────────────────────────
    lv_obj_t *cui_dials;
    cui_dials = lv_obj_create(comp_parent);
    lv_obj_remove_style_all(cui_dials);
    lv_obj_set_size(cui_dials, 180, 480);
    lv_obj_set_pos(cui_dials, 0, 0);
    lv_obj_clear_flag(cui_dials, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(cui_dials, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_dials,   255,                     LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(cui_dials,  LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_dials, 2,                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_dials, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_dials,   255,                 LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(cui_dials, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(cui_dials, false, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── TEMP section header "TEMP" ────────────────────────────────────────
    lv_obj_t *cui_tempHeader;
    cui_tempHeader = lv_label_create(cui_dials);
    lv_label_set_text(cui_tempHeader, "TEMP");
    lv_obj_set_align(cui_tempHeader, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_tempHeader, 0);
    lv_obj_set_y(cui_tempHeader, -218);   // → absolute y ≈ 22
    lv_obj_set_style_text_font(cui_tempHeader, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(cui_tempHeader, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_tempHeader, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(cui_tempHeader, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(cui_tempHeader, LV_OBJ_FLAG_CLICKABLE);

    // ── [6] TEMPTEXT  "92°C" ──────────────────────────────────────────────
    // adjustDials() will call lv_obj_set_x(-50) / lv_obj_set_y(-205) which
    // for a 180×480 container → absolute (40, 35). Match that initial position.
    lv_obj_t *cui_tempText;
    cui_tempText = lv_label_create(cui_dials);
    lv_obj_set_width(cui_tempText,  LV_SIZE_CONTENT);
    lv_obj_set_height(cui_tempText, LV_SIZE_CONTENT);
    lv_obj_set_align(cui_tempText, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_tempText, -50);
    lv_obj_set_y(cui_tempText, -205);
    lv_label_set_text(cui_tempText, "92°C");
    lv_obj_set_style_text_font(cui_tempText, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(cui_tempText, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(cui_tempText, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);
    ui_object_set_themeable_style_property(cui_tempText, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,   _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(cui_tempText, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,     _ui_theme_alpha_Dark);

    // ── [1] TEMPGAUGE  (lv_bar, horizontal, full width) ──────────────────
    // Range 0-160 initially; adjustDials leaves range unchanged for temp.
    // DefaultUI effects call gauge_set_value(gauge, currentTemp).
    lv_obj_t *cui_tempGauge;
    cui_tempGauge = lv_bar_create(cui_dials);
    lv_bar_set_range(cui_tempGauge, 0, 160);
    lv_bar_set_value(cui_tempGauge, 80, LV_ANIM_OFF);
    lv_obj_set_size(cui_tempGauge, 150, 14);
    lv_obj_set_align(cui_tempGauge, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_tempGauge, 0);
    lv_obj_set_y(cui_tempGauge, -168);   // → absolute y ≈ 72
    lv_obj_clear_flag(cui_tempGauge, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(cui_tempGauge,  lv_color_hex(0x2A2A2A), LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_tempGauge,    255,                     LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_tempGauge,  lv_color_hex(0xF62C2C), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_tempGauge,    255,                     LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_tempGauge, 3, LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_tempGauge, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // ── [7] TEMPICON  (heating indicator, recolored red/green) ───────────
    lv_obj_t *cui_tempIcon;
    cui_tempIcon = lv_img_create(cui_dials);
    lv_img_set_src(cui_tempIcon, &ui_img_1951499226);
    lv_obj_set_width(cui_tempIcon,  LV_SIZE_CONTENT);
    lv_obj_set_height(cui_tempIcon, LV_SIZE_CONTENT);
    lv_obj_set_align(cui_tempIcon, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_tempIcon, -55);
    lv_obj_set_y(cui_tempIcon, -145);   // → absolute y ≈ 95
    lv_obj_add_flag(cui_tempIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(cui_tempIcon, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(cui_tempIcon, 150);
    lv_obj_set_style_img_recolor(    cui_tempIcon, lv_color_hex(0xF62C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(cui_tempIcon, 255,                    LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── [2] TEMPTARGET  (dummy image, invisible; keeps lv_img_set_angle safe) ──
    lv_obj_t *cui_tempTarget;
    cui_tempTarget = lv_img_create(cui_dials);
    lv_obj_set_size(cui_tempTarget, 1, 1);
    lv_obj_set_align(cui_tempTarget, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_tempTarget, LV_OBJ_FLAG_HIDDEN);

    // ── centre divider ────────────────────────────────────────────────────
    lv_obj_t *cui_divider;
    cui_divider = lv_obj_create(cui_dials);
    lv_obj_remove_style_all(cui_divider);
    lv_obj_set_size(cui_divider, 160, 1);
    lv_obj_set_align(cui_divider, LV_ALIGN_CENTER);
    lv_obj_set_y(cui_divider, -10);      // slightly above exact centre
    lv_obj_clear_flag(cui_divider, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(cui_divider, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_divider, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── PRESSURE section header "BAR" ─────────────────────────────────────
    lv_obj_t *cui_pressureHeader;
    cui_pressureHeader = lv_label_create(cui_dials);
    lv_label_set_text(cui_pressureHeader, "BAR");
    lv_obj_set_align(cui_pressureHeader, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_pressureHeader, 0);
    lv_obj_set_y(cui_pressureHeader, 10);   // → absolute y ≈ 250
    lv_obj_set_style_text_font(cui_pressureHeader, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(cui_pressureHeader, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_pressureHeader,   255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(cui_pressureHeader, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(cui_pressureHeader, LV_OBJ_FLAG_HIDDEN);  // shown by adjustDials
    lv_obj_clear_flag(cui_pressureHeader, LV_OBJ_FLAG_CLICKABLE);

    // ── [5] PRESSURETEXT  "9 bar" ─────────────────────────────────────────
    // adjustDials() only repositions tempText, not pressureText, so this stays put.
    lv_obj_t *cui_pressureText;
    cui_pressureText = lv_label_create(cui_dials);
    lv_obj_set_width(cui_pressureText,  LV_SIZE_CONTENT);
    lv_obj_set_height(cui_pressureText, LV_SIZE_CONTENT);
    lv_obj_set_align(cui_pressureText, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_pressureText, -50);
    lv_obj_set_y(cui_pressureText,  40);   // → absolute y ≈ 280
    lv_label_set_text(cui_pressureText, "9 bar");
    lv_obj_set_style_text_font(cui_pressureText, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(cui_pressureText, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(cui_pressureText, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_pad_left(cui_pressureText,   6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_pressureText,  6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(cui_pressureText, LV_OBJ_FLAG_HIDDEN);  // initially hidden, shown by adjustDials

    // ── [3] PRESSUREGAUGE  (lv_bar, horizontal) ───────────────────────────
    // Range starts at 0-16 matching original arc; adjustDials calls
    // gauge_set_range(pressureGauge, 0, pressureScaling*10) to widen it.
    lv_obj_t *cui_pressureGauge;
    cui_pressureGauge = lv_bar_create(cui_dials);
    lv_bar_set_range(cui_pressureGauge, 0, 16);
    lv_bar_set_value(cui_pressureGauge, 0, LV_ANIM_OFF);
    lv_obj_set_size(cui_pressureGauge, 150, 14);
    lv_obj_set_align(cui_pressureGauge, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_pressureGauge, 0);
    lv_obj_set_y(cui_pressureGauge, 72);   // → absolute y ≈ 312
    lv_obj_clear_flag(cui_pressureGauge, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(cui_pressureGauge,  lv_color_hex(0x2A2A2A), LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_pressureGauge,    255,                     LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_pressureGauge,  lv_color_hex(0x2CA4F6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_pressureGauge,    255,                     LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_pressureGauge, 3, LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_pressureGauge, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_flag(cui_pressureGauge, LV_OBJ_FLAG_HIDDEN);  // shown by adjustDials

    // ── [8] IMAGE6  (pressure / water icon) ──────────────────────────────
    lv_obj_t *cui_Image6;
    cui_Image6 = lv_img_create(cui_dials);
    lv_img_set_src(cui_Image6, &ui_img_1220767159);
    lv_obj_set_width(cui_Image6,  LV_SIZE_CONTENT);
    lv_obj_set_height(cui_Image6, LV_SIZE_CONTENT);
    lv_obj_set_align(cui_Image6, LV_ALIGN_CENTER);
    lv_obj_set_x(cui_Image6, 55);
    lv_obj_set_y(cui_Image6, 40);   // alongside pressureText
    lv_obj_add_flag(cui_Image6, LV_OBJ_FLAG_ADV_HITTEST | LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(cui_Image6, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(cui_Image6, 150);
    lv_obj_set_style_img_recolor(    cui_Image6, lv_color_hex(0x2CA4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(cui_Image6, 255,                    LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── [4] PRESSURETARGET  (dummy image, invisible) ─────────────────────
    lv_obj_t *cui_pressureTarget;
    cui_pressureTarget = lv_img_create(cui_dials);
    lv_obj_set_size(cui_pressureTarget, 1, 1);
    lv_obj_set_align(cui_pressureTarget, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_pressureTarget, LV_OBJ_FLAG_HIDDEN);

    // ── register children at canonical indices ────────────────────────────
    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_DIALS_NUM);
    children[UI_COMP_DIALS_DIALS]          = cui_dials;
    children[UI_COMP_DIALS_TEMPGAUGE]      = cui_tempGauge;
    children[UI_COMP_DIALS_TEMPTARGET]     = cui_tempTarget;
    children[UI_COMP_DIALS_PRESSUREGAUGE]  = cui_pressureGauge;
    children[UI_COMP_DIALS_PRESSURETARGET] = cui_pressureTarget;
    children[UI_COMP_DIALS_PRESSURETEXT]   = cui_pressureText;
    children[UI_COMP_DIALS_TEMPTEXT]       = cui_tempText;
    children[UI_COMP_DIALS_TEMPICON]       = cui_tempIcon;
    children[UI_COMP_DIALS_IMAGE6]         = cui_Image6;
    lv_obj_add_event_cb(cui_dials, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_dials, del_component_child_event_cb, LV_EVENT_DELETE, children);
    return cui_dials;
}
