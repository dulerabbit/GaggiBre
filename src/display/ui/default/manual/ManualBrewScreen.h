#pragma once

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_manual_pressure_80x80;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class Controller;

// Hand-built 480×480 Manual Brew screen (works on LilyGo + letterboxed Waveshare).
// Not part of the EEZ Studio project — loaded via DefaultUI when MODE_MANUAL is active.
namespace ManualBrewScreen {

void init(Controller *controller);
void show();
void hide();
bool isActive();
void update(); // refresh labels / chart / save panel

void backupProfile();
void restoreProfile();

void onMenuIconApply(lv_obj_t *grindBtn); // swap coffee-bean ↔ manual-pressure icon

} // namespace ManualBrewScreen

#endif
