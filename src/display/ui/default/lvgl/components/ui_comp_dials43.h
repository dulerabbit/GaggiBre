// GaggiMate — 4.3" rectangular display sidebar dials component
// Sidebar: 180×480px, two horizontal lv_bar gauges (no arcs)
// Children registered at identical indices to ui_comp_dials so DefaultUI.cpp needs no changes.

#ifndef UI_COMP_DIALS43_H
#define UI_COMP_DIALS43_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../ui.h"

lv_obj_t *ui_dials43_create(lv_obj_t *comp_parent);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
