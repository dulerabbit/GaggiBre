#pragma once

class Controller;

// Shared brew/process control semantics used by legacy eez/actions.cpp (via thin
// wrappers), experimental Actions.cpp, ManualBrewScreen, and NativeManualBrew.
// Callers own mode changes; these helpers only touch activate / deactivate / clear.
namespace ProcessControl {

void activate(Controller &controller);
void deactivate(Controller &controller);
void clear(Controller &controller);

// Stop the running process and discard the completed-process snapshot (brew cancel,
// Manual Brew exit while active, WideLayout pause).
void deactivateAndClear(Controller &controller);

// Idle → activate; active → deactivate (does not clear).
void toggleActive(Controller &controller);

} // namespace ProcessControl
