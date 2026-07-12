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

// Hand-built Manual Brew screen (not in EEZ Studio).
// Matches WideLayout shell on 800×480: swipe left temp / right pressure + center graph.
// On 480×480 boards keeps the compact square layout.
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
