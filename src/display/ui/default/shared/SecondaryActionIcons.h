#pragma once

#include <lvgl.h>

// Map Settings secondary-action to the 4th menu icon (Manual Brew pressure vs grind bean).
namespace SecondaryActionIcons {

void applyMenuGrindButton(lv_obj_t *grindBtn, int secondaryAction);

} // namespace SecondaryActionIcons
