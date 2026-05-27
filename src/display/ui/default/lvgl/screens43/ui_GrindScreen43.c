// GaggiMate — 4.3" Grind Screen (800×480)
// Layout: 180px sidebar | 618px main content
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_GrindScreen_screen_init_43(void) {
    ui_GrindScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_GrindScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_GrindScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_GrindScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_GrindScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_GrindScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Sidebar
    ui_GrindScreen_dials = ui_dials43_create(ui_GrindScreen);

    // ── Content panel (618×480) ────────────────────────────────────────────
    ui_GrindScreen_contentPanel7 = lv_obj_create(ui_GrindScreen);
    lv_obj_remove_style_all(ui_GrindScreen_contentPanel7);
    lv_obj_set_size(ui_GrindScreen_contentPanel7, 618, 480);
    lv_obj_set_pos(ui_GrindScreen_contentPanel7, 182, 0);
    lv_obj_clear_flag(ui_GrindScreen_contentPanel7, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_GrindScreen_contentPanel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Menu (back) button
    ui_GrindScreen_ImgButton2 = lv_imgbtn_create(ui_GrindScreen_contentPanel7);
    lv_imgbtn_set_src(ui_GrindScreen_ImgButton2, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_GrindScreen_ImgButton2, 44, 44);
    lv_obj_set_align(ui_GrindScreen_ImgButton2, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_GrindScreen_ImgButton2, 14, 14);
    ui_object_set_themeable_style_property(ui_GrindScreen_ImgButton2, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_ImgButton2, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Screen title
    ui_GrindScreen_mainLabel7 = lv_label_create(ui_GrindScreen_contentPanel7);
    lv_obj_set_size(ui_GrindScreen_mainLabel7, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_GrindScreen_mainLabel7, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_GrindScreen_mainLabel7, 0, 16);
    lv_label_set_text(ui_GrindScreen_mainLabel7, "Grind");
    lv_obj_set_style_text_font(ui_GrindScreen_mainLabel7, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_GrindScreen_mainLabel7, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_mainLabel7, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Mode switch (by weight / by time)
    ui_GrindScreen_modeSwitch = lv_obj_create(ui_GrindScreen_contentPanel7);
    lv_obj_set_size(ui_GrindScreen_modeSwitch, 280, 54);
    lv_obj_set_align(ui_GrindScreen_modeSwitch, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_GrindScreen_modeSwitch, 0, -90);
    lv_obj_set_flex_flow(ui_GrindScreen_modeSwitch, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_GrindScreen_modeSwitch, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_GrindScreen_modeSwitch, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(ui_GrindScreen_modeSwitch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_GrindScreen_modeSwitch, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_GrindScreen_modeSwitch, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    ui_GrindScreen_volumetricButton = lv_img_create(ui_GrindScreen_modeSwitch);
    lv_img_set_src(ui_GrindScreen_volumetricButton, &ui_img_1424216268);
    lv_obj_set_size(ui_GrindScreen_volumetricButton, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_GrindScreen_volumetricButton, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_GrindScreen_volumetricButton, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_GrindScreen_volumetricButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_volumetricButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_GrindScreen_weightLabel = lv_label_create(ui_GrindScreen_modeSwitch);
    lv_obj_set_size(ui_GrindScreen_weightLabel, 140, 34);
    lv_label_set_text(ui_GrindScreen_weightLabel, "0.0g");
    lv_obj_set_style_text_align(ui_GrindScreen_weightLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_GrindScreen_weightLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_GrindScreen_weightLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_weightLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Duration target row ────────────────────────────────────────────────
    ui_GrindScreen_targetContainer = lv_obj_create(ui_GrindScreen_contentPanel7);
    lv_obj_remove_style_all(ui_GrindScreen_targetContainer);
    lv_obj_set_size(ui_GrindScreen_targetContainer, 400, 56);
    lv_obj_set_align(ui_GrindScreen_targetContainer, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_GrindScreen_targetContainer, 0, 0);
    lv_obj_clear_flag(ui_GrindScreen_targetContainer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_GrindScreen_targetContainer, LV_OBJ_FLAG_CLICKABLE);

    ui_GrindScreen_targetSymbol = lv_img_create(ui_GrindScreen_targetContainer);
    lv_img_set_src(ui_GrindScreen_targetSymbol, &ui_img_360122106);
    lv_obj_set_size(ui_GrindScreen_targetSymbol, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_GrindScreen_targetSymbol, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_GrindScreen_targetSymbol, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_GrindScreen_targetSymbol, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_GrindScreen_targetSymbol, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_targetSymbol, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_GrindScreen_downDurationButton = lv_imgbtn_create(ui_GrindScreen_targetContainer);
    lv_imgbtn_set_src(ui_GrindScreen_downDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_GrindScreen_downDurationButton, 44, 44);
    lv_obj_set_align(ui_GrindScreen_downDurationButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_GrindScreen_downDurationButton, -80, 0);
    ui_object_set_themeable_style_property(ui_GrindScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_downDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_GrindScreen_targetDuration = lv_label_create(ui_GrindScreen_targetContainer);
    lv_obj_set_size(ui_GrindScreen_targetDuration, 120, 40);
    lv_obj_set_align(ui_GrindScreen_targetDuration, LV_ALIGN_CENTER);
    lv_label_set_text(ui_GrindScreen_targetDuration, "0:15");
    lv_obj_set_style_text_align(ui_GrindScreen_targetDuration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_GrindScreen_targetDuration, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_GrindScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_targetDuration, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    ui_GrindScreen_upDurationButton = lv_imgbtn_create(ui_GrindScreen_targetContainer);
    lv_imgbtn_set_src(ui_GrindScreen_upDurationButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_GrindScreen_upDurationButton, 44, 44);
    lv_obj_set_align(ui_GrindScreen_upDurationButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_GrindScreen_upDurationButton, 80, 0);
    ui_object_set_themeable_style_property(ui_GrindScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_upDurationButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Start button
    ui_GrindScreen_startButton = lv_imgbtn_create(ui_GrindScreen_contentPanel7);
    lv_imgbtn_set_src(ui_GrindScreen_startButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_GrindScreen_startButton, 70, 70);
    lv_obj_set_align(ui_GrindScreen_startButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_GrindScreen_startButton, 0, 130);
    ui_object_set_themeable_style_property(ui_GrindScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_GrindScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Events ────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_GrindScreen,                        ui_event_GrindScreen,                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_GrindScreen_ImgButton2,             ui_event_GrindScreen_ImgButton2,             LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_GrindScreen_modeSwitch,             ui_event_GrindScreen_modeSwitch,             LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_GrindScreen_startButton,            ui_event_GrindScreen_startButton,            LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_GrindScreen_upDurationButton,       ui_event_GrindScreen_upDurationButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_GrindScreen_downDurationButton,     ui_event_GrindScreen_downDurationButton,     LV_EVENT_ALL, NULL);

    // ── Cache component child pointers ────────────────────────────────────
    uic_GrindScreen_dials_tempGauge      = ui_comp_get_child(ui_GrindScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_GrindScreen_dials_tempTarget     = ui_comp_get_child(ui_GrindScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_GrindScreen_dials_pressureGauge  = ui_comp_get_child(ui_GrindScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_GrindScreen_dials_pressureTarget = ui_comp_get_child(ui_GrindScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_GrindScreen_dials_pressureText   = ui_comp_get_child(ui_GrindScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_GrindScreen_dials_tempText       = ui_comp_get_child(ui_GrindScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
