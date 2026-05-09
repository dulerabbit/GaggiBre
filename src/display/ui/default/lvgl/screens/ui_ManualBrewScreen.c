#include "../ui.h"

// Dial component child pointers
lv_obj_t *uic_ManualBrewScreen_dials_tempText;
lv_obj_t *uic_ManualBrewScreen_dials_pressureText;
lv_obj_t *uic_ManualBrewScreen_dials_pressureTarget;
lv_obj_t *uic_ManualBrewScreen_dials_pressureGauge;
lv_obj_t *uic_ManualBrewScreen_dials_tempTarget;
lv_obj_t *uic_ManualBrewScreen_dials_tempGauge;

// Screen root + outer dials
lv_obj_t *ui_ManualBrewScreen             = NULL;
lv_obj_t *ui_ManualBrewScreen_dials       = NULL;
lv_obj_t *ui_ManualBrewScreen_tempZone    = NULL;
lv_obj_t *ui_ManualBrewScreen_pressureZone = NULL;
lv_obj_t *ui_ManualBrewScreen_backButton  = NULL;
lv_obj_t *ui_ManualBrewScreen_contentPanel = NULL;
lv_obj_t *ui_ManualBrewScreen_mainLabel   = NULL;

// Live data chart (top of inner circle): 3 series (pressure/temp/flow)
lv_obj_t            *ui_ManualBrewScreen_chart          = NULL;
lv_chart_series_t   *ui_ManualBrewScreen_chart_pressure = NULL;
lv_chart_series_t   *ui_ManualBrewScreen_chart_temp     = NULL;
lv_chart_series_t   *ui_ManualBrewScreen_chart_flow     = NULL;

// Centre pressure setpoint circle + large number
lv_obj_t *ui_ManualBrewScreen_pressureCircle = NULL;
lv_obj_t *ui_ManualBrewScreen_pressureTarget = NULL;

// Bottom stats row
lv_obj_t *ui_ManualBrewScreen_bottomRow  = NULL;
lv_obj_t *ui_ManualBrewScreen_targetTemp = NULL;
lv_obj_t *ui_ManualBrewScreen_elapsed    = NULL;
lv_obj_t *ui_ManualBrewScreen_flow       = NULL;
lv_obj_t *ui_ManualBrewScreen_weight     = NULL;

// Start / stop button
lv_obj_t *ui_ManualBrewScreen_startButton = NULL;

// Post-shot save overlay
lv_obj_t *ui_ManualBrewScreen_savePanel     = NULL;
lv_obj_t *ui_ManualBrewScreen_saveNameLabel = NULL;
lv_obj_t *ui_ManualBrewScreen_saveButton    = NULL;
lv_obj_t *ui_ManualBrewScreen_discardButton = NULL;

void ui_event_ManualBrewScreen(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_SCREEN_LOADED) {
        onManualBrewScreenLoad(e);
    }
}

void ui_event_ManualBrewScreen_backButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) onMenuClick(e);
}

void ui_event_ManualBrewScreen_startButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) onManualBrewToggle(e);
}

void ui_event_ManualBrewScreen_tempZone(lv_event_t *e) { onManualBrewAdjustTemp(e); }

void ui_event_ManualBrewScreen_pressureZone(lv_event_t *e) { onManualBrewAdjustPressure(e); }

void ui_event_ManualBrewScreen_saveButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) onManualBrewSave(e);
}

void ui_event_ManualBrewScreen_discardButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) onManualBrewDiscard(e);
}

