// GaggiBre — round profile settings screen (480×480)
#include "../ui.h"

lv_obj_t *ui_ProfileSettingsScreen = NULL;
lv_obj_t *ui_ProfileSettingsScreen_dials = NULL;
lv_obj_t *ui_ProfileSettingsScreen_backButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_titleLabel = NULL;
lv_obj_t *ui_ProfileSettingsScreen_profileName = NULL;
lv_obj_t *ui_ProfileSettingsScreen_adaptiveLabel = NULL;
lv_obj_t *ui_ProfileSettingsScreen_adjustments = NULL;
lv_obj_t *ui_ProfileSettingsScreen_targetTemp = NULL;
lv_obj_t *ui_ProfileSettingsScreen_downTempButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_upTempButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_targetDuration = NULL;
lv_obj_t *ui_ProfileSettingsScreen_downDurationButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_upDurationButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_saveButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_acceptButton = NULL;
lv_obj_t *ui_ProfileSettingsScreen_saveAsNewButton = NULL;

lv_obj_t *uic_ProfileSettingsScreen_dials_tempGauge = NULL;
lv_obj_t *uic_ProfileSettingsScreen_dials_tempTarget = NULL;
lv_obj_t *uic_ProfileSettingsScreen_dials_pressureGauge = NULL;
lv_obj_t *uic_ProfileSettingsScreen_dials_pressureTarget = NULL;
lv_obj_t *uic_ProfileSettingsScreen_dials_pressureText = NULL;
lv_obj_t *uic_ProfileSettingsScreen_dials_tempText = NULL;

void ui_event_ProfileSettingsScreen(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_SCREEN_LOADED) {
        // no-op; DefaultUI reactive effects populate labels
    }
}

void ui_event_ProfileSettingsScreen_backButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onProfileAccept(e);
    }
}

void ui_event_ProfileSettingsScreen_downTempButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onBrewTempLower(e);
    }
}

void ui_event_ProfileSettingsScreen_upTempButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onBrewTempRaise(e);
    }
}

void ui_event_ProfileSettingsScreen_downDurationButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onBrewTimeLower(e);
    }
}

void ui_event_ProfileSettingsScreen_upDurationButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onBrewTimeRaise(e);
    }
}

void ui_event_ProfileSettingsScreen_saveButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onProfileSave(e);
    }
}

void ui_event_ProfileSettingsScreen_acceptButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onProfileAccept(e);
    }
}

void ui_event_ProfileSettingsScreen_saveAsNewButton(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onProfileSaveAsNew(e);
    }
}

void ui_event_ProfileSettingsScreen_adaptiveLabel(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        onSelectedProfileAdaptiveToggle(e);
    }
}

