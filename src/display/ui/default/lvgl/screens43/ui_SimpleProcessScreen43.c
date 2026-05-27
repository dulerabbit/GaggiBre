// GaggiMate — 4.3" Simple Process Screen (800×480) — Steam / Water / Rinse
// Layout: 180px sidebar | 618px main content
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_SimpleProcessScreen_screen_init_43(void) {
    ui_SimpleProcessScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SimpleProcessScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_SimpleProcessScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_SimpleProcessScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Sidebar
    ui_SimpleProcessScreen_dials = ui_dials43_create(ui_SimpleProcessScreen);

    // ── Content panel (618×480) ────────────────────────────────────────────
    ui_SimpleProcessScreen_contentPanel5 = lv_obj_create(ui_SimpleProcessScreen);
    lv_obj_remove_style_all(ui_SimpleProcessScreen_contentPanel5);
    lv_obj_set_size(ui_SimpleProcessScreen_contentPanel5, 618, 480);
    lv_obj_set_pos(ui_SimpleProcessScreen_contentPanel5, 182, 0);
    lv_obj_clear_flag(ui_SimpleProcessScreen_contentPanel5, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_SimpleProcessScreen_contentPanel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Menu (back) button
    ui_SimpleProcessScreen_ImgButton6 = lv_imgbtn_create(ui_SimpleProcessScreen_contentPanel5);
    lv_imgbtn_set_src(ui_SimpleProcessScreen_ImgButton6, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_SimpleProcessScreen_ImgButton6, 44, 44);
    lv_obj_set_align(ui_SimpleProcessScreen_ImgButton6, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(ui_SimpleProcessScreen_ImgButton6, 14, 14);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_ImgButton6, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_ImgButton6, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Process label (e.g. "Steam")
    ui_SimpleProcessScreen_mainLabel5 = lv_label_create(ui_SimpleProcessScreen_contentPanel5);
    lv_obj_set_size(ui_SimpleProcessScreen_mainLabel5, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_SimpleProcessScreen_mainLabel5, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_SimpleProcessScreen_mainLabel5, 0, 16);
    lv_label_set_text(ui_SimpleProcessScreen_mainLabel5, "Steam");
    lv_obj_set_style_text_font(ui_SimpleProcessScreen_mainLabel5, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_mainLabel5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_mainLabel5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Thermometer icon — centre-left
    ui_SimpleProcessScreen_Image9 = lv_img_create(ui_SimpleProcessScreen_contentPanel5);
    lv_img_set_src(ui_SimpleProcessScreen_Image9, &ui_img_1951499226);
    lv_obj_set_size(ui_SimpleProcessScreen_Image9, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_SimpleProcessScreen_Image9, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_SimpleProcessScreen_Image9, 0, -90);
    lv_obj_add_flag(ui_SimpleProcessScreen_Image9, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_SimpleProcessScreen_Image9, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_SimpleProcessScreen_Image9, 220);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_Image9, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_Image9, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Target temperature (big label)
    ui_SimpleProcessScreen_targetTemp = lv_label_create(ui_SimpleProcessScreen_contentPanel5);
    lv_obj_set_size(ui_SimpleProcessScreen_targetTemp, 180, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_SimpleProcessScreen_targetTemp, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_SimpleProcessScreen_targetTemp, 0, 0);
    lv_label_set_text(ui_SimpleProcessScreen_targetTemp, "120°C");
    lv_obj_set_style_text_align(ui_SimpleProcessScreen_targetTemp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SimpleProcessScreen_targetTemp, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_targetTemp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Down temp button — left of label
    ui_SimpleProcessScreen_downTempButton = lv_imgbtn_create(ui_SimpleProcessScreen_contentPanel5);
    lv_imgbtn_set_src(ui_SimpleProcessScreen_downTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_SimpleProcessScreen_downTempButton, 50, 50);
    lv_obj_set_align(ui_SimpleProcessScreen_downTempButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_SimpleProcessScreen_downTempButton, -140, 0);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_downTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Up temp button — right of label
    ui_SimpleProcessScreen_upTempButton = lv_imgbtn_create(ui_SimpleProcessScreen_contentPanel5);
    lv_imgbtn_set_src(ui_SimpleProcessScreen_upTempButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_390988422, NULL);
    lv_obj_set_size(ui_SimpleProcessScreen_upTempButton, 50, 50);
    lv_obj_set_align(ui_SimpleProcessScreen_upTempButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_SimpleProcessScreen_upTempButton, 140, 0);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_upTempButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Go button — below the temp row
    ui_SimpleProcessScreen_goButton = lv_imgbtn_create(ui_SimpleProcessScreen_contentPanel5);
    lv_imgbtn_set_src(ui_SimpleProcessScreen_goButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_SimpleProcessScreen_goButton, 70, 70);
    lv_obj_set_align(ui_SimpleProcessScreen_goButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_SimpleProcessScreen_goButton, 0, 130);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_goButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_SimpleProcessScreen_goButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Events ────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_SimpleProcessScreen,                    ui_event_SimpleProcessScreen,                    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SimpleProcessScreen_ImgButton6,         ui_event_SimpleProcessScreen_ImgButton6,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SimpleProcessScreen_goButton,           ui_event_SimpleProcessScreen_goButton,           LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SimpleProcessScreen_downTempButton,     ui_event_SimpleProcessScreen_downTempButton,     LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SimpleProcessScreen_upTempButton,       ui_event_SimpleProcessScreen_upTempButton,       LV_EVENT_ALL, NULL);

    // ── Cache component child pointers ────────────────────────────────────
    uic_SimpleProcessScreen_dials_tempGauge      = ui_comp_get_child(ui_SimpleProcessScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_SimpleProcessScreen_dials_tempTarget     = ui_comp_get_child(ui_SimpleProcessScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_SimpleProcessScreen_dials_pressureGauge  = ui_comp_get_child(ui_SimpleProcessScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_SimpleProcessScreen_dials_pressureTarget = ui_comp_get_child(ui_SimpleProcessScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_SimpleProcessScreen_dials_pressureText   = ui_comp_get_child(ui_SimpleProcessScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_SimpleProcessScreen_dials_tempText       = ui_comp_get_child(ui_SimpleProcessScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