void ui_ManualBrewScreen_screen_init(void) {
    ui_ManualBrewScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ManualBrewScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_dials = ui_dials_create(ui_ManualBrewScreen);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_dials, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,
                                           _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_dials, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ManualBrewScreen_tempZone = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_remove_style_all(ui_ManualBrewScreen_tempZone);
    lv_obj_set_size(ui_ManualBrewScreen_tempZone, 200, 360);
    lv_obj_set_align(ui_ManualBrewScreen_tempZone, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_tempZone, -140);
    lv_obj_set_y(ui_ManualBrewScreen_tempZone, 40);
    lv_obj_add_flag(ui_ManualBrewScreen_tempZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_ManualBrewScreen_tempZone, LV_OBJ_FLAG_SCROLLABLE);

    ui_ManualBrewScreen_pressureZone = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_remove_style_all(ui_ManualBrewScreen_pressureZone);
    // Right pressure swipe segment: from top pressure readout (~y=-205) to bottom pressure icon (~y=+200).
    lv_obj_set_size(ui_ManualBrewScreen_pressureZone, 170, 430);
    lv_obj_set_align(ui_ManualBrewScreen_pressureZone, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_pressureZone, 115);
    lv_obj_set_y(ui_ManualBrewScreen_pressureZone, 0);
    lv_obj_add_flag(ui_ManualBrewScreen_pressureZone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_ManualBrewScreen_pressureZone, LV_OBJ_FLAG_SCROLLABLE);

    ui_ManualBrewScreen_backButton = lv_imgbtn_create(ui_ManualBrewScreen);
    lv_imgbtn_set_src(ui_ManualBrewScreen_backButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_width(ui_ManualBrewScreen_backButton, 40);
    lv_obj_set_height(ui_ManualBrewScreen_backButton, 40);
    lv_obj_set_x(ui_ManualBrewScreen_backButton, 0);
    lv_obj_set_y(ui_ManualBrewScreen_backButton, 210);
    lv_obj_set_align(ui_ManualBrewScreen_backButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_img_recolor(ui_ManualBrewScreen_backButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_ManualBrewScreen_backButton, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_contentPanel = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_set_width(ui_ManualBrewScreen_contentPanel, 360);
    lv_obj_set_height(ui_ManualBrewScreen_contentPanel, 360);
    lv_obj_set_align(ui_ManualBrewScreen_contentPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ManualBrewScreen_contentPanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(ui_ManualBrewScreen_contentPanel, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_contentPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_contentPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui_ManualBrewScreen_contentPanel, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_chart = lv_chart_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_set_width(ui_ManualBrewScreen_chart, 260);
    lv_obj_set_height(ui_ManualBrewScreen_chart, 110);
    lv_obj_set_align(ui_ManualBrewScreen_chart, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_chart, 0);
    lv_obj_set_y(ui_ManualBrewScreen_chart, -125);
    lv_chart_set_type(ui_ManualBrewScreen_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ui_ManualBrewScreen_chart, 60);
    lv_chart_set_range(ui_ManualBrewScreen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 160);
    lv_chart_set_div_line_count(ui_ManualBrewScreen_chart, 0, 0);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_size(ui_ManualBrewScreen_chart, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui_ManualBrewScreen_chart, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_ManualBrewScreen_chart, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_ManualBrewScreen_chart, LV_OBJ_FLAG_CLICKABLE);

    ui_ManualBrewScreen_chart_pressure = lv_chart_add_series(ui_ManualBrewScreen_chart,
        lv_color_hex(0x00FFFF), LV_CHART_AXIS_PRIMARY_Y);
    ui_ManualBrewScreen_chart_temp = lv_chart_add_series(ui_ManualBrewScreen_chart,
        lv_color_hex(0xFF8C00), LV_CHART_AXIS_PRIMARY_Y);
    ui_ManualBrewScreen_chart_flow = lv_chart_add_series(ui_ManualBrewScreen_chart,
        lv_color_hex(0x00CC44), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(ui_ManualBrewScreen_chart, ui_ManualBrewScreen_chart_pressure, LV_CHART_POINT_NONE);
    lv_chart_set_all_value(ui_ManualBrewScreen_chart, ui_ManualBrewScreen_chart_temp, LV_CHART_POINT_NONE);
    lv_chart_set_all_value(ui_ManualBrewScreen_chart, ui_ManualBrewScreen_chart_flow, LV_CHART_POINT_NONE);

    ui_ManualBrewScreen_pressureCircle = lv_obj_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_set_width(ui_ManualBrewScreen_pressureCircle, 110);
    lv_obj_set_height(ui_ManualBrewScreen_pressureCircle, 110);
    lv_obj_set_align(ui_ManualBrewScreen_pressureCircle, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_pressureCircle, 0);
    lv_obj_set_y(ui_ManualBrewScreen_pressureCircle, -6);
    lv_obj_clear_flag(ui_ManualBrewScreen_pressureCircle, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(ui_ManualBrewScreen_pressureCircle, 55, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen_pressureCircle, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_pressureCircle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ManualBrewScreen_pressureCircle, lv_color_hex(0x00FFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_pressureCircle, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_pressureTarget = lv_label_create(ui_ManualBrewScreen_pressureCircle);
    lv_obj_set_width(ui_ManualBrewScreen_pressureTarget, 106);
    lv_obj_set_style_text_align(ui_ManualBrewScreen_pressureTarget, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align(ui_ManualBrewScreen_pressureTarget, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_pressureTarget, 0);
    lv_obj_set_y(ui_ManualBrewScreen_pressureTarget, 0);
    lv_label_set_text(ui_ManualBrewScreen_pressureTarget, "0.0");
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_pressureTarget, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_pressureTarget, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_pressureTarget, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_bottomRow = lv_obj_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_remove_style_all(ui_ManualBrewScreen_bottomRow);
    lv_obj_set_width(ui_ManualBrewScreen_bottomRow, 270);
    lv_obj_set_height(ui_ManualBrewScreen_bottomRow, 22);
    lv_obj_set_align(ui_ManualBrewScreen_bottomRow, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_bottomRow, 0);
    lv_obj_set_y(ui_ManualBrewScreen_bottomRow, 78);
    lv_obj_set_flex_flow(ui_ManualBrewScreen_bottomRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_ManualBrewScreen_bottomRow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ManualBrewScreen_bottomRow, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    ui_ManualBrewScreen_targetTemp = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_label_set_text(ui_ManualBrewScreen_targetTemp, "95\xC2\xB0""C");
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_targetTemp, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_elapsed = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_label_set_text(ui_ManualBrewScreen_elapsed, "00:00");
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_elapsed, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_elapsed, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_elapsed, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_flow = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_label_set_text(ui_ManualBrewScreen_flow, "--ml/s");
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_flow, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_flow, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_color_NiceWhite);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_flow, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_weight = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_label_set_text(ui_ManualBrewScreen_weight, "--g");
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_weight, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_weight, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_weight, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_startButton = lv_imgbtn_create(ui_ManualBrewScreen);
    lv_imgbtn_set_src(ui_ManualBrewScreen_startButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_width(ui_ManualBrewScreen_startButton, 40);
    lv_obj_set_height(ui_ManualBrewScreen_startButton, 40);
    lv_obj_set_align(ui_ManualBrewScreen_startButton, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_startButton, 0);
    lv_obj_set_y(ui_ManualBrewScreen_startButton, 130);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ManualBrewScreen_savePanel = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_set_width(ui_ManualBrewScreen_savePanel, 480);
    lv_obj_set_height(ui_ManualBrewScreen_savePanel, 480);
    lv_obj_set_align(ui_ManualBrewScreen_savePanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ManualBrewScreen_savePanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_ManualBrewScreen_savePanel, 480, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen_savePanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_savePanel, 230, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_savePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_ManualBrewScreen_savePanel, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *savePrompt = lv_label_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_align(savePrompt, LV_ALIGN_CENTER);
    lv_obj_set_x(savePrompt, 0);
    lv_obj_set_y(savePrompt, -50);
    lv_label_set_text(savePrompt, "Save this shot?");
    ui_object_set_themeable_style_property(savePrompt, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(savePrompt, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_text_font(savePrompt, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_saveNameLabel = lv_label_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_align(ui_ManualBrewScreen_saveNameLabel, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_saveNameLabel, 0);
    lv_obj_set_y(ui_ManualBrewScreen_saveNameLabel, -15);
    lv_label_set_text(ui_ManualBrewScreen_saveNameLabel, "Manual 1");
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_saveNameLabel, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_saveNameLabel, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_saveNameLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ManualBrewScreen_saveButton = lv_btn_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_width(ui_ManualBrewScreen_saveButton, 100);
    lv_obj_set_height(ui_ManualBrewScreen_saveButton, 44);
    lv_obj_set_align(ui_ManualBrewScreen_saveButton, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_saveButton, -65);
    lv_obj_set_y(ui_ManualBrewScreen_saveButton, 40);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen_saveButton, lv_color_hex(0x007700), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *lbl = lv_label_create(ui_ManualBrewScreen_saveButton);
        lv_label_set_text(lbl, "Save");
        lv_obj_center(lbl);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    ui_ManualBrewScreen_discardButton = lv_btn_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_width(ui_ManualBrewScreen_discardButton, 100);
    lv_obj_set_height(ui_ManualBrewScreen_discardButton, 44);
    lv_obj_set_align(ui_ManualBrewScreen_discardButton, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_ManualBrewScreen_discardButton, 65);
    lv_obj_set_y(ui_ManualBrewScreen_discardButton, 40);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen_discardButton, lv_color_hex(0x770000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *lbl = lv_label_create(ui_ManualBrewScreen_discardButton);
        lv_label_set_text(lbl, "Discard");
        lv_obj_center(lbl);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    lv_obj_add_event_cb(ui_ManualBrewScreen_tempZone, ui_event_ManualBrewScreen_tempZone, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_pressureZone, ui_event_ManualBrewScreen_pressureZone, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_backButton, ui_event_ManualBrewScreen_backButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_startButton, ui_event_ManualBrewScreen_startButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_saveButton, ui_event_ManualBrewScreen_saveButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_discardButton, ui_event_ManualBrewScreen_discardButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen, ui_event_ManualBrewScreen, LV_EVENT_ALL, NULL);

    lv_obj_move_foreground(ui_ManualBrewScreen_tempZone);
    lv_obj_move_foreground(ui_ManualBrewScreen_pressureZone);
    lv_obj_move_foreground(ui_ManualBrewScreen_backButton);
    lv_obj_move_foreground(ui_ManualBrewScreen_startButton);
    lv_obj_move_foreground(ui_ManualBrewScreen_savePanel);

    uic_ManualBrewScreen_dials_tempGauge = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_ManualBrewScreen_dials_tempTarget = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_ManualBrewScreen_dials_pressureGauge = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_ManualBrewScreen_dials_pressureTarget = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_ManualBrewScreen_dials_pressureText = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_ManualBrewScreen_dials_tempText = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_TEMPTEXT);

}


void ui_ManualBrewScreen_screen_destroy(void) {
    if (ui_ManualBrewScreen)
        lv_obj_del(ui_ManualBrewScreen);

    ui_ManualBrewScreen = NULL;
    ui_ManualBrewScreen_dials = NULL;
    ui_ManualBrewScreen_tempZone = NULL;
    ui_ManualBrewScreen_pressureZone = NULL;
    uic_ManualBrewScreen_dials_tempGauge = NULL;
    uic_ManualBrewScreen_dials_tempTarget = NULL;
    uic_ManualBrewScreen_dials_pressureGauge = NULL;
    uic_ManualBrewScreen_dials_pressureTarget = NULL;
    uic_ManualBrewScreen_dials_pressureText = NULL;
    uic_ManualBrewScreen_dials_tempText = NULL;
    ui_ManualBrewScreen_backButton = NULL;
    ui_ManualBrewScreen_contentPanel = NULL;
    ui_ManualBrewScreen_mainLabel = NULL;
    ui_ManualBrewScreen_chart = NULL;
    ui_ManualBrewScreen_chart_pressure = NULL;
    ui_ManualBrewScreen_chart_temp = NULL;
    ui_ManualBrewScreen_chart_flow = NULL;
    ui_ManualBrewScreen_pressureCircle = NULL;
    ui_ManualBrewScreen_pressureTarget = NULL;
    ui_ManualBrewScreen_bottomRow = NULL;
    ui_ManualBrewScreen_targetTemp = NULL;
    ui_ManualBrewScreen_elapsed = NULL;
    ui_ManualBrewScreen_flow = NULL;
    ui_ManualBrewScreen_weight = NULL;
    ui_ManualBrewScreen_startButton = NULL;
    ui_ManualBrewScreen_savePanel = NULL;
    ui_ManualBrewScreen_saveNameLabel = NULL;
    ui_ManualBrewScreen_saveButton = NULL;
    ui_ManualBrewScreen_discardButton = NULL;
}