#pragma once

#include <lvgl.h>

class Controller;

namespace NativeManualBrew {

void init(Controller *controller);
void update();

void onLoad();
void toggle();
void setTemperatureFromPointer(lv_event_t *event);
void setPressureFromPointer(lv_event_t *event);
void setFlowFromPointer(lv_event_t *event);
void save();
void discard();
// Leave Manual Brew: deactivate+clear if active, restore profile backup, reset shot UI.
void onExit();

} // namespace NativeManualBrew
