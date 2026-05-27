// GaggiMate — 4.3" Status Screen (800×480)
// Layout: 180px sidebar | 618px main content
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_StatusScreen_screen_init_43(void) {
    ui_StatusScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_StatusScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_StatusScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_StatusScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_StatusScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_StatusScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Sidebar
    ui_StatusScreen_dials = ui_dials43_create(ui_StatusScreen);

    // ── Content panel (618×480) ────────────────────────────────────────────
    ui_StatusScreen_contentPanel2 = lv_obj_create(ui_StatusScreen);
    lv_obj_remove_style_all(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_contentPanel2, 618, 480);
    lv_obj_set_pos(ui_StatusScreen_contentPanel2, 182, 0);
    lv_obj_clear_flag(ui_StatusScreen_contentPanel2, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_StatusScreen_contentPanel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Menu (back) button — top-left of content panel
    ui_StatusScreen_ImgButton8 = lv_imgbtn_create(ui_StatusScreen_contentPanel2);
    lv_imgbtn_set_src(ui_StatusScreen_ImgButton8, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_StatusScreen_ImgButton8, 44, 44);
    lv_obj_set_align(ui_StatusScreen_ImgButton8, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_StatusScreen_ImgButton8, 14, 14);
    ui_object_set_themeable_style_property(ui_StatusScreen_ImgButton8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_ImgButton8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Top info row (thermometer icon | target temp | timer icon | target duration) ──
    lv_obj_t *topRow = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(topRow);
    lv_obj_set_size(topRow, 520, 44);
    lv_obj_set_align(topRow, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(topRow, 0, 16);
    lv_obj_set_flex_flow(topRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(topRow, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(topRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StatusScreen_Image7 = lv_img_create(topRow);
    lv_img_set_src(ui_StatusScreen_Image7, &ui_img_1951499226);
    lv_obj_set_size(ui_StatusScreen_Image7, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_StatusScreen_Image7, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StatusScreen_Image7, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image7, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image7, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_StatusScreen_targetTemp = lv_label_create(topRow);
    lv_obj_set_size(ui_StatusScreen_targetTemp, 130, LV_SIZE_CONTENT);
    lv_label_set_text(ui_StatusScreen_targetTemp, "93°C");
    lv_obj_set_style_text_align(ui_StatusScreen_targetTemp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_targetTemp, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_StatusScreen_Image8 = lv_img_create(topRow);
    lv_img_set_src(ui_StatusScreen_Image8, &ui_img_360122106);
    lv_obj_set_size(ui_StatusScreen_Image8, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_StatusScreen_Image8, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StatusScreen_Image8, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_StatusScreen_targetDuration = lv_label_create(topRow);
    lv_obj_set_size(ui_StatusScreen_targetDuration, 130, LV_SIZE_CONTENT);
    lv_label_set_text(ui_StatusScreen_targetDuration, "0:30");
    lv_obj_set_style_text_align(ui_StatusScreen_targetDuration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_targetDuration, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Step label ─────────────────────────────────────────────────────────
    ui_StatusScreen_stepLabel = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_stepLabel, 500, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_stepLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_stepLabel, 0, -80);
    lv_label_set_text(ui_StatusScreen_stepLabel, "Step");
    lv_obj_set_style_text_align(ui_StatusScreen_stepLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_stepLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_stepLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_stepLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_SemiDark);

    // ── Phase label (scrolling) ────────────────────────────────────────────
    ui_StatusScreen_phaseLabel = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_phaseLabel, 540, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_phaseLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_phaseLabel, 0, -48);
    lv_label_set_long_mode(ui_StatusScreen_phaseLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_StatusScreen_phaseLabel, "Phase");
    lv_obj_set_style_text_align(ui_StatusScreen_phaseLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_phaseLabel, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_phaseLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_phaseLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Current duration (large, centred) ─────────────────────────────────
    ui_StatusScreen_currentDuration = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_currentDuration, 260, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_currentDuration, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_currentDuration, 0, 30);
    lv_label_set_text(ui_StatusScreen_currentDuration, "0:00");
    lv_obj_set_style_text_align(ui_StatusScreen_currentDuration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_currentDuration, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_currentDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_currentDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Progress bar container ─────────────────────────────────────────────
    ui_StatusScreen_barContainer = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(ui_StatusScreen_barContainer);
    lv_obj_set_size(ui_StatusScreen_barContainer, 560, 18);
    lv_obj_set_align(ui_StatusScreen_barContainer, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_barContainer, 0, 105);
    lv_obj_set_flex_flow(ui_StatusScreen_barContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_StatusScreen_barContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_StatusScreen_barContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StatusScreen_brewBar = lv_bar_create(ui_StatusScreen_barContainer);
    lv_obj_set_flex_grow(ui_StatusScreen_brewBar, 1);
    lv_obj_set_height(ui_StatusScreen_brewBar, 16);
    lv_bar_set_range(ui_StatusScreen_brewBar, 0, 27);
    lv_bar_set_value(ui_StatusScreen_brewBar, 0, LV_ANIM_OFF);
    lv_obj_set_style_radius(ui_StatusScreen_brewBar, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_StatusScreen_brewBar, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewBar, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewBar, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewBar, LV_PART_INDICATOR | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Progress);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewBar, LV_PART_INDICATOR | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Progress);

    // ── Label container ────────────────────────────────────────────────────
    ui_StatusScreen_labelContainer = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(ui_StatusScreen_labelContainer);
    lv_obj_set_size(ui_StatusScreen_labelContainer, 560, 24);
    lv_obj_set_align(ui_StatusScreen_labelContainer, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_labelContainer, 0, 128);
    lv_obj_set_flex_flow(ui_StatusScreen_labelContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_StatusScreen_labelContainer, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_StatusScreen_labelContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StatusScreen_brewLabel = lv_label_create(ui_StatusScreen_labelContainer);
    lv_obj_set_flex_grow(ui_StatusScreen_brewLabel, 1);
    lv_label_set_text(ui_StatusScreen_brewLabel, "0s");
    lv_obj_set_style_text_font(ui_StatusScreen_brewLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_SemiDark);

    // ── Brew volume ────────────────────────────────────────────────────────
    ui_StatusScreen_brewVolume = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_brewVolume, 200, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_brewVolume, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_brewVolume, 0, 160);
    lv_label_set_text(ui_StatusScreen_brewVolume, "0.0g");
    lv_obj_set_style_text_align(ui_StatusScreen_brewVolume, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_brewVolume, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewVolume, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewVolume, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Pause button ───────────────────────────────────────────────────────
    ui_StatusScreen_pauseButton = lv_imgbtn_create(ui_StatusScreen_contentPanel2);
    lv_imgbtn_set_src(ui_StatusScreen_pauseButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_340148213, NULL);
    lv_obj_set_size(ui_StatusScreen_pauseButton, 50, 50);
    lv_obj_set_align(ui_StatusScreen_pauseButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_StatusScreen_pauseButton, 0, -20);
    ui_object_set_themeable_style_property(ui_StatusScreen_pauseButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_pauseButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Events ────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_StatusScreen,               ui_event_StatusScreen,               LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_StatusScreen_ImgButton8,    ui_event_StatusScreen_ImgButton8,    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_StatusScreen_pauseButton,   ui_event_StatusScreen_pauseButton,   LV_EVENT_ALL, NULL);

    // ── Cache component child pointers ────────────────────────────────────
    uic_StatusScreen_dials_tempGauge      = ui_comp_get_child(ui_StatusScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_StatusScreen_dials_tempTarget     = ui_comp_get_child(ui_StatusScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_StatusScreen_dials_pressureGauge  = ui_comp_get_child(ui_StatusScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_StatusScreen_dials_pressureTarget = ui_comp_get_child(ui_StatusScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_StatusScreen_dials_pressureText   = ui_comp_get_child(ui_StatusScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_StatusScreen_dials_tempText       = ui_comp_get_child(ui_StatusScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
