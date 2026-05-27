// GaggiMate — 4.3" Menu Screen (800×480)
// Layout: 180px sidebar | 618px main with 2×2 button grid
#include "../ui.h"
#include "../components/ui_comp_dials43.h"

// ─── helper: create a common content panel (618×480, borderless) ──────────────
static lv_obj_t *make_main_panel(lv_obj_t *parent) {
    lv_obj_t *p = lv_obj_create(parent);
    lv_obj_remove_style_all(p);
    lv_obj_set_size(p, 618, 480);
    lv_obj_set_pos(p, 182, 0);
    lv_obj_clear_flag(p, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(p, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    return p;
}

void ui_MenuScreen_screen_init_43(void) {
    ui_MenuScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_MenuScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_MenuScreen, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, ui_MenuScreen_screen_destroy);
    ui_object_set_themeable_style_property(ui_MenuScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Dark);
    ui_object_set_themeable_style_property(ui_MenuScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_Dark);

    // Sidebar dials
    ui_MenuScreen_dials = ui_dials43_create(ui_MenuScreen);

    // Standby button — top-right of main area
    ui_MenuScreen_standbyButton = lv_imgbtn_create(ui_MenuScreen);
    lv_imgbtn_set_src(ui_MenuScreen_standbyButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_2044104741, NULL);
    lv_obj_set_size(ui_MenuScreen_standbyButton, 44, 44);
    lv_obj_set_align(ui_MenuScreen_standbyButton, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(ui_MenuScreen_standbyButton, -14, 14);
    ui_object_set_themeable_style_property(ui_MenuScreen_standbyButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_MenuScreen_standbyButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Main content panel (618×480 at x=182)
    ui_MenuScreen_contentPanel1 = lv_obj_create(ui_MenuScreen);
    lv_obj_remove_style_all(ui_MenuScreen_contentPanel1);
    lv_obj_set_size(ui_MenuScreen_contentPanel1, 618, 480);
    lv_obj_set_pos(ui_MenuScreen_contentPanel1, 182, 0);
    lv_obj_set_flex_flow(ui_MenuScreen_contentPanel1, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_MenuScreen_contentPanel1, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_obj_clear_flag(ui_MenuScreen_contentPanel1, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_left(ui_MenuScreen_contentPanel1,   20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_MenuScreen_contentPanel1,  60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_MenuScreen_contentPanel1,    20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_MenuScreen_contentPanel1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_MenuScreen_contentPanel1,    10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_MenuScreen_contentPanel1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    // no radius (rectangular)
    lv_obj_set_style_bg_opa(ui_MenuScreen_contentPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Brew button (top-left of 2×2 grid)
    ui_MenuScreen_btnBrew = lv_btn_create(ui_MenuScreen_contentPanel1);
    lv_obj_set_size(ui_MenuScreen_btnBrew, 248, 200);
    lv_obj_add_flag(ui_MenuScreen_btnBrew, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_MenuScreen_btnBrew, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_MenuScreen_btnBrew, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuScreen_btnBrew, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_MenuScreen_btnBrew, &ui_img_979979123, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_MenuScreen_btnBrew, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_MenuScreen_btnBrew, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_shadow_width(ui_MenuScreen_btnBrew, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Steam button (top-right)
    ui_MenuScreen_btnSteam = lv_btn_create(ui_MenuScreen_contentPanel1);
    lv_obj_set_size(ui_MenuScreen_btnSteam, 248, 200);
    lv_obj_add_flag(ui_MenuScreen_btnSteam, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_MenuScreen_btnSteam, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_MenuScreen_btnSteam, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuScreen_btnSteam, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_MenuScreen_btnSteam, &ui_img_783005998, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_MenuScreen_btnSteam, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_MenuScreen_btnSteam, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_shadow_width(ui_MenuScreen_btnSteam, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Water button (bottom-left)
    ui_MenuScreen_waterBtn = lv_btn_create(ui_MenuScreen_contentPanel1);
    lv_obj_set_size(ui_MenuScreen_waterBtn, 248, 200);
    lv_obj_add_flag(ui_MenuScreen_waterBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_MenuScreen_waterBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_MenuScreen_waterBtn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuScreen_waterBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_MenuScreen_waterBtn, &ui_img_545340440, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(ui_MenuScreen_waterBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_MenuScreen_waterBtn, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);
    lv_obj_set_style_shadow_width(ui_MenuScreen_waterBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Grind button (bottom-right)
    ui_MenuScreen_grindBtn = lv_btn_create(ui_MenuScreen_contentPanel1);
    lv_obj_set_size(ui_MenuScreen_grindBtn, 248, 200);
    lv_obj_add_flag(ui_MenuScreen_grindBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_MenuScreen_grindBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_MenuScreen_grindBtn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuScreen_grindBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_MenuScreen_grindBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_MenuScreen_grindIcon = lv_img_create(ui_MenuScreen_grindBtn);
    lv_img_set_src(ui_MenuScreen_grindIcon, &ui_img_363557387);
    lv_obj_center(ui_MenuScreen_grindIcon);
    lv_obj_clear_flag(ui_MenuScreen_grindIcon, LV_OBJ_FLAG_CLICKABLE);
    ui_object_set_themeable_style_property(ui_MenuScreen_grindIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR,     _ui_theme_color_NiceWhite);
    ui_object_set_themeable_style_property(ui_MenuScreen_grindIcon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, _ui_theme_alpha_NiceWhite);

    // Events
    lv_obj_add_event_cb(ui_MenuScreen_standbyButton, ui_event_MenuScreen_standbyButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MenuScreen_btnBrew,       ui_event_MenuScreen_btnBrew,       LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MenuScreen_btnSteam,      ui_event_MenuScreen_btnSteam,      LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MenuScreen_waterBtn,      ui_event_MenuScreen_waterBtn,      LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MenuScreen_grindBtn,      ui_event_MenuScreen_grindBtn,      LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MenuScreen,               ui_event_MenuScreen,               LV_EVENT_ALL, NULL);

    // Cache component child pointers
    uic_MenuScreen_dials_tempGauge      = ui_comp_get_child(ui_MenuScreen_dials, UI_COMP_DIALS_TEMPGAUGE);
    uic_MenuScreen_dials_tempTarget     = ui_comp_get_child(ui_MenuScreen_dials, UI_COMP_DIALS_TEMPTARGET);
    uic_MenuScreen_dials_pressureGauge  = ui_comp_get_child(ui_MenuScreen_dials, UI_COMP_DIALS_PRESSUREGAUGE);
    uic_MenuScreen_dials_pressureTarget = ui_comp_get_child(ui_MenuScreen_dials, UI_COMP_DIALS_PRESSURETARGET);
    uic_MenuScreen_dials_pressureText   = ui_comp_get_child(ui_MenuScreen_dials, UI_COMP_DIALS_PRESSURETEXT);
    uic_MenuScreen_dials_tempText       = ui_comp_get_child(ui_MenuScreen_dials, UI_COMP_DIALS_TEMPTEXT);
}
