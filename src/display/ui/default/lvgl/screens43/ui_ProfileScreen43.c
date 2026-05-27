// GaggiMate — 4.3" Profile Screen (800×480)
// Layout: 180px sidebar | 618px main content
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_ProfileScreen_screen_init_43(void) {
    ui_ProfileScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ProfileScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_ProfileScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_ProfileScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_ProfileScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_ProfileScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Sidebar
    ui_ProfileScreen_dials = ui_dials43_create(ui_ProfileScreen);

    // ── Content panel (618×480) ────────────────────────────────────────────
    ui_ProfileScreen_contentPanel = lv_obj_create(ui_ProfileScreen);
    lv_obj_remove_style_all(ui_ProfileScreen_contentPanel);
    lv_obj_set_size(ui_ProfileScreen_contentPanel, 618, 480);
    lv_obj_set_pos(ui_ProfileScreen_contentPanel, 182, 0);
    lv_obj_clear_flag(ui_ProfileScreen_contentPanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_ProfileScreen_contentPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Menu (back) button
    ui_ProfileScreen_ImgButton1 = lv_imgbtn_create(ui_ProfileScreen_contentPanel);
    lv_imgbtn_set_src(ui_ProfileScreen_ImgButton1, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_ProfileScreen_ImgButton1, 44, 44);
    lv_obj_set_align(ui_ProfileScreen_ImgButton1, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_ProfileScreen_ImgButton1, 14, 14);
    ui_object_set_themeable_style_property(ui_ProfileScreen_ImgButton1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_ImgButton1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // "Select profile" label
    ui_ProfileScreen_mainLabel = lv_label_create(ui_ProfileScreen_contentPanel);
    lv_obj_set_size(ui_ProfileScreen_mainLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ProfileScreen_mainLabel, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileScreen_mainLabel, 0, 20);
    lv_label_set_text(ui_ProfileScreen_mainLabel, "Select profile");
    lv_obj_set_style_text_font(ui_ProfileScreen_mainLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileScreen_mainLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_mainLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Profile name (scrolling)
    ui_ProfileScreen_profileName = lv_label_create(ui_ProfileScreen_contentPanel);
    lv_obj_set_size(ui_ProfileScreen_profileName, 540, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ProfileScreen_profileName, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileScreen_profileName, 0, 56);
    lv_label_set_long_mode(ui_ProfileScreen_profileName, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_ProfileScreen_profileName, "Cremina Lever");
    lv_obj_set_style_text_align(ui_ProfileScreen_profileName, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileScreen_profileName, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Previous profile button (left side)
    ui_ProfileScreen_previousProfileBtn = lv_imgbtn_create(ui_ProfileScreen_contentPanel);
    lv_imgbtn_set_src(ui_ProfileScreen_previousProfileBtn, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_ProfileScreen_previousProfileBtn, 50, 50);
    lv_obj_set_align(ui_ProfileScreen_previousProfileBtn, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileScreen_previousProfileBtn, -250, 0);
    ui_object_set_themeable_style_property(ui_ProfileScreen_previousProfileBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_previousProfileBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Next profile button (right side)
    ui_ProfileScreen_nextProfileBtn = lv_imgbtn_create(ui_ProfileScreen_contentPanel);
    lv_imgbtn_set_src(ui_ProfileScreen_nextProfileBtn, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_ProfileScreen_nextProfileBtn, 50, 50);
    lv_obj_set_align(ui_ProfileScreen_nextProfileBtn, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileScreen_nextProfileBtn, 250, 0);
    ui_object_set_themeable_style_property(ui_ProfileScreen_nextProfileBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_nextProfileBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Loading spinner
    ui_ProfileScreen_loadingSpinner = lv_spinner_create(ui_ProfileScreen_contentPanel, 1000, 60);
    lv_obj_set_size(ui_ProfileScreen_loadingSpinner, 80, 80);
    lv_obj_center(ui_ProfileScreen_loadingSpinner);
    lv_obj_add_flag(ui_ProfileScreen_loadingSpinner, LV_OBJ_FLAG_HIDDEN);

    // ── Profile details overlay (618×480, hidden initially) ───────────────
    ui_ProfileScreen_profileDetails = lv_obj_create(ui_ProfileScreen_contentPanel);
    lv_obj_remove_style_all(ui_ProfileScreen_profileDetails);
    lv_obj_set_size(ui_ProfileScreen_profileDetails, 618, 480);
    lv_obj_set_pos(ui_ProfileScreen_profileDetails, 0, 0);
    lv_obj_clear_flag(ui_ProfileScreen_profileDetails, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_ProfileScreen_profileDetails, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_opa(ui_ProfileScreen_profileDetails, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Choose button — bottom centre of details overlay
    ui_ProfileScreen_chooseButton = lv_imgbtn_create(ui_ProfileScreen_profileDetails);
    lv_imgbtn_set_src(ui_ProfileScreen_chooseButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_ProfileScreen_chooseButton, 54, 54);
    lv_obj_set_align(ui_ProfileScreen_chooseButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_ProfileScreen_chooseButton, 0, -18);
    ui_object_set_themeable_style_property(ui_ProfileScreen_chooseButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_chooseButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Simple content (steps + phases text for simple profiles)
    ui_ProfileScreen_simpleContent = lv_obj_create(ui_ProfileScreen_profileDetails);
    lv_obj_remove_style_all(ui_ProfileScreen_simpleContent);
    lv_obj_set_size(ui_ProfileScreen_simpleContent, 560, 140);
    lv_obj_set_align(ui_ProfileScreen_simpleContent, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileScreen_simpleContent, 0, -50);
    lv_obj_set_flex_flow(ui_ProfileScreen_simpleContent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_ProfileScreen_simpleContent, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ProfileScreen_simpleContent, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_ProfileScreen_phasesLabel = lv_label_create(ui_ProfileScreen_simpleContent);
    lv_obj_set_size(ui_ProfileScreen_phasesLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ProfileScreen_phasesLabel, "3 phases");
    lv_obj_set_style_text_font(ui_ProfileScreen_phasesLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileScreen_phasesLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_phasesLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_ProfileScreen_stepsLabel = lv_label_create(ui_ProfileScreen_simpleContent);
    lv_obj_set_size(ui_ProfileScreen_stepsLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ProfileScreen_stepsLabel, "5 steps");
    lv_obj_set_style_text_font(ui_ProfileScreen_stepsLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileScreen_stepsLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_stepsLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Extended content (chart for advanced profiles, hidden by default)
    ui_ProfileScreen_extendedContent = lv_obj_create(ui_ProfileScreen_profileDetails);
    lv_obj_remove_style_all(ui_ProfileScreen_extendedContent);
    lv_obj_set_size(ui_ProfileScreen_extendedContent, 560, 240);
    lv_obj_set_align(ui_ProfileScreen_extendedContent, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileScreen_extendedContent, 0, -60);
    lv_obj_clear_flag(ui_ProfileScreen_extendedContent, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_ProfileScreen_extendedContent, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_opa(ui_ProfileScreen_extendedContent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ProfileScreen_Chart1 = lv_chart_create(ui_ProfileScreen_extendedContent);
    lv_obj_set_size(ui_ProfileScreen_Chart1, 540, 200);
    lv_obj_center(ui_ProfileScreen_Chart1);
    lv_chart_set_type(ui_ProfileScreen_Chart1, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui_ProfileScreen_Chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 15);
    lv_chart_set_point_count(ui_ProfileScreen_Chart1, 50);
    lv_obj_set_style_bg_opa(ui_ProfileScreen_Chart1, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ProfileScreen_Chart1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_ProfileScreen_Chart1, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_size(ui_ProfileScreen_Chart1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_chart_add_series(ui_ProfileScreen_Chart1, lv_color_hex(0x2CA4F6), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_add_series(ui_ProfileScreen_Chart1, lv_color_hex(0xF6812C), LV_CHART_AXIS_PRIMARY_Y);

    // Target info row — inside simpleContent bottom
    lv_obj_t *targetRow = lv_obj_create(ui_ProfileScreen_profileDetails);
    lv_obj_remove_style_all(targetRow);
    lv_obj_set_size(targetRow, 500, 44);
    lv_obj_set_align(targetRow, LV_ALIGN_CENTER);
    lv_obj_set_pos(targetRow, 0, 100);
    lv_obj_set_flex_flow(targetRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(targetRow, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(targetRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_ProfileScreen_tempIcon = lv_img_create(targetRow);
    lv_img_set_src(ui_ProfileScreen_tempIcon, &ui_img_1951499226);
    lv_obj_set_size(ui_ProfileScreen_tempIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_ProfileScreen_tempIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ProfileScreen_tempIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_ProfileScreen_tempIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_tempIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileScreen_targetTemp2 = lv_label_create(targetRow);
    lv_obj_set_size(ui_ProfileScreen_targetTemp2, 100, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ProfileScreen_targetTemp2, "93°C");
    lv_obj_set_style_text_align(ui_ProfileScreen_targetTemp2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileScreen_targetTemp2, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileScreen_targetTemp2, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_targetTemp2, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_ProfileScreen_targetIcon = lv_img_create(targetRow);
    lv_img_set_src(ui_ProfileScreen_targetIcon, &ui_img_360122106);
    lv_obj_set_size(ui_ProfileScreen_targetIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_ProfileScreen_targetIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ProfileScreen_targetIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_ProfileScreen_targetIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_targetIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileScreen_targetDuration2 = lv_label_create(targetRow);
    lv_obj_set_size(ui_ProfileScreen_targetDuration2, 100, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ProfileScreen_targetDuration2, "0:30");
    lv_obj_set_style_text_align(ui_ProfileScreen_targetDuration2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileScreen_targetDuration2, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileScreen_targetDuration2, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileScreen_targetDuration2, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Events ────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_ProfileScreen,                      ui_event_ProfileScreen,                      LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileScreen_ImgButton1,           ui_event_ProfileScreen_ImgButton1,           LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileScreen_previousProfileBtn,   ui_event_ProfileScreen_previousProfileBtn,   LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileScreen_nextProfileBtn,       ui_event_ProfileScreen_nextProfileBtn,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileScreen_chooseButton,         ui_event_ProfileScreen_chooseButton,         LV_EVENT_ALL, NULL);

    // ── Cache component child pointers ────────────────────────────────────
    uic_ProfileScreen_dials_tempGauge      = ui_comp_get_child(ui_ProfileScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_ProfileScreen_dials_tempTarget     = ui_comp_get_child(ui_ProfileScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_ProfileScreen_dials_pressureGauge  = ui_comp_get_child(ui_ProfileScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_ProfileScreen_dials_pressureTarget = ui_comp_get_child(ui_ProfileScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_ProfileScreen_dials_pressureText   = ui_comp_get_child(ui_ProfileScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_ProfileScreen_dials_tempText       = ui_comp_get_child(ui_ProfileScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
