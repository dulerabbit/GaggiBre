// GaggiMate — 4.3" Status Screen (800×480)
// Layout: thin left/right sidebars | 685px centred main content (x=60)
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

    // ── Content panel (685×480, centred between sidebars) ─────────────────
    ui_StatusScreen_contentPanel2 = lv_obj_create(ui_StatusScreen);
    lv_obj_remove_style_all(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_contentPanel2, 685, 480);
    lv_obj_set_pos(ui_StatusScreen_contentPanel2, 60, 0);
    lv_obj_clear_flag(ui_StatusScreen_contentPanel2, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_StatusScreen_contentPanel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Menu (back) button — top-centre of content panel
    ui_StatusScreen_ImgButton8 = lv_imgbtn_create(ui_StatusScreen_contentPanel2);
    lv_imgbtn_set_src(ui_StatusScreen_ImgButton8, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_StatusScreen_ImgButton8, 44, 44);
    lv_obj_set_align(ui_StatusScreen_ImgButton8, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_StatusScreen_ImgButton8, 0, 14);
    ui_object_set_themeable_style_property(ui_StatusScreen_ImgButton8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_ImgButton8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Step label ─────────────────────────────────────────────────────────
    ui_StatusScreen_stepLabel = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_stepLabel, 500, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_stepLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_stepLabel, 0, -145);
    lv_label_set_text(ui_StatusScreen_stepLabel, "Step");
    lv_obj_set_style_text_align(ui_StatusScreen_stepLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_stepLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_stepLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_stepLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_SemiDark);

    // ── Phase label (scrolling) ────────────────────────────────────────────
    ui_StatusScreen_phaseLabel = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_phaseLabel, 560, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_phaseLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_phaseLabel, 0, -95);
    lv_label_set_long_mode(ui_StatusScreen_phaseLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_StatusScreen_phaseLabel, "Phase");
    lv_obj_set_style_text_align(ui_StatusScreen_phaseLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_phaseLabel, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_phaseLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_phaseLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Adaptive / channeling warning label (centre slot) ─────────────────
    // DefaultUI.cpp sets this to "⚠ CHANNELING", "Adaptive +0.3 bar", "[A]", or "" (hidden).
    ui_StatusScreen_adaptiveLabel = lv_label_create(ui_StatusScreen_contentPanel2);
    lv_obj_set_size(ui_StatusScreen_adaptiveLabel, 560, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StatusScreen_adaptiveLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_adaptiveLabel, 0, -35);
    lv_label_set_text(ui_StatusScreen_adaptiveLabel, "");
    lv_obj_set_style_text_align(ui_StatusScreen_adaptiveLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusScreen_adaptiveLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_adaptiveLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_adaptiveLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Progress bar container ─────────────────────────────────────────────
    ui_StatusScreen_barContainer = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(ui_StatusScreen_barContainer);
    lv_obj_set_size(ui_StatusScreen_barContainer, 580, 18);
    lv_obj_set_align(ui_StatusScreen_barContainer, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_barContainer, 0, 40);
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

    // ── Label container (phase progress text) ─────────────────────────────
    ui_StatusScreen_labelContainer = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(ui_StatusScreen_labelContainer);
    lv_obj_set_size(ui_StatusScreen_labelContainer, 580, 24);
    lv_obj_set_align(ui_StatusScreen_labelContainer, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StatusScreen_labelContainer, 0, 66);
    lv_obj_set_flex_flow(ui_StatusScreen_labelContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_StatusScreen_labelContainer, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_StatusScreen_labelContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StatusScreen_brewLabel = lv_label_create(ui_StatusScreen_labelContainer);
    lv_obj_set_flex_grow(ui_StatusScreen_brewLabel, 1);
    lv_label_set_text(ui_StatusScreen_brewLabel, "0s");
    lv_obj_set_style_text_font(ui_StatusScreen_brewLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_SemiDark);

    // ── Bottom row: [targets-left]  [STOP centre]  [live-right] ──────────
    // Stop button — bottom-centre, raised slightly above the side groups
    ui_StatusScreen_pauseButton = lv_imgbtn_create(ui_StatusScreen_contentPanel2);
    lv_imgbtn_set_src(ui_StatusScreen_pauseButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1456692430, NULL);
    lv_obj_set_size(ui_StatusScreen_pauseButton, 54, 54);
    lv_obj_set_align(ui_StatusScreen_pauseButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_StatusScreen_pauseButton, 0, -28);
    ui_object_set_themeable_style_property(ui_StatusScreen_pauseButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_pauseButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Left group: static targets (thermometer+temp, clock+duration) ─────
    // Condensed flex column, bottom-left, slightly lower than stop button
    lv_obj_t *targetsGroup = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(targetsGroup);
    lv_obj_set_size(targetsGroup, 220, 60);
    lv_obj_set_align(targetsGroup, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_pos(targetsGroup, 20, -14);
    lv_obj_set_flex_flow(targetsGroup, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(targetsGroup, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(targetsGroup, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(targetsGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    // Temp row: thermometer icon + target temp
    lv_obj_t *tempRow = lv_obj_create(targetsGroup);
    lv_obj_remove_style_all(tempRow);
    lv_obj_set_size(tempRow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(tempRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tempRow, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(tempRow, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(tempRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StatusScreen_Image7 = lv_img_create(tempRow);
    lv_img_set_src(ui_StatusScreen_Image7, &ui_img_1951499226);
    lv_obj_set_size(ui_StatusScreen_Image7, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_zoom(ui_StatusScreen_Image7, 160);
    lv_obj_add_flag(ui_StatusScreen_Image7, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StatusScreen_Image7, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image7, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image7, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_SemiDark);

    ui_StatusScreen_targetTemp = lv_label_create(tempRow);
    lv_obj_set_size(ui_StatusScreen_targetTemp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_StatusScreen_targetTemp, "93\xC2\xB0""C");
    lv_obj_set_style_text_font(ui_StatusScreen_targetTemp, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_SemiDark);

    // Duration row: clock icon + target duration
    lv_obj_t *durRow = lv_obj_create(targetsGroup);
    lv_obj_remove_style_all(durRow);
    lv_obj_set_size(durRow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(durRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(durRow, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(durRow, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(durRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StatusScreen_Image8 = lv_img_create(durRow);
    lv_img_set_src(ui_StatusScreen_Image8, &ui_img_360122106);
    lv_obj_set_size(ui_StatusScreen_Image8, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_zoom(ui_StatusScreen_Image8, 160);
    lv_obj_add_flag(ui_StatusScreen_Image8, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StatusScreen_Image8, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_Image8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_SemiDark);

    ui_StatusScreen_targetDuration = lv_label_create(durRow);
    lv_obj_set_size(ui_StatusScreen_targetDuration, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_StatusScreen_targetDuration, "0:30");
    lv_obj_set_style_text_font(ui_StatusScreen_targetDuration, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_SemiDark);
    ui_object_set_themeable_style_property(ui_StatusScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_SemiDark);

    // ── Right group: live elapsed time + brew weight ───────────────────────
    lv_obj_t *liveGroup = lv_obj_create(ui_StatusScreen_contentPanel2);
    lv_obj_remove_style_all(liveGroup);
    lv_obj_set_size(liveGroup, 220, 60);
    lv_obj_set_align(liveGroup, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_pos(liveGroup, -20, -14);
    lv_obj_set_flex_flow(liveGroup, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(liveGroup, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_set_style_pad_row(liveGroup, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(liveGroup, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    // Elapsed time row: clock icon + currentDuration label
    lv_obj_t *elapsedRow = lv_obj_create(liveGroup);
    lv_obj_remove_style_all(elapsedRow);
    lv_obj_set_size(elapsedRow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(elapsedRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(elapsedRow, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(elapsedRow, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(elapsedRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *elapsedIcon = lv_img_create(elapsedRow);
    lv_img_set_src(elapsedIcon, &ui_img_360122106);
    lv_obj_set_size(elapsedIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_zoom(elapsedIcon, 160);
    lv_obj_add_flag(elapsedIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(elapsedIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(elapsedIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(elapsedIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Live elapsed time label — created directly inside elapsedRow
    ui_StatusScreen_currentDuration = lv_label_create(elapsedRow);
    lv_obj_set_size(ui_StatusScreen_currentDuration, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_StatusScreen_currentDuration, "0:00");
    lv_obj_set_style_text_font(ui_StatusScreen_currentDuration, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_currentDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_currentDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Weight row: weight icon + brewVolume label
    lv_obj_t *weightRow = lv_obj_create(liveGroup);
    lv_obj_remove_style_all(weightRow);
    lv_obj_set_size(weightRow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(weightRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(weightRow, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(weightRow, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(weightRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *weightIcon = lv_img_create(weightRow);
    lv_img_set_src(weightIcon, &ui_img_1424216268);
    lv_obj_set_size(weightIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_zoom(weightIcon, 160);
    lv_obj_add_flag(weightIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(weightIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(weightIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(weightIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Brew weight label — created directly inside weightRow
    ui_StatusScreen_brewVolume = lv_label_create(weightRow);
    lv_obj_set_size(ui_StatusScreen_brewVolume, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_StatusScreen_brewVolume, "0.0g");
    lv_obj_set_style_text_font(ui_StatusScreen_brewVolume, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewVolume, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StatusScreen_brewVolume, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

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