void ui_ProfileSettingsScreen_screen_init(void) {
    ui_ProfileSettingsScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ProfileSettingsScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_ProfileSettingsScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED,
                        ui_ProfileSettingsScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Dark);

    ui_ProfileSettingsScreen_dials = ui_dials_create(ui_ProfileSettingsScreen);

    ui_ProfileSettingsScreen_backButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_backButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_backButton, 40);
    lv_obj_set_height(ui_ProfileSettingsScreen_backButton, 40);
    lv_obj_set_x(ui_ProfileSettingsScreen_backButton, 0);
    lv_obj_set_y(ui_ProfileSettingsScreen_backButton, -200);
    lv_obj_set_align(ui_ProfileSettingsScreen_backButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_backButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_titleLabel = lv_label_create(ui_ProfileSettingsScreen);
    lv_obj_set_width(ui_ProfileSettingsScreen_titleLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ProfileSettingsScreen_titleLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_ProfileSettingsScreen_titleLabel, 0);
    lv_obj_set_y(ui_ProfileSettingsScreen_titleLabel, -155);
    lv_obj_set_align(ui_ProfileSettingsScreen_titleLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ProfileSettingsScreen_titleLabel, "Edit profile");
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_titleLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_titleLabel, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_titleLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_profileName = lv_label_create(ui_ProfileSettingsScreen);
    lv_obj_set_width(ui_ProfileSettingsScreen_profileName, 280);
    lv_obj_set_height(ui_ProfileSettingsScreen_profileName, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_ProfileSettingsScreen_profileName, 0);
    lv_obj_set_y(ui_ProfileSettingsScreen_profileName, -120);
    lv_obj_set_align(ui_ProfileSettingsScreen_profileName, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_ProfileSettingsScreen_profileName, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_ProfileSettingsScreen_profileName, "Profile");
    lv_obj_set_style_text_align(ui_ProfileSettingsScreen_profileName, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_profileName, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_adaptiveLabel = lv_label_create(ui_ProfileSettingsScreen);
    lv_label_set_text(ui_ProfileSettingsScreen_adaptiveLabel, "");
    lv_obj_add_flag(ui_ProfileSettingsScreen_adaptiveLabel, LV_OBJ_FLAG_HIDDEN);

    ui_ProfileSettingsScreen_adjustments = lv_obj_create(ui_ProfileSettingsScreen);
    lv_obj_remove_style_all(ui_ProfileSettingsScreen_adjustments);
    lv_obj_set_width(ui_ProfileSettingsScreen_adjustments, 360);
    lv_obj_set_height(ui_ProfileSettingsScreen_adjustments, 120);
    lv_obj_set_x(ui_ProfileSettingsScreen_adjustments, 0);
    lv_obj_set_y(ui_ProfileSettingsScreen_adjustments, 10);
    lv_obj_set_align(ui_ProfileSettingsScreen_adjustments, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_ProfileSettingsScreen_adjustments, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_ProfileSettingsScreen_adjustments, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ProfileSettingsScreen_adjustments, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(ui_ProfileSettingsScreen_adjustments, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *tempRow = lv_obj_create(ui_ProfileSettingsScreen_adjustments);
    lv_obj_remove_style_all(tempRow);
    lv_obj_set_width(tempRow, 360);
    lv_obj_set_height(tempRow, 50);
    lv_obj_clear_flag(tempRow, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tempIcon = lv_img_create(tempRow);
    lv_img_set_src(tempIcon, &ui_img_1951499226);
    lv_obj_set_width(tempIcon, 40);
    lv_obj_set_height(tempIcon, 40);
    lv_obj_set_x(tempIcon, -140);
    lv_obj_set_align(tempIcon, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(tempIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,
                                           _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(tempIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_downTempButton = lv_imgbtn_create(tempRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_downTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_downTempButton, 40);
    lv_obj_set_height(ui_ProfileSettingsScreen_downTempButton, 40);
    lv_obj_set_x(ui_ProfileSettingsScreen_downTempButton, -60);
    lv_obj_set_align(ui_ProfileSettingsScreen_downTempButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_targetTemp = lv_label_create(tempRow);
    lv_obj_set_width(ui_ProfileSettingsScreen_targetTemp, 100);
    lv_obj_set_height(ui_ProfileSettingsScreen_targetTemp, 40);
    lv_obj_set_align(ui_ProfileSettingsScreen_targetTemp, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ProfileSettingsScreen_targetTemp, "93°C");
    lv_obj_set_style_text_align(ui_ProfileSettingsScreen_targetTemp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ProfileSettingsScreen_targetTemp, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_upTempButton = lv_imgbtn_create(tempRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_upTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_upTempButton, 40);
    lv_obj_set_height(ui_ProfileSettingsScreen_upTempButton, 40);
    lv_obj_set_x(ui_ProfileSettingsScreen_upTempButton, 60);
    lv_obj_set_align(ui_ProfileSettingsScreen_upTempButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    lv_obj_t *durationRow = lv_obj_create(ui_ProfileSettingsScreen_adjustments);
    lv_obj_remove_style_all(durationRow);
    lv_obj_set_width(durationRow, 360);
    lv_obj_set_height(durationRow, 50);
    lv_obj_clear_flag(durationRow, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *durationIcon = lv_img_create(durationRow);
    lv_img_set_src(durationIcon, &ui_img_360122106);
    lv_obj_set_width(durationIcon, 40);
    lv_obj_set_height(durationIcon, 40);
    lv_obj_set_x(durationIcon, -140);
    lv_obj_set_align(durationIcon, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(durationIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,
                                           _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(durationIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA,
                                           _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_downDurationButton = lv_imgbtn_create(durationRow);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_downDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_downDurationButton, 40);
    lv_obj_set_height(ui_ProfileSettingsScreen_downDurationButton, 40);
    lv_obj_set_x(ui_ProfileSettingsScreen_downDurationButton, -60);
    lv_obj_set_align(ui_ProfileSettingsScreen_downDurationButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_targetDuration = lv_label_create(durationRow);
    lv_obj_set_width(ui_ProfileSettingsScreen_targetDuration, 100);
    lv_obj_set_height(ui_ProfileSettingsScreen_targetDuration, 40);
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
    lv_obj_set_width(ui_ProfileSettingsScreen_upDurationButton, 40);
    lv_obj_set_height(ui_ProfileSettingsScreen_upDurationButton, 40);
    lv_obj_set_x(ui_ProfileSettingsScreen_upDurationButton, 60);
    lv_obj_set_align(ui_ProfileSettingsScreen_upDurationButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_saveButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_saveButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1594943393, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_saveButton, 30);
    lv_obj_set_height(ui_ProfileSettingsScreen_saveButton, 30);
    lv_obj_set_x(ui_ProfileSettingsScreen_saveButton, -70);
    lv_obj_set_y(ui_ProfileSettingsScreen_saveButton, 130);
    lv_obj_set_align(ui_ProfileSettingsScreen_saveButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_acceptButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_acceptButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_631115820, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_acceptButton, 40);
    lv_obj_set_height(ui_ProfileSettingsScreen_acceptButton, 40);
    lv_obj_set_x(ui_ProfileSettingsScreen_acceptButton, 0);
    lv_obj_set_y(ui_ProfileSettingsScreen_acceptButton, 130);
    lv_obj_set_align(ui_ProfileSettingsScreen_acceptButton, LV_ALIGN_CENTER);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_ProfileSettingsScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT,
                                           LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_ProfileSettingsScreen_saveAsNewButton = lv_imgbtn_create(ui_ProfileSettingsScreen);
    lv_imgbtn_set_src(ui_ProfileSettingsScreen_saveAsNewButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1464184441, NULL);
    lv_obj_set_width(ui_ProfileSettingsScreen_saveAsNewButton, 30);
    lv_obj_set_height(ui_ProfileSettingsScreen_saveAsNewButton, 30);
    lv_obj_set_x(ui_ProfileSettingsScreen_saveAsNewButton, 70);
    lv_obj_set_y(ui_ProfileSettingsScreen_saveAsNewButton, 130);
    lv_obj_set_align(ui_ProfileSettingsScreen_saveAsNewButton, LV_ALIGN_CENTER);
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

    uic_ProfileSettingsScreen_dials_tempGauge = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_ProfileSettingsScreen_dials_tempTarget = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_ProfileSettingsScreen_dials_pressureGauge = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_ProfileSettingsScreen_dials_pressureTarget = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_ProfileSettingsScreen_dials_pressureText = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_ProfileSettingsScreen_dials_tempText = ui_comp_get_child(ui_ProfileSettingsScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}

void ui_ProfileSettingsScreen_screen_destroy(void) {
    if (ui_ProfileSettingsScreen)
        lv_obj_del(ui_ProfileSettingsScreen);

    ui_ProfileSettingsScreen = NULL;
    ui_ProfileSettingsScreen_dials = NULL;
    ui_ProfileSettingsScreen_backButton = NULL;
    ui_ProfileSettingsScreen_titleLabel = NULL;
    ui_ProfileSettingsScreen_profileName = NULL;
    ui_ProfileSettingsScreen_adaptiveLabel = NULL;
    ui_ProfileSettingsScreen_adjustments = NULL;
    ui_ProfileSettingsScreen_targetTemp = NULL;
    ui_ProfileSettingsScreen_downTempButton = NULL;
    ui_ProfileSettingsScreen_upTempButton = NULL;
    ui_ProfileSettingsScreen_targetDuration = NULL;
    ui_ProfileSettingsScreen_downDurationButton = NULL;
    ui_ProfileSettingsScreen_upDurationButton = NULL;
    ui_ProfileSettingsScreen_saveButton = NULL;
    ui_ProfileSettingsScreen_acceptButton = NULL;
    ui_ProfileSettingsScreen_saveAsNewButton = NULL;
    uic_ProfileSettingsScreen_dials_tempGauge = NULL;
    uic_ProfileSettingsScreen_dials_tempTarget = NULL;
    uic_ProfileSettingsScreen_dials_pressureGauge = NULL;
    uic_ProfileSettingsScreen_dials_pressureTarget = NULL;
    uic_ProfileSettingsScreen_dials_pressureText = NULL;
    uic_ProfileSettingsScreen_dials_tempText = NULL;
}
