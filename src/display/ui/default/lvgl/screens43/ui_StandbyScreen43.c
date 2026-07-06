// GaggiMate — 4.3" Standby Screen (800×480, no sidebar)
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

void ui_StandbyScreen_screen_init_43(void) {
    ui_StandbyScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_StandbyScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_StandbyScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_StandbyScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_StandbyScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_StandbyScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Logo — centered on 800×480
    ui_StandbyScreen_logo = lv_img_create(ui_StandbyScreen);
    lv_img_set_src(ui_StandbyScreen_logo, &ui_img_logo_png);
    lv_obj_set_size(ui_StandbyScreen_logo, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StandbyScreen_logo, LV_ALIGN_CENTER);
    lv_obj_set_pos(ui_StandbyScreen_logo, 0, 10);
    lv_obj_add_flag(ui_StandbyScreen_logo, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StandbyScreen_logo, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_StandbyScreen_logo, 256);
    ui_object_set_themeable_style_property(ui_StandbyScreen_logo, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_logo, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Time label — top centre, hidden until active
    ui_StandbyScreen_time = lv_label_create(ui_StandbyScreen);
    lv_obj_set_size(ui_StandbyScreen_time, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StandbyScreen_time, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(ui_StandbyScreen_time, 0, 18);
    lv_label_set_text(ui_StandbyScreen_time, "0:00");
    lv_obj_add_flag(ui_StandbyScreen_time, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_align(ui_StandbyScreen_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StandbyScreen_time, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StandbyScreen_time, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_time, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    // Status icon row — top-right corner
    ui_StandbyScreen_statusContainer = lv_obj_create(ui_StandbyScreen);
    lv_obj_remove_style_all(ui_StandbyScreen_statusContainer);
    lv_obj_set_size(ui_StandbyScreen_statusContainer, 160, 26);
    lv_obj_set_align(ui_StandbyScreen_statusContainer, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(ui_StandbyScreen_statusContainer, -8, 8);
    lv_obj_set_flex_flow(ui_StandbyScreen_statusContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_StandbyScreen_statusContainer, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(ui_StandbyScreen_statusContainer, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_StandbyScreen_statusContainer, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_StandbyScreen_statusContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_StandbyScreen_wifiIcon = lv_img_create(ui_StandbyScreen_statusContainer);
    lv_img_set_src(ui_StandbyScreen_wifiIcon, &ui_img_364513079);
    lv_obj_set_size(ui_StandbyScreen_wifiIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_StandbyScreen_wifiIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StandbyScreen_wifiIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StandbyScreen_wifiIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_wifiIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_StandbyScreen_bluetoothIcon = lv_img_create(ui_StandbyScreen_statusContainer);
    lv_img_set_src(ui_StandbyScreen_bluetoothIcon, &ui_img_1091371356);
    lv_obj_set_size(ui_StandbyScreen_bluetoothIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_StandbyScreen_bluetoothIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StandbyScreen_bluetoothIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StandbyScreen_bluetoothIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_bluetoothIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    ui_StandbyScreen_updateIcon = lv_img_create(ui_StandbyScreen_statusContainer);
    lv_img_set_src(ui_StandbyScreen_updateIcon, &ui_img_1765671371);
    lv_obj_set_size(ui_StandbyScreen_updateIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_flag(ui_StandbyScreen_updateIcon, LV_OBJ_FLAG_ADV_HITTEST | LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_StandbyScreen_updateIcon, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_StandbyScreen_updateIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_updateIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Touch/tap icon — bottom centre
    ui_StandbyScreen_touchIcon = lv_img_create(ui_StandbyScreen);
    lv_img_set_src(ui_StandbyScreen_touchIcon, &ui_img_1732953241);
    lv_obj_set_size(ui_StandbyScreen_touchIcon, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StandbyScreen_touchIcon, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_StandbyScreen_touchIcon, 0, -50);
    lv_obj_add_flag(ui_StandbyScreen_touchIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_StandbyScreen_touchIcon, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_StandbyScreen_touchIcon, 256);
    ui_object_set_themeable_style_property(ui_StandbyScreen_touchIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_touchIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Main label — just above the touch icon
    ui_StandbyScreen_mainLabel = lv_label_create(ui_StandbyScreen);
    lv_obj_set_size(ui_StandbyScreen_mainLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_StandbyScreen_mainLabel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_pos(ui_StandbyScreen_mainLabel, 0, -14);
    lv_label_set_text(ui_StandbyScreen_mainLabel, "");
    lv_obj_set_style_text_font(ui_StandbyScreen_mainLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_StandbyScreen_mainLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_StandbyScreen_mainLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,   _ui_theme_alpha_NiceWhite);

    lv_obj_add_event_cb(ui_StandbyScreen, ui_event_StandbyScreen, LV_EVENT_ALL, NULL);
}
