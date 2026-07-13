#pragma once

// Shared generated-UI contract for DefaultUI and actions.
// Native 800x480 EEZ (eez43) is selected only by the explicit experimental
// flag. Production Waveshare (WS43C_BOARD alone) continues to use upstream eez
// plus WideLayout / procedural Manual Brew.
#if defined(GAGGIMATE_NATIVE_WS43_EEZ)
#define GAGGIMATE_HAS_NATIVE_MANUAL_BREW 1
#include <display/ui/default/eez43/actions.h>
#include <display/ui/default/eez43/images.h>
#include <display/ui/default/eez43/screens.h>
#include <display/ui/default/eez43/structs.h>
#include <display/ui/default/eez43/ui.h>
#else
#define GAGGIMATE_HAS_NATIVE_MANUAL_BREW 0
#include <display/ui/default/eez/actions.h>
#include <display/ui/default/eez/images.h>
#include <display/ui/default/eez/screens.h>
#include <display/ui/default/eez/structs.h>
#include <display/ui/default/eez/ui.h>
#endif
