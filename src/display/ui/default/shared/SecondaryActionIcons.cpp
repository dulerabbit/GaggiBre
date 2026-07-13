#include "SecondaryActionIcons.h"

#include <display/core/constants.h>
#include <display/ui/default/GeneratedUI.h>

extern "C" {
extern const lv_img_dsc_t img_manual_pressure_80x80;
}

namespace SecondaryActionIcons {

void applyMenuGrindButton(lv_obj_t *grindBtn, int secondaryAction) {
    if (!grindBtn) {
        return;
    }
    if (secondaryAction == SECONDARY_ACTION_MANUAL_BREW) {
        lv_obj_set_style_bg_img_src(grindBtn, &img_manual_pressure_80x80, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_recolor(grindBtn, lv_color_hex(0x2CA4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_recolor_opa(grindBtn, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_bg_img_src(grindBtn, &img_coffee_bean_80x80, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_recolor_opa(grindBtn, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

} // namespace SecondaryActionIcons
