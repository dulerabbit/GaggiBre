#include "ui_overhaul43.h"

namespace {

bool is_wide_screen() {
    lv_disp_t *disp = lv_disp_get_default();
    return disp != nullptr && lv_disp_get_hor_res(disp) > 481;
}

void set_icon(lv_obj_t *btn, const void *icon) {
    if (!btn || !icon) {
        return;
    }
    lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED,         nullptr, static_cast<const lv_img_dsc_t *>(icon), nullptr);
    lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_PRESSED,          nullptr, static_cast<const lv_img_dsc_t *>(icon), nullptr);
    lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_CHECKED_RELEASED, nullptr, static_cast<const lv_img_dsc_t *>(icon), nullptr);
    lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_CHECKED_PRESSED,  nullptr, static_cast<const lv_img_dsc_t *>(icon), nullptr);
}

// Bare icon button — no ring, no background, no border, no shadow.
// All icons are 40×40 px native — set size to 40×40 to prevent lv_imgbtn
// tiling (which would render a second cut-off copy to the right).
// Touch area is extended via lv_obj_set_ext_click_area() at the call site.
void style_round_icon_button(lv_obj_t *obj, int size = 40) {
    if (!obj) {
        return;
    }
    // Always use native icon size (40×40) to prevent tiling artefacts.
    // The caller's size hint is intentionally ignored here.
    (void)size;
    lv_obj_set_size(obj, 40, 40);

    // Fully transparent — no ring, no fill, no border
    lv_obj_set_style_bg_opa(obj,     LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0,           LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(obj, 0,           LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(obj,    0,             LV_PART_MAIN | LV_STATE_DEFAULT);

    // Dim slightly on press for tactile feedback
    lv_obj_set_style_img_opa(obj, LV_OPA_60, LV_PART_MAIN | LV_STATE_PRESSED);

    // White icon tint
    lv_obj_set_style_img_recolor(obj,     lv_color_hex(0xF2F4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(obj, LV_OPA_COVER,           LV_PART_MAIN | LV_STATE_DEFAULT);

    // Extend touch area so the effective hit zone is ~80×80 px
    lv_obj_set_ext_click_area(obj, 20);
}

void place_top_middle(lv_obj_t *obj) {
    if (!obj) {
        return;
    }
    lv_obj_set_align(obj, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(obj, 0, 14);
}

void style_back_button(lv_obj_t *btn) {
    if (!btn) {
        return;
    }
    set_icon(btn, &ui_img_295763949);
    style_round_icon_button(btn, 54);
    place_top_middle(btn);
}

void style_plus_minus_pair(lv_obj_t *minus_btn, lv_obj_t *plus_btn) {
    if (minus_btn) {
        set_icon(minus_btn, &ui_img_834125362);
        style_round_icon_button(minus_btn, 48);
    }
    if (plus_btn) {
        set_icon(plus_btn, &ui_img_390988422);
        style_round_icon_button(plus_btn, 48);
    }
}

} // namespace

void ui_overhaul43_apply_menu() {
    if (!is_wide_screen()) {
        return;
    }

    // No tile boxes — just bare icons as touch targets
    // (style_primary_tile removed; buttons already have bg_opa=0 and bg_img_src set)

    if (ui_MenuScreen_standbyButton) {
        set_icon(ui_MenuScreen_standbyButton, &ui_img_2044104741);
        style_round_icon_button(ui_MenuScreen_standbyButton, 54);
        place_top_middle(ui_MenuScreen_standbyButton);
    }
}

void ui_overhaul43_apply_brew() {
    if (!is_wide_screen()) {
        return;
    }
    style_back_button(ui_BrewScreen_ImgButton5);
    // startButton is a plain imgbtn — apply the same bare-icon style used on all other screens.
    style_round_icon_button(ui_BrewScreen_startButton, 40);
}

void ui_overhaul43_apply_status() {
    if (!is_wide_screen()) {
        return;
    }

    style_back_button(ui_StatusScreen_ImgButton8);
    style_round_icon_button(ui_StatusScreen_pauseButton, 56);
}

void ui_overhaul43_apply_manual() {
    if (!is_wide_screen()) {
        return;
    }

    style_back_button(ui_ManualBrewScreen_backButton);
    style_round_icon_button(ui_ManualBrewScreen_startButton, 80);
    if (ui_ManualBrewScreen_startButton) {
        lv_obj_set_align(ui_ManualBrewScreen_startButton, LV_ALIGN_CENTER);
        lv_obj_set_pos(ui_ManualBrewScreen_startButton, 150, 110);
    }
}

void ui_overhaul43_apply_simple_process() {
    if (!is_wide_screen()) {
        return;
    }

    style_back_button(ui_SimpleProcessScreen_ImgButton6);
    style_plus_minus_pair(ui_SimpleProcessScreen_downTempButton, ui_SimpleProcessScreen_upTempButton);
    style_round_icon_button(ui_SimpleProcessScreen_goButton, 80);
    if (ui_SimpleProcessScreen_goButton) {
        lv_obj_set_align(ui_SimpleProcessScreen_goButton, LV_ALIGN_BOTTOM_MID);
        lv_obj_set_pos(ui_SimpleProcessScreen_goButton, 0, -60);
    }
}

void ui_overhaul43_apply_grind() {
    if (!is_wide_screen()) {
        return;
    }

    style_back_button(ui_GrindScreen_ImgButton2);
    style_plus_minus_pair(ui_GrindScreen_downDurationButton, ui_GrindScreen_upDurationButton);
    style_round_icon_button(ui_GrindScreen_startButton, 80);
    if (ui_GrindScreen_startButton) {
        lv_obj_set_align(ui_GrindScreen_startButton, LV_ALIGN_BOTTOM_MID);
        lv_obj_set_pos(ui_GrindScreen_startButton, 0, -20);
    }
}

void ui_overhaul43_apply_profile() {
    if (!is_wide_screen()) {
        return;
    }

    style_back_button(ui_ProfileScreen_ImgButton1);

    if (ui_ProfileScreen_previousProfileBtn) {
        set_icon(ui_ProfileScreen_previousProfileBtn, &ui_img_98036921);
        style_round_icon_button(ui_ProfileScreen_previousProfileBtn, 52);
    }
    if (ui_ProfileScreen_nextProfileBtn) {
        set_icon(ui_ProfileScreen_nextProfileBtn, &ui_img_944513416);
        style_round_icon_button(ui_ProfileScreen_nextProfileBtn, 52);
    }

    style_round_icon_button(ui_ProfileScreen_chooseButton, 80);
    if (ui_ProfileScreen_chooseButton) {
        lv_obj_set_align(ui_ProfileScreen_chooseButton, LV_ALIGN_BOTTOM_MID);
        lv_obj_set_pos(ui_ProfileScreen_chooseButton, 0, -20);
    }
}
