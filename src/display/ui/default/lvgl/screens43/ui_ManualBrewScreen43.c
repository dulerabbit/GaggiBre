// GaggiMate — 4.3" Manual Brew Screen (800×480)
// Layout: 180px sidebar (top half = temp zone, bottom half = pressure zone)
//         618px main content (chart + pressure display + controls)
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_ManualBrewScreen_screen_init_43(void) {
    ui_ManualBrewScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ManualBrewScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_ManualBrewScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_ManualBrewScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // ── Sidebar dials ──────────────────────────────────────────────────────
    ui_ManualBrewScreen_dials = ui_dials43_create(ui_ManualBrewScreen);

    // ── Drag zones — side-column based (left column = temp, right column = pressure)
    // Left column full height: temperature (swipe up/down)
    ui_ManualBrewScreen_tempZone = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_remove_style_all(ui_ManualBrewScreen_tempZone);
    lv_obj_set_size(ui_ManualBrewScreen_tempZone, 60, 480);
    lv_obj_set_pos(ui_ManualBrewScreen_tempZone, 0, 0);
    lv_obj_add_flag(ui_ManualBrewScreen_tempZone, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ManualBrewScreen_tempZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_tempZone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Right column full height: pressure (swipe up/down)
    ui_ManualBrewScreen_pressureZone = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_remove_style_all(ui_ManualBrewScreen_pressureZone);
    lv_obj_set_size(ui_ManualBrewScreen_pressureZone, 60, 480);
    lv_obj_set_pos(ui_ManualBrewScreen_pressureZone, 740, 0);
    lv_obj_add_flag(ui_ManualBrewScreen_pressureZone, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ManualBrewScreen_pressureZone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_pressureZone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── Main content panel (800×480, full-width) ───────────────────────────
    ui_ManualBrewScreen_contentPanel = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_remove_style_all(ui_ManualBrewScreen_contentPanel);
    lv_obj_set_size(ui_ManualBrewScreen_contentPanel, 800, 480);
    lv_obj_set_pos(ui_ManualBrewScreen_contentPanel, 0, 0);
    lv_obj_clear_flag(ui_ManualBrewScreen_contentPanel, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_contentPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Back button — centred at top (overhaul will restyle to bare icon)
    ui_ManualBrewScreen_backButton = lv_imgbtn_create(ui_ManualBrewScreen_contentPanel);
    lv_imgbtn_set_src(ui_ManualBrewScreen_backButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_ManualBrewScreen_backButton, 54, 54);
    lv_obj_set_align(ui_ManualBrewScreen_backButton, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ManualBrewScreen_backButton, 0, 14);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Hidden label (not shown on 43" — keep var valid)
    ui_ManualBrewScreen_mainLabel = lv_label_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_set_size(ui_ManualBrewScreen_mainLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_ManualBrewScreen_mainLabel, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_ManualBrewScreen_mainLabel, "Manual");
    lv_obj_set_style_text_font(ui_ManualBrewScreen_mainLabel, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_mainLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_mainLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Live chart (580×200, top of content panel) ─────────────────────────
    ui_ManualBrewScreen_chart = lv_chart_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_set_size(ui_ManualBrewScreen_chart, 580, 220);
    lv_obj_set_align(ui_ManualBrewScreen_chart, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ManualBrewScreen_chart, 0, 66);
    lv_obj_clear_flag(ui_ManualBrewScreen_chart, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_chart_set_type(ui_ManualBrewScreen_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui_ManualBrewScreen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 160);
    lv_chart_set_point_count(ui_ManualBrewScreen_chart, 120);
    lv_chart_set_div_line_count(ui_ManualBrewScreen_chart, 3, 5);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_chart, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_ManualBrewScreen_chart, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_ManualBrewScreen_chart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_size(ui_ManualBrewScreen_chart, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Pressure series (cyan)
    ui_ManualBrewScreen_chart_pressure = lv_chart_add_series(
        ui_ManualBrewScreen_chart, lv_color_hex(0x2CA4F6), LV_CHART_AXIS_PRIMARY_Y);
    // Temperature series (orange)
    ui_ManualBrewScreen_chart_temp = lv_chart_add_series(
        ui_ManualBrewScreen_chart, lv_color_hex(0xF6812C), LV_CHART_AXIS_PRIMARY_Y);
    // Flow series (green)
    ui_ManualBrewScreen_chart_flow = lv_chart_add_series(
        ui_ManualBrewScreen_chart, lv_color_hex(0x2CF67C), LV_CHART_AXIS_PRIMARY_Y);

    // Line widths
    lv_obj_set_style_line_width(ui_ManualBrewScreen_chart, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);

    // ── Pressure circle (left of centre, lower half) ──────────────────────
    ui_ManualBrewScreen_pressureCircle = lv_obj_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_set_size(ui_ManualBrewScreen_pressureCircle, 120, 120);
    lv_obj_set_align(ui_ManualBrewScreen_pressureCircle, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ManualBrewScreen_pressureCircle, -150, 110);
    lv_obj_clear_flag(ui_ManualBrewScreen_pressureCircle, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(ui_ManualBrewScreen_pressureCircle, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_pressureCircle, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ManualBrewScreen_pressureCircle, lv_color_hex(0x2CA4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen_pressureCircle, lv_color_hex(0x0A0A0A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_pressureCircle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_ManualBrewScreen_pressureCircle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Pressure target number inside circle
    ui_ManualBrewScreen_pressureTarget = lv_label_create(ui_ManualBrewScreen_pressureCircle);
    lv_obj_set_size(ui_ManualBrewScreen_pressureTarget, 100, LV_SIZE_CONTENT);
    lv_obj_center(ui_ManualBrewScreen_pressureTarget);
    lv_label_set_text(ui_ManualBrewScreen_pressureTarget, "0.0");
    lv_obj_set_style_text_align(ui_ManualBrewScreen_pressureTarget, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_pressureTarget, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_pressureTarget, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_pressureTarget, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Bottom info row (temp | time | flow | weight) ──────────────────────
    ui_ManualBrewScreen_bottomRow = lv_obj_create(ui_ManualBrewScreen_contentPanel);
    lv_obj_remove_style_all(ui_ManualBrewScreen_bottomRow);
    lv_obj_set_size(ui_ManualBrewScreen_bottomRow, 580, 36);
    lv_obj_set_align(ui_ManualBrewScreen_bottomRow, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_ManualBrewScreen_bottomRow, 0, -8);
    lv_obj_set_flex_flow(ui_ManualBrewScreen_bottomRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_ManualBrewScreen_bottomRow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ManualBrewScreen_bottomRow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_ManualBrewScreen_targetTemp = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_obj_set_size(ui_ManualBrewScreen_targetTemp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ManualBrewScreen_targetTemp, "93°C");
    lv_obj_set_style_text_font(ui_ManualBrewScreen_targetTemp, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_ManualBrewScreen_elapsed = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_obj_set_size(ui_ManualBrewScreen_elapsed, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ManualBrewScreen_elapsed, "0:00");
    lv_obj_set_style_text_font(ui_ManualBrewScreen_elapsed, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_elapsed, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_elapsed, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_ManualBrewScreen_flow = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_obj_set_size(ui_ManualBrewScreen_flow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ManualBrewScreen_flow, "0.0 ml/s");
    lv_obj_set_style_text_font(ui_ManualBrewScreen_flow, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_flow, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_flow, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_ManualBrewScreen_weight = lv_label_create(ui_ManualBrewScreen_bottomRow);
    lv_obj_set_size(ui_ManualBrewScreen_weight, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_ManualBrewScreen_weight, "0.0g");
    lv_obj_set_style_text_font(ui_ManualBrewScreen_weight, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_weight, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_weight, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Start / stop button ────────────────────────────────────────────────
    ui_ManualBrewScreen_startButton = lv_imgbtn_create(ui_ManualBrewScreen_contentPanel);
    lv_imgbtn_set_src(ui_ManualBrewScreen_startButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_ManualBrewScreen_startButton, 60, 60);
    lv_obj_set_align(ui_ManualBrewScreen_startButton, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_pos(ui_ManualBrewScreen_startButton, -14, -50);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Save overlay (full screen, dark translucent) ──────────────────────
    ui_ManualBrewScreen_savePanel = lv_obj_create(ui_ManualBrewScreen);
    lv_obj_set_size(ui_ManualBrewScreen_savePanel, 800, 480);
    lv_obj_set_pos(ui_ManualBrewScreen_savePanel, 0, 0);
    lv_obj_clear_flag(ui_ManualBrewScreen_savePanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_ManualBrewScreen_savePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ManualBrewScreen_savePanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_savePanel, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_savePanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_ManualBrewScreen_savePanel, LV_OBJ_FLAG_HIDDEN);

    // Save prompt label
    lv_obj_t *savePrompt = lv_label_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_size(savePrompt, 400, LV_SIZE_CONTENT);
    lv_obj_set_align(savePrompt, LV_ALIGN_CENTER);
    lv_obj_set_pos(savePrompt, 0, -80);
    lv_label_set_text(savePrompt, "Save as new profile?");
    lv_obj_set_style_text_align(savePrompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(savePrompt, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(savePrompt, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(savePrompt, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Profile name label
    ui_ManualBrewScreen_saveNameLabel = lv_label_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_size(ui_ManualBrewScreen_saveNameLabel, 400, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ManualBrewScreen_saveNameLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ManualBrewScreen_saveNameLabel, 0, -30);
    lv_label_set_text(ui_ManualBrewScreen_saveNameLabel, "Manual 1");
    lv_obj_set_style_text_align(ui_ManualBrewScreen_saveNameLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ManualBrewScreen_saveNameLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_saveNameLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_saveNameLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Save button
    ui_ManualBrewScreen_saveButton = lv_btn_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_size(ui_ManualBrewScreen_saveButton, 120, 50);
    lv_obj_set_align(ui_ManualBrewScreen_saveButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ManualBrewScreen_saveButton, 70, 60);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ManualBrewScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_NiceWhite);
    lv_obj_t *saveLbl = lv_label_create(ui_ManualBrewScreen_saveButton);
    lv_obj_center(saveLbl);
    lv_label_set_text(saveLbl, "Save");
    lv_obj_set_style_text_font(saveLbl, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Discard button
    ui_ManualBrewScreen_discardButton = lv_btn_create(ui_ManualBrewScreen_savePanel);
    lv_obj_set_size(ui_ManualBrewScreen_discardButton, 120, 50);
    lv_obj_set_align(ui_ManualBrewScreen_discardButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ManualBrewScreen_discardButton, -70, 60);
    lv_obj_set_style_bg_opa(ui_ManualBrewScreen_discardButton, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ManualBrewScreen_discardButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ManualBrewScreen_discardButton, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *discardLbl = lv_label_create(ui_ManualBrewScreen_discardButton);
    lv_obj_center(discardLbl);
    lv_label_set_text(discardLbl, "Discard");
    lv_obj_set_style_text_font(discardLbl, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(discardLbl, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(discardLbl, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Z-order: zones and buttons must be above content panel ─────────────
    lv_obj_move_foreground(ui_ManualBrewScreen_tempZone);
    lv_obj_move_foreground(ui_ManualBrewScreen_pressureZone);
    lv_obj_move_foreground(ui_ManualBrewScreen_savePanel);

    // ── Events ────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_ManualBrewScreen,                  ui_event_ManualBrewScreen,               LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_startButton,      ui_event_ManualBrewScreen_startButton,   LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_backButton,       ui_event_ManualBrewScreen_backButton,    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_saveButton,       ui_event_ManualBrewScreen_saveButton,    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_discardButton,    ui_event_ManualBrewScreen_discardButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_tempZone,         ui_event_ManualBrewScreen_tempZone,      LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ManualBrewScreen_pressureZone,     ui_event_ManualBrewScreen_pressureZone,  LV_EVENT_ALL, NULL);

    // ── Cache component child pointers ────────────────────────────────────
    uic_ManualBrewScreen_dials_tempGauge      = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_ManualBrewScreen_dials_tempTarget     = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_ManualBrewScreen_dials_pressureGauge  = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_ManualBrewScreen_dials_pressureTarget = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_ManualBrewScreen_dials_pressureText   = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_ManualBrewScreen_dials_tempText       = ui_comp_get_child(ui_ManualBrewScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
