// GaggiBre — 4.3" profile settings screen (800×480)
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_ProfileSettingsScreen_screen_init_43(void) {
    ui_ProfileSettingsScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ProfileSettingsScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED,
                        ui_ProfileSettingsScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Dark);

    ui_ProfileSettingsScreen_dials = ui_dials43_create(ui_ProfileSettingsScreen);

    ui_ProfileSettingsScreen_backButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_backButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_backButton, 40, 40);
    lv_obj_set_align(ui_ProfileSettingsScreen_backButton, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_backButton, 0, 14);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_titleLabel = lv_label_create(ui_ProfileSettingsScreen);
    lv_obj_set_size(ui_ProfileSettingsScreen_titleLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ProfileSettingsScreen_titleLabel, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_titleLabel, 0, 70);
    lv_label_set_text(ui_ProfileSettingsScreen_titleLabel, "Edit profile");
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_titleLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_titleLabel, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_titleLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_profileName = lv_label_create(ui_ProfileSettingsScreen);
    lv_obj_set_size(ui_ProfileSettingsScreen_profileName, 560, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ProfileSettingsScreen_profileName, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_profileName, 0, 110);
    lv_label_set_long_mode(ui_ProfileSettingsScreen_profileName, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_ProfileSettingsScreen_profileName, "Profile");
    lv_obj_set_style_text_align(ui_ProfileSettingsScreen_profileName, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_profileName, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_adaptiveLabel = lv_label_create(ui_ProfileSettingsScreen);
    lv_obj_set_size(ui_ProfileSettingsScreen_adaptiveLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ProfileSettingsScreen_adaptiveLabel, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_adaptiveLabel, 0, 165);
    lv_label_set_text(ui_ProfileSettingsScreen_adaptiveLabel, "Adaptive OFF");
    lv_obj_add_flag(ui_ProfileSettingsScreen_adaptiveLabel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_adaptiveLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_ProfileSettingsScreen_adaptiveLabel, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_ProfileSettingsScreen_adaptiveLabel, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_ProfileSettingsScreen_adaptiveLabel, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_ProfileSettingsScreen_adaptiveLabel, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_ProfileSettingsScreen_adaptiveLabel, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ProfileSettingsScreen_adaptiveLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_ProfileSettingsScreen_adaptiveLabel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ProfileSettingsScreen_adaptiveLabel, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_adaptiveLabel, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_adaptiveLabel, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_adjustments = lv_obj_create(ui_ProfileSettingsScreen);
    lv_obj_remove_style_all(ui_ProfileSettingsScreen_adjustments);
    lv_obj_set_size(ui_ProfileSettingsScreen_adjustments, 560, 140);
    lv_obj_set_align(ui_ProfileSettingsScreen_adjustments, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_adjustments, 0, 220);
    lv_obj_set_flex_flow(ui_ProfileSettingsScreen_adjustments, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_ProfileSettingsScreen_adjustments, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ProfileSettingsScreen_adjustments, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tempRow = lv_obj_create(ui_ProfileSettingsScreen_adjustments);
    lv_obj_remove_style_all(tempRow);
    lv_obj_set_size(tempRow, 560, 50);
    lv_obj_clear_flag(tempRow, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tempIcon = lv_img_create(tempRow);
    lv_img_set_src(tempIcon, &ui_img_1951499226);
    lv_obj_set_size(tempIcon, 40, 40);
    lv_obj_set_align(tempIcon, LV_ALIGN_LEFT_MID);
    ui_object_set_themeable_style_property(tempIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,
                                           _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(tempIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_downTempButton = lv_imgbtn_create(tempRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_downTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_downTempButton, 44, 44);
    lv_obj_set_align(ui_ProfileSettingsScreen_downTempButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileSettingsScreen_downTempButton, -80, 0);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_targetTemp = lv_label_create(tempRow);
    lv_obj_set_size(ui_ProfileSettingsScreen_targetTemp, 120, 40);
    lv_obj_set_align(ui_ProfileSettingsScreen_targetTemp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ProfileSettingsScreen_targetTemp, "93\xC2\xB0"
                                                           "C");
    lv_obj_set_style_text_align(ui_ProfileSettingsScreen_targetTemp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_targetTemp, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_upTempButton = lv_imgbtn_create(tempRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_upTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_upTempButton, 44, 44);
    lv_obj_set_align(ui_ProfileSettingsScreen_upTempButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileSettingsScreen_upTempButton, 80, 0);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    lv_obj_t *durationRow = lv_obj_create(ui_ProfileSettingsScreen_adjustments);
    lv_obj_remove_style_all(durationRow);
    lv_obj_set_size(durationRow, 560, 50);
    lv_obj_clear_flag(durationRow, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *durationIcon = lv_img_create(durationRow);
    lv_img_set_src(durationIcon, &ui_img_360122106);
    lv_obj_set_size(durationIcon, 40, 40);
    lv_obj_set_align(durationIcon, LV_ALIGN_LEFT_MID);
    ui_object_set_themeable_style_property(durationIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,
                                           _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(durationIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_downDurationButton = lv_imgbtn_create(durationRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_downDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_downDurationButton, 44, 44);
    lv_obj_set_align(ui_ProfileSettingsScreen_downDurationButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileSettingsScreen_downDurationButton, -80, 0);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_targetDuration = lv_label_create(durationRow);
    lv_obj_set_size(ui_ProfileSettingsScreen_targetDuration, 120, 40);
    lv_obj_set_align(ui_ProfileSettingsScreen_targetDuration, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ProfileSettingsScreen_targetDuration, "0:30");
    lv_obj_set_style_text_align(ui_ProfileSettingsScreen_targetDuration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_targetDuration, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_upDurationButton = lv_imgbtn_create(durationRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_upDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_upDurationButton, 44, 44);
    lv_obj_set_align(ui_ProfileSettingsScreen_upDurationButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_ProfileSettingsScreen_upDurationButton, 80, 0);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_saveButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_saveButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1594943393, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_saveButton, 40, 40);
    lv_obj_set_align(ui_ProfileSettingsScreen_saveButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_saveButton, -90, -30);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_acceptButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_acceptButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_631115820, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_acceptButton, 44, 44);
    lv_obj_set_align(ui_ProfileSettingsScreen_acceptButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_acceptButton, 0, -28);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_saveAsNewButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_saveAsNewButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1464184441, NULL);
    lv_obj_set_size(ui_ProfileSettingsScreen_saveAsNewButton, 40, 40);
    lv_obj_set_align(ui_ProfileSettingsScreen_saveAsNewButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_ProfileSettingsScreen_saveAsNewButton, 90, -30);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_saveAsNewButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_saveAsNewButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    lv_obj_add_event_cb(ui_ProfileSettingsScreen, ui_event_ProfileSettingsScreen, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_backButton, ui_event_ProfileSettingsScreen_backButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_downTempButton, ui_event_ProfileSettingsScreen_downTempButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_upTempButton, ui_event_ProfileSettingsScreen_upTempButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_downDurationButton, ui_event_ProfileSettingsScreen_downDurationButton, LV_EVENT_ALL,
                        NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_upDurationButton, ui_event_ProfileSettingsScreen_upDurationButton, LV_EVENT_ALL,
                        NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_saveButton, ui_event_ProfileSettingsScreen_saveButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_acceptButton, ui_event_ProfileSettingsScreen_acceptButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_saveAsNewButton, ui_event_ProfileSettingsScreen_saveAsNewButton, LV_EVENT_ALL,
                        NULL);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen_adaptiveLabel, ui_event_ProfileSettingsScreen_adaptiveLabel, LV_EVENT_ALL, NULL);

    uic_ProfileSettingsScreen_dials_tempGauge = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_ProfileSettingsScreen_dials_tempTarget = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_ProfileSettingsScreen_dials_pressureGauge = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_ProfileSettingsScreen_dials_pressureTarget = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_ProfileSettingsScreen_dials_pressureText = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_ProfileSettingsScreen_dials_tempText = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
