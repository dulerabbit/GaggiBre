#include "ProcessControl.h"

#include <display/core/Controller.h>

namespace ProcessControl {

void activate(Controller &controller) { controller.activate(); }

void deactivate(Controller &controller) { controller.deactivate(); }

void clear(Controller &controller) { controller.clear(); }

void deactivateAndClear(Controller &controller) {
    controller.deactivate();
    controller.clear();
}

void toggleActive(Controller &controller) {
    if (controller.isActive()) {
        controller.deactivate();
    } else {
        controller.activate();
    }
}

} // namespace ProcessControl
