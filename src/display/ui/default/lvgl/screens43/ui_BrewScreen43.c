// GaggiMate — 4.3" Brew Screen (800×480)
// Layout: 180px sidebar | 618px main content
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_BrewScreen_screen_init_43(void) {
    ui_BrewScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_BrewScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_BrewScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_BrewScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_BrewScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_BrewScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Sidebar
    ui_BrewScreen_dials = ui_dials43_create(ui_BrewScreen);

    // Menu (back) button — bottom-right
    ui_BrewScreen_ImgButton5 = lv_imgbtn_create(ui_BrewScreen);
    lv_imgbtn_set_src(ui_BrewScreen_ImgButton5, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_295763949, NULL);
    lv_obj_set_size(ui_BrewScreen_ImgButton5, 44, 44);
    lv_obj_set_align(ui_BrewScreen_ImgButton5, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_pos(ui_BrewScreen_ImgButton5, -14, -14);
    ui_object_set_themeable_style_property(ui_BrewScreen_ImgButton5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_ImgButton5, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── content panel (618×480, no clipping) ──────────────────────────────
    ui_BrewScreen_contentPanel4 = lv_obj_create(ui_BrewScreen);
    lv_obj_remove_style_all(ui_BrewScreen_contentPanel4);
    lv_obj_set_size(ui_BrewScreen_contentPanel4, 618, 480);
    lv_obj_set_pos(ui_BrewScreen_contentPanel4, 182, 0);
    lv_obj_clear_flag(ui_BrewScreen_contentPanel4, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_BrewScreen_contentPanel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Screen title
    ui_BrewScreen_mainLabel3 = lv_label_create(ui_BrewScreen_contentPanel4);
    lv_obj_set_size(ui_BrewScreen_mainLabel3, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_BrewScreen_mainLabel3, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_BrewScreen_mainLabel3, 0, 16);
    lv_label_set_text(ui_BrewScreen_mainLabel3, "Brew");
    lv_obj_set_style_text_font(ui_BrewScreen_mainLabel3, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_mainLabel3, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_mainLabel3, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Start button — prominent, vertically centred
    ui_BrewScreen_startButton = lv_imgbtn_create(ui_BrewScreen_contentPanel4);
    lv_imgbtn_set_src(ui_BrewScreen_startButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_BrewScreen_startButton, 60, 60);
    lv_obj_set_align(ui_BrewScreen_startButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_BrewScreen_startButton, 0, 120);
    lv_obj_add_flag(ui_BrewScreen_startButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_startButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Control container — column layout, centred
    ui_BrewScreen_controlContainer = lv_obj_create(ui_BrewScreen_contentPanel4);
    lv_obj_remove_style_all(ui_BrewScreen_controlContainer);
    lv_obj_set_size(ui_BrewScreen_controlContainer, 580, 260);
    lv_obj_set_align(ui_BrewScreen_controlContainer, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_BrewScreen_controlContainer, 0, -20);
    lv_obj_set_flex_flow(ui_BrewScreen_controlContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_BrewScreen_controlContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_BrewScreen_controlContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(ui_BrewScreen_controlContainer, 18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── Mode switch ────────────────────────────────────────────────────────
    ui_BrewScreen_modeSwitch = lv_obj_create(ui_BrewScreen_controlContainer);
    lv_obj_set_size(ui_BrewScreen_modeSwitch, 260, 54);
    lv_obj_set_flex_flow(ui_BrewScreen_modeSwitch, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_BrewScreen_modeSwitch, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_BrewScreen_modeSwitch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_BrewScreen_modeSwitch, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(ui_BrewScreen_modeSwitch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_modeSwitch, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_BrewScreen_modeSwitch, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    ui_BrewScreen_volumetricButton = lv_img_create(ui_BrewScreen_modeSwitch);
    lv_img_set_src(ui_BrewScreen_volumetricButton, &ui_img_1424216268);
    lv_obj_set_size(ui_BrewScreen_volumetricButton, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_BrewScreen_volumetricButton, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_BrewScreen_volumetricButton, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_BrewScreen_volumetricButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_volumetricButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_weightLabel = lv_label_create(ui_BrewScreen_modeSwitch);
    lv_obj_set_size(ui_BrewScreen_weightLabel, 130, 34);
    lv_label_set_text(ui_BrewScreen_weightLabel, "0.0g");
    lv_obj_set_style_text_align(ui_BrewScreen_weightLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_BrewScreen_weightLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_weightLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_weightLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // ── Profile info row ───────────────────────────────────────────────────
    ui_BrewScreen_profileInfo = lv_obj_create(ui_BrewScreen_controlContainer);
    lv_obj_remove_style_all(ui_BrewScreen_profileInfo);
    lv_obj_set_size(ui_BrewScreen_profileInfo, 560, 80);
    lv_obj_set_flex_flow(ui_BrewScreen_profileInfo, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_BrewScreen_profileInfo, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_BrewScreen_profileInfo, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_BrewScreen_profileInfo, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(ui_BrewScreen_profileInfo, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BrewScreen_Label1 = lv_label_create(ui_BrewScreen_profileInfo);
    lv_obj_set_size(ui_BrewScreen_Label1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(ui_BrewScreen_Label1, "Selected profile");
    lv_obj_set_style_text_font(ui_BrewScreen_Label1, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_BrewScreen_Label1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_Label1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);
    lv_obj_add_flag(ui_BrewScreen_Label1, LV_OBJ_FLAG_CLICKABLE);

    ui_BrewScreen_Container3 = lv_obj_create(ui_BrewScreen_profileInfo);
    lv_obj_remove_style_all(ui_BrewScreen_Container3);
    lv_obj_set_size(ui_BrewScreen_Container3, 560, 44);
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
    ui_object_set_themeable_style_property(ui_BrewScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_profileName, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);
    lv_obj_add_flag(ui_BrewScreen_profileName, LV_OBJ_FLAG_CLICKABLE);

    ui_BrewScreen_settingsButton = lv_imgbtn_create(ui_BrewScreen_Container3);
    lv_imgbtn_set_src(ui_BrewScreen_settingsButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_340148213, NULL);
    lv_obj_set_size(ui_BrewScreen_settingsButton, LV_SIZE_CONTENT, 44);
    ui_object_set_themeable_style_property(ui_BrewScreen_settingsButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_settingsButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Adjustments column (temp + duration rows) ──────────────────────────
    ui_BrewScreen_adjustments = lv_obj_create(ui_BrewScreen_controlContainer);
    lv_obj_remove_style_all(ui_BrewScreen_adjustments);
    lv_obj_set_size(ui_BrewScreen_adjustments, 560, 140);
    lv_obj_set_flex_flow(ui_BrewScreen_adjustments, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_BrewScreen_adjustments, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_BrewScreen_adjustments, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(ui_BrewScreen_adjustments, 16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Temperature row
    ui_BrewScreen_tempContainer = lv_obj_create(ui_BrewScreen_adjustments);
    lv_obj_remove_style_all(ui_BrewScreen_tempContainer);
    lv_obj_set_size(ui_BrewScreen_tempContainer, 560, 56);
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
    lv_label_set_text(ui_BrewScreen_targetTemp, "93°C");
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
    lv_obj_set_size(ui_BrewScreen_targetContainer, 560, 56);
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

    // By-time (delete volumetric) button
    ui_BrewScreen_byTimeButton = lv_imgbtn_create(ui_BrewScreen_contentPanel4);
    lv_imgbtn_set_src(ui_BrewScreen_byTimeButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_834125362, NULL);
    lv_obj_set_size(ui_BrewScreen_byTimeButton, 40, 40);
    lv_obj_set_align(ui_BrewScreen_byTimeButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_BrewScreen_byTimeButton, -110, -20);
    lv_obj_add_flag(ui_BrewScreen_byTimeButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_byTimeButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_byTimeButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Bottom save/accept row ─────────────────────────────────────────────
    ui_BrewScreen_saveButton = lv_imgbtn_create(ui_BrewScreen_contentPanel4);
    lv_imgbtn_set_src(ui_BrewScreen_saveButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_631115820, NULL);
    lv_obj_set_size(ui_BrewScreen_saveButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_saveButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_BrewScreen_saveButton, -60, -20);
    lv_obj_add_flag(ui_BrewScreen_saveButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_acceptButton = lv_imgbtn_create(ui_BrewScreen_contentPanel4);
    lv_imgbtn_set_src(ui_BrewScreen_acceptButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_445946954, NULL);
    lv_obj_set_size(ui_BrewScreen_acceptButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_acceptButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_BrewScreen_acceptButton, 0, -20);
    lv_obj_add_flag(ui_BrewScreen_acceptButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_acceptButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_BrewScreen_saveAsNewButton = lv_imgbtn_create(ui_BrewScreen_contentPanel4);
    lv_imgbtn_set_src(ui_BrewScreen_saveAsNewButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_332059803, NULL);
    lv_obj_set_size(ui_BrewScreen_saveAsNewButton, 44, 44);
    lv_obj_set_align(ui_BrewScreen_saveAsNewButton, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_BrewScreen_saveAsNewButton, 60, -20);
    lv_obj_add_flag(ui_BrewScreen_saveAsNewButton, LV_OBJ_FLAG_HIDDEN);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveAsNewButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_BrewScreen_saveAsNewButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // ── Events ────────────────────────────────────────────────────────────
    lv_obj_add_event_cb(ui_BrewScreen,                    ui_event_BrewScreen,                    LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_ImgButton5,         ui_event_BrewScreen_ImgButton5,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_startButton,        ui_event_BrewScreen_startButton,        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_modeSwitch,         ui_event_BrewScreen_modeSwitch,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_profileSelectBtn,   ui_event_BrewScreen_profileSelectBtn,   LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_settingsButton,     ui_event_BrewScreen_settingsButton,     LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_downTempButton,     ui_event_BrewScreen_downTempButton,     LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_upTempButton,       ui_event_BrewScreen_upTempButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_upDurationButton,   ui_event_BrewScreen_upDurationButton,   LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_downDurationButton, ui_event_BrewScreen_downDurationButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_byTimeButton,       ui_event_BrewScreen_byTimeButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_saveButton,         ui_event_BrewScreen_saveButton,         LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_acceptButton,       ui_event_BrewScreen_acceptButton,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BrewScreen_saveAsNewButton,    ui_event_BrewScreen_saveAsNewButton,    LV_EVENT_ALL, NULL);

    // ── Cache component child pointers ────────────────────────────────────
    uic_BrewScreen_dials_tempGauge      = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_BrewScreen_dials_tempTarget     = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_BrewScreen_dials_pressureGauge  = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_BrewScreen_dials_pressureTarget = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_BrewScreen_dials_pressureText   = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_BrewScreen_dials_tempText       = ui_comp_get_child(ui_BrewScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
