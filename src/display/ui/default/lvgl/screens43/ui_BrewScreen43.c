// GaggiMate — 4.3" Brew Screen (800×480)
// Black background, decorative circles, metric pills, centered controls.
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

// Module-level globals — exposed so DefaultUI.cpp can update the circle readouts
// independently of the sidebar dials labels.
lv_obj_t *ui_BrewScreen_circleTempLabel     = NULL;
lv_obj_t *ui_BrewScreen_circlePressureLabel = NULL;

void ui_BrewScreen_screen_init_43(void) {
    ui_BrewScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_BrewScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_BrewScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_BrewScreen_screen_destroy);
    lv_obj_set_style_bg_color(ui_BrewScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BrewScreen, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── Sidebar bars (full-screen overlay, non-interactive) ──────────────────
    ui_BrewScreen_dials = ui_dials43_create(ui_BrewScreen);

    // ── Back button (imgbtn, styled by apply_brew()) ──────────────────────────
    ui_BrewScreen_ImgButton5 = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_ImgButton5, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_BrewScreen_ImgButton5, 40, 40);
    lv_obj_set_align(ui_BrewScreen_ImgButton5, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_BrewScreen_ImgButton5, 0, 14);
    ui_object_set_themeable_style_property(ui_BrewScreen_ImgButton5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_ImgButton5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Standalone circle value labels (direct children of ui_BrewScreen) ─────
    // These are NOT children of the dials component, so they are never clipped.
    lv_obj_t *brewCircleTempLabel;
    lv_obj_t *brewCirclePressureLabel;

    // ── Temp decorative circle (pos 110,43 — center 200,133) ─────────────────
    {
        lv_obj_t *outer = lv_obj_create(ui_BrewScreen);
        lv_obj_remove_style_all(outer);
        lv_obj_set_size(outer, 180, 180);
        lv_obj_set_pos(outer, 110, 43);
        lv_obj_clear_flag(outer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(outer, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(outer, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(outer, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(outer, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(outer, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t *inner = lv_obj_create(ui_BrewScreen);
        lv_obj_remove_style_all(inner);
        lv_obj_set_size(inner, 148, 148);
        lv_obj_set_pos(inner, 126, 59);
        lv_obj_clear_flag(inner, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(inner, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(inner, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(inner, lv_color_hex(0xCC2222), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(inner, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(inner, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    }

    // Temp circle value label — standalone, visible in circle center
    brewCircleTempLabel = lv_label_create(ui_BrewScreen);
    lv_obj_set_pos(brewCircleTempLabel, 150, 116);
    lv_obj_set_size(brewCircleTempLabel, 100, LV_SIZE_CONTENT);
    lv_label_set_text(brewCircleTempLabel, "--");
    lv_obj_set_style_text_font(brewCircleTempLabel, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(brewCircleTempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(brewCircleTempLabel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(brewCircleTempLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── Pressure decorative circle (pos 510,43 — center 600,133) ─────────────
    {
        lv_obj_t *outer = lv_obj_create(ui_BrewScreen);
        lv_obj_remove_style_all(outer);
        lv_obj_set_size(outer, 180, 180);
        lv_obj_set_pos(outer, 510, 43);
        lv_obj_clear_flag(outer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(outer, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(outer, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(outer, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(outer, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(outer, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t *inner = lv_obj_create(ui_BrewScreen);
        lv_obj_remove_style_all(inner);
        lv_obj_set_size(inner, 148, 148);
        lv_obj_set_pos(inner, 526, 59);
        lv_obj_clear_flag(inner, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(inner, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(inner, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(inner, lv_color_hex(0x2266CC), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(inner, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(inner, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        // No "pressure" caption — removed per issue 3
    }

    // Pressure circle value label — standalone, visible in circle center
    brewCirclePressureLabel = lv_label_create(ui_BrewScreen);
    lv_obj_set_pos(brewCirclePressureLabel, 550, 106);
    lv_obj_set_size(brewCirclePressureLabel, 100, LV_SIZE_CONTENT);
    lv_label_set_text(brewCirclePressureLabel, "--");
    lv_obj_set_style_text_font(brewCirclePressureLabel, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(brewCirclePressureLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(brewCirclePressureLabel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(brewCirclePressureLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── Profile info (always visible on 43": adaptive pill + hamburger | name | gear) ──
    ui_BrewScreen_profileInfo = lv_obj_create(ui_BrewScreen);
    lv_obj_remove_style_all(ui_BrewScreen_profileInfo);
    lv_obj_set_size(ui_BrewScreen_profileInfo, 560, 140);
    lv_obj_set_align(ui_BrewScreen_profileInfo, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_BrewScreen_profileInfo, 0, 155);
    lv_obj_clear_flag(ui_BrewScreen_profileInfo, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(ui_BrewScreen_profileInfo, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    // NOTE: do NOT add LV_OBJ_FLAG_HIDDEN here — DefaultUI controls visibility via effect_mgr

    // Adaptive pill button (outline/shape set here; ON/OFF opa toggled by DefaultUI)
    ui_BrewScreen_Label1 = lv_label_create(ui_BrewScreen_profileInfo);
    lv_obj_set_size(ui_BrewScreen_Label1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_BrewScreen_Label1, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_BrewScreen_Label1, 0, 4);
    lv_label_set_text(ui_BrewScreen_Label1, "Adaptive OFF");
    lv_obj_set_style_text_font(ui_BrewScreen_Label1, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_Label1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_Label1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);
    lv_obj_add_flag(ui_BrewScreen_Label1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(ui_BrewScreen_Label1, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_BrewScreen_Label1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_BrewScreen_Label1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_BrewScreen_Label1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_BrewScreen_Label1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BrewScreen_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BrewScreen_Label1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BrewScreen_Label1, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BrewScreen_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BrewScreen_Label1, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BrewScreen_Label1, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_ext_click_area(ui_BrewScreen_Label1, 12);

    // Profile row: hamburger | name | gear
    ui_BrewScreen_Container3 = lv_obj_create(ui_BrewScreen_profileInfo);
    lv_obj_remove_style_all(ui_BrewScreen_Container3);
    lv_obj_set_size(ui_BrewScreen_Container3, 560, 44);
    lv_obj_set_align(ui_BrewScreen_Container3, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_BrewScreen_Container3, 0, -4);
    lv_obj_set_flex_flow(ui_BrewScreen_Container3, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_BrewScreen_Container3, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_BrewScreen_Container3, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_column(ui_BrewScreen_Container3, 16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BrewScreen_profileSelectBtn = lv_imgbtn_create(ui_BrewScreen_Container3);
    lv_imgbtn_set_src(ui_BrewScreen_profileSelectBtn, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_332059803, NULL);
    lv_obj_set_size(ui_BrewScreen_profileSelectBtn, LV_SIZE_CONTENT, 44);
    ui_object_set_themeable_style_property(ui_BrewScreen_profileSelectBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_profileSelectBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_profileName = lv_label_create(ui_BrewScreen_Container3);
    lv_obj_set_size(ui_BrewScreen_profileName, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(ui_BrewScreen_profileName, 1);
    lv_label_set_long_mode(ui_BrewScreen_profileName, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_BrewScreen_profileName, "Cremina Lever");
    lv_obj_set_style_text_font(ui_BrewScreen_profileName, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_BrewScreen_profileName, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);
    lv_obj_add_flag(ui_BrewScreen_profileName, LV_OBJ_FLAG_CLICKABLE);

    ui_BrewScreen_settingsButton = lv_imgbtn_create(ui_BrewScreen_Container3);
    lv_imgbtn_set_src(ui_BrewScreen_settingsButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_340148213, NULL);
    lv_obj_set_size(ui_BrewScreen_settingsButton, LV_SIZE_CONTENT, 44);
    ui_object_set_themeable_style_property(ui_BrewScreen_settingsButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_settingsButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Adjustments (shown in Brew mode, hidden in Settings mode) ─────────────
    ui_BrewScreen_adjustments = lv_obj_create(ui_BrewScreen);
    lv_obj_remove_style_all(ui_BrewScreen_adjustments);
    lv_obj_set_size(ui_BrewScreen_adjustments, 560, 115);
    lv_obj_set_align(ui_BrewScreen_adjustments, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_BrewScreen_adjustments, 0, 155);
    lv_obj_set_flex_flow(ui_BrewScreen_adjustments, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_BrewScreen_adjustments, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_BrewScreen_adjustments, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    // Temperature row
    ui_BrewScreen_tempContainer = lv_obj_create(ui_BrewScreen_adjustments);
    lv_obj_remove_style_all(ui_BrewScreen_tempContainer);
    lv_obj_set_size(ui_BrewScreen_tempContainer, 560, 50);
    lv_obj_clear_flag(ui_BrewScreen_tempContainer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_BrewScreen_tempContainer, LV_OBJ_FLAG_CLICKABLE);

    ui_BrewScreen_Image5 = lv_img_create(ui_BrewScreen_tempContainer);
    lv_img_set_src(ui_BrewScreen_Image5, &ui_img_1951499226);
    lv_obj_set_size(ui_BrewScreen_Image5, 40, 40);
    lv_obj_set_align(ui_BrewScreen_Image5, LV_ALIGN_LEFT_MID);
    lv_obj_set_pos(ui_BrewScreen_Image5, 0, 0);
    lv_obj_add_flag(ui_BrewScreen_Image5, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_BrewScreen_Image5, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_BrewScreen_Image5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_Image5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_downTempButton = lv_imgbtn_create(ui_BrewScreen_tempContainer);
    lv_imgbtn_set_src(ui_BrewScreen_downTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_BrewScreen_downTempButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_downTempButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_BrewScreen_downTempButton, -80, 0);
    ui_object_set_themeable_style_property(ui_BrewScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_targetTemp = lv_label_create(ui_BrewScreen_tempContainer);
    lv_obj_set_size(ui_BrewScreen_targetTemp, 120, 40);
    lv_obj_set_align(ui_BrewScreen_targetTemp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BrewScreen_targetTemp, "93\xC2\xB0" "C");
    lv_obj_set_style_text_align(ui_BrewScreen_targetTemp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_BrewScreen_targetTemp, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_upTempButton = lv_imgbtn_create(ui_BrewScreen_tempContainer);
    lv_imgbtn_set_src(ui_BrewScreen_upTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_BrewScreen_upTempButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_upTempButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_BrewScreen_upTempButton, 80, 0);
    ui_object_set_themeable_style_property(ui_BrewScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Duration row
    ui_BrewScreen_targetContainer = lv_obj_create(ui_BrewScreen_adjustments);
    lv_obj_remove_style_all(ui_BrewScreen_targetContainer);
    lv_obj_set_size(ui_BrewScreen_targetContainer, 560, 50);
    lv_obj_clear_flag(ui_BrewScreen_targetContainer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_BrewScreen_targetContainer, LV_OBJ_FLAG_CLICKABLE);

    ui_BrewScreen_Image4 = lv_img_create(ui_BrewScreen_targetContainer);
    lv_img_set_src(ui_BrewScreen_Image4, &ui_img_360122106);
    lv_obj_set_size(ui_BrewScreen_Image4, 40, 40);
    lv_obj_set_align(ui_BrewScreen_Image4, LV_ALIGN_LEFT_MID);
    lv_obj_set_pos(ui_BrewScreen_Image4, 0, 0);
    lv_obj_add_flag(ui_BrewScreen_Image4, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_BrewScreen_Image4, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_BrewScreen_Image4, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_Image4, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_downDurationButton = lv_imgbtn_create(ui_BrewScreen_targetContainer);
    lv_imgbtn_set_src(ui_BrewScreen_downDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_BrewScreen_downDurationButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_downDurationButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_BrewScreen_downDurationButton, -80, 0);
    ui_object_set_themeable_style_property(ui_BrewScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_targetDuration = lv_label_create(ui_BrewScreen_targetContainer);
    lv_obj_set_size(ui_BrewScreen_targetDuration, 120, 40);
    lv_obj_set_align(ui_BrewScreen_targetDuration, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BrewScreen_targetDuration, "0:30");
    lv_obj_set_style_text_align(ui_BrewScreen_targetDuration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_BrewScreen_targetDuration, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_upDurationButton = lv_imgbtn_create(ui_BrewScreen_targetContainer);
    lv_imgbtn_set_src(ui_BrewScreen_upDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_BrewScreen_upDurationButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_upDurationButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_BrewScreen_upDurationButton, 80, 0);
    ui_object_set_themeable_style_property(ui_BrewScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Mode switch (off-screen dummy, for DefaultUI compatibility) ───────────
    ui_BrewScreen_modeSwitch = lv_obj_create(ui_BrewScreen);
    lv_obj_remove_style_all(ui_BrewScreen_modeSwitch);
    lv_obj_set_size(ui_BrewScreen_modeSwitch, 1, 1);
    lv_obj_set_pos(ui_BrewScreen_modeSwitch, -200, -200);
    lv_obj_add_flag(ui_BrewScreen_modeSwitch, LV_OBJ_FLAG_HIDDEN);

    ui_BrewScreen_volumetricButton = lv_img_create(ui_BrewScreen_modeSwitch);
    lv_img_set_src(ui_BrewScreen_volumetricButton, &ui_img_1424216268);
    lv_obj_set_size(ui_BrewScreen_volumetricButton, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_BrewScreen_volumetricButton, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_BrewScreen_volumetricButton, LV_OBJ_FLAG_SCROLLABLE);

    // ── Metric pills (always visible) ─────────────────────────────────────────
    // Left pill: flow rate  pos(108,340) 182×46
    {
        lv_obj_t *pill = lv_obj_create(ui_BrewScreen);
        lv_obj_set_size(pill, 182, 46);
        lv_obj_set_pos(pill, 108, 340);
        lv_obj_clear_flag(pill, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(pill, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(pill, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(pill, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(pill, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(pill, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(pill, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t *lbl = lv_label_create(pill);
        lv_obj_set_size(lbl, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_align(lbl, LV_ALIGN_CENTER);
        lv_label_set_text(lbl, "0.0 ml/s");
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xE8A020), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // Center pill: weight  pos(304,350) 192×46
    {
        lv_obj_t *pill = lv_obj_create(ui_BrewScreen);
        lv_obj_set_size(pill, 192, 46);
        lv_obj_set_pos(pill, 304, 320);
        lv_obj_clear_flag(pill, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(pill, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(pill, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(pill, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(pill, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(pill, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(pill, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        ui_BrewScreen_weightLabel = lv_label_create(pill);
        lv_obj_set_size(ui_BrewScreen_weightLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_align(ui_BrewScreen_weightLabel, LV_ALIGN_CENTER);
        lv_label_set_text(ui_BrewScreen_weightLabel, "-");
        lv_obj_set_style_text_font(ui_BrewScreen_weightLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui_BrewScreen_weightLabel, lv_color_hex(0xE8A020), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // Right pill: time  pos(510,340) 182×46
    {
        lv_obj_t *pill = lv_obj_create(ui_BrewScreen);
        lv_obj_set_size(pill, 182, 46);
        lv_obj_set_pos(pill, 510, 340);
        lv_obj_clear_flag(pill, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(pill, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(pill, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(pill, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(pill, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(pill, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(pill, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t *lbl = lv_label_create(pill);
        lv_obj_set_size(lbl, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_align(lbl, LV_ALIGN_CENTER);
        lv_label_set_text(lbl, "0.0 s");
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xE8A020), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // ── By-time button (off-screen — never visible on 43", kept for DefaultUI compatibility) ──
    ui_BrewScreen_byTimeButton = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_byTimeButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_BrewScreen_byTimeButton, 40, 40);
    lv_obj_set_align(ui_BrewScreen_byTimeButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_BrewScreen_byTimeButton, 0, 600);
    lv_obj_add_flag(ui_BrewScreen_byTimeButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_byTimeButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_byTimeButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Save / accept / save-as-new (off-screen — never visible on 43") ───────
    // DefaultUI may clear HIDDEN on these; placing them at y=600 keeps them
    // functionally present but invisible on the 480px-tall display.
    ui_BrewScreen_saveButton = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_saveButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_631115820, NULL);
    lv_obj_set_size(ui_BrewScreen_saveButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_saveButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_BrewScreen_saveButton, 0, 600);
    lv_obj_add_flag(ui_BrewScreen_saveButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_acceptButton = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_acceptButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_BrewScreen_acceptButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_acceptButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_BrewScreen_acceptButton, 60, 600);
    lv_obj_add_flag(ui_BrewScreen_acceptButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_saveAsNewButton = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_saveAsNewButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_332059803, NULL);
    lv_obj_set_size(ui_BrewScreen_saveAsNewButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_saveAsNewButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_BrewScreen_saveAsNewButton, 120, 600);
    lv_obj_add_flag(ui_BrewScreen_saveAsNewButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveAsNewButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveAsNewButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Start (play) button — plain white imgbtn, 40×40 native size (no ring, no bg) ──
    // Sized exactly to the native icon to prevent lv_imgbtn tiling artefacts.
    ui_BrewScreen_startButton = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_startButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_BrewScreen_startButton, 40, 40);
    lv_obj_set_align(ui_BrewScreen_startButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_BrewScreen_startButton, 0, -40);
    ui_object_set_themeable_style_property(ui_BrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Events ────────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_BrewScreen,                    ui_event_BrewScreen,                    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_ImgButton5,         ui_event_BrewScreen_ImgButton5,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_startButton,        ui_event_BrewScreen_startButton,        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_modeSwitch,         ui_event_BrewScreen_modeSwitch,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_profileSelectBtn,   ui_event_BrewScreen_profileSelectBtn,   LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_settingsButton,     onBrewSettings43,                       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_downTempButton,     ui_event_BrewScreen_downTempButton,     LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_upTempButton,       ui_event_BrewScreen_upTempButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_upDurationButton,   ui_event_BrewScreen_upDurationButton,   LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_downDurationButton, ui_event_BrewScreen_downDurationButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_byTimeButton,       ui_event_BrewScreen_byTimeButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_saveButton,         ui_event_BrewScreen_saveButton,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_acceptButton,       ui_event_BrewScreen_acceptButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_saveAsNewButton,    ui_event_BrewScreen_saveAsNewButton,    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_Label1,             onSelectedProfileAdaptiveToggle,        LV_EVENT_CLICKED, NULL);

    // ── Cache component child pointers ────────────────────────────────────────
    uic_BrewScreen_dials_tempGauge      = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_BrewScreen_dials_tempTarget     = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_BrewScreen_dials_pressureGauge  = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_BrewScreen_dials_pressureTarget = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    // Keep sidebar dials text pointers pointing at the dials component labels
    uic_BrewScreen_dials_pressureText   = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_BrewScreen_dials_tempText       = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_TEMPTEXT);
    // Expose standalone circle labels for DefaultUI.cpp to update independently
    ui_BrewScreen_circleTempLabel     = brewCircleTempLabel;
    ui_BrewScreen_circlePressureLabel = brewCirclePressureLabel;
}
