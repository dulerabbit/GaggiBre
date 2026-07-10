// GaggiMate — 4.3" rectangular display segmented-block sidebar dials
// Screen: 800×480 px
//
// Left edge  (x=0):   Temperature — red blocks, full height top→bottom,
//                     thermometer icon in the bottom-left corner.
// Right edge (x=742): Pressure    — blue blocks, full height top→bottom,
//                     pressure icon in the bottom-right corner.
//
// Blocks fill from TOP_MARGIN down to just above the corner icon (ICON_AREA_H).
//
// The functional lv_bar objects (TEMPGAUGE / PRESSUREGAUGE) are kept hidden so
// that DefaultUI.cpp's gauge_set_value() / gauge_set_range() calls continue to
// work.  A LV_EVENT_VALUE_CHANGED callback on each bar refreshes the visible
// block segments.  Block pointers are stored per-instance via lv_obj user_data
// so that multiple screens can each have their own independent dials component.
//
// Component child indices are IDENTICAL to ui_comp_dials.h so that
// DefaultUI.cpp's adjustDials() / adjustHeatingIndicator() work without changes.

#include "../ui.h"

// ── Layout constants ──────────────────────────────────────────────────────────
#define ICON_AREA_H   44    // px reserved at the bottom for the corner icon
#define TOP_MARGIN     6    // px gap at the very top edge

// Both sides use identical geometry for perfect symmetry.
// Available height = 480 - TOP_MARGIN - ICON_AREA_H = 430 px
// Math: 15×24 + 14×5 = 360 + 70 = 430 px exactly ✓
#define BLOCK_W           58    // block width — same for both sides (px)
#define BLOCK_H           24    // block height — same for both sides (px)
#define BLOCK_GAP          5    // gap between blocks — same for both sides (px)
#define BLOCK_COUNT       15    // total segments — same for both sides

// Temperature (left)
#define TEMP_BLOCK_W      BLOCK_W
#define TEMP_BLOCK_H      BLOCK_H
#define TEMP_BLOCK_GAP    BLOCK_GAP
#define TEMP_BLOCK_COUNT  BLOCK_COUNT
#define TEMP_BAR_RANGE   160    // must match lv_bar_set_range

// Pressure (right)
#define PRES_BLOCK_W      BLOCK_W
#define PRES_BLOCK_H      BLOCK_H
#define PRES_BLOCK_GAP    BLOCK_GAP
#define PRES_BLOCK_COUNT  BLOCK_COUNT
#define PRES_BAR_RANGE   160    // bar range = 16 bar × 10 (pressure stored ×10)

// ── Colour constants ──────────────────────────────────────────────────────────
#define TEMP_COLOR_ON   0xF62C2C   // bright red  — lit segment
#define TEMP_COLOR_OFF  0x3A1818   // dim red     — unlit segment
#define PRES_COLOR_ON   0x2CA4F6   // bright blue — lit segment
#define PRES_COLOR_OFF  0x0F2035   // dim blue    — unlit segment

// ── Per-instance block arrays stored as user_data on the bar ─────────────────
typedef struct {
    lv_obj_t *blocks[TEMP_BLOCK_COUNT];
    int8_t lastLit[TEMP_BLOCK_COUNT];
} temp_blocks_t;

typedef struct {
    lv_obj_t *blocks[PRES_BLOCK_COUNT];
    int8_t lastLit[PRES_BLOCK_COUNT];
} pres_blocks_t;

// ── Helper: free the user_data allocation when the bar is deleted ─────────────
static void bar_delete_cb(lv_event_t *e) {
    void *ud = lv_obj_get_user_data(lv_event_get_target(e));
    if (ud) lv_mem_free(ud);
}

// ── Helper: refresh temperature block colours from bar value ──────────────────
static void temp_bar_event_cb(lv_event_t *e) {
    lv_obj_t *bar       = lv_event_get_target(e);
    temp_blocks_t *inst = (temp_blocks_t *)lv_obj_get_user_data(bar);
    if (!inst) return;

    int32_t val = lv_bar_get_value(bar);
    int32_t max = lv_bar_get_max_value(bar);
    if (max <= 0) max = TEMP_BAR_RANGE;

    // blocks[0] is at the TOP (highest value), blocks[N-1] at the BOTTOM
    for (int i = 0; i < TEMP_BLOCK_COUNT; i++) {
        if (!inst->blocks[i]) continue;
        // Proportional threshold — no integer-truncation drift.
        // Block i (0=top) lights when val*COUNT >= (COUNT-i)*max
        const int8_t lit = (val * TEMP_BLOCK_COUNT >= (TEMP_BLOCK_COUNT - i) * max) ? 1 : 0;
        if (inst->lastLit[i] == lit) continue;
        inst->lastLit[i] = lit;
        lv_obj_set_style_bg_color(inst->blocks[i],
            lv_color_hex(lit ? TEMP_COLOR_ON : TEMP_COLOR_OFF),
            LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

// ── Helper: refresh pressure block colours from bar value ─────────────────────
static void pres_bar_event_cb(lv_event_t *e) {
    lv_obj_t *bar       = lv_event_get_target(e);
    pres_blocks_t *inst = (pres_blocks_t *)lv_obj_get_user_data(bar);
    if (!inst) return;

    int32_t val = lv_bar_get_value(bar);
    int32_t max = lv_bar_get_max_value(bar);
    if (max <= 0) max = PRES_BAR_RANGE;

    for (int i = 0; i < PRES_BLOCK_COUNT; i++) {
        if (!inst->blocks[i]) continue;
        // Proportional threshold — no integer-truncation drift.
        // Block i (0=top) lights when val*COUNT >= (COUNT-i)*max
        const int8_t lit = (val * PRES_BLOCK_COUNT >= (PRES_BLOCK_COUNT - i) * max) ? 1 : 0;
        if (inst->lastLit[i] == lit) continue;
        inst->lastLit[i] = lit;
        lv_obj_set_style_bg_color(inst->blocks[i],
            lv_color_hex(lit ? PRES_COLOR_ON : PRES_COLOR_OFF),
            LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

// ── Factory ───────────────────────────────────────────────────────────────────
lv_obj_t *ui_dials43_create(lv_obj_t *comp_parent) {

    // ── root container (full-screen, transparent, non-interactive) ────────────
    lv_obj_t *cui_dials = lv_obj_create(comp_parent);
    lv_obj_remove_style_all(cui_dials);
    lv_obj_set_size(cui_dials, 800, 480);
    lv_obj_set_pos(cui_dials, 0, 0);
    lv_obj_clear_flag(cui_dials, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(cui_dials, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ═════════════════════════════════════════════════════════════════════════
    // LEFT SIDE — Temperature blocks (full height, top to bottom)
    // ═════════════════════════════════════════════════════════════════════════

    temp_blocks_t *temp_inst = (temp_blocks_t *)lv_mem_alloc(sizeof(temp_blocks_t));
    lv_memset_00(temp_inst, sizeof(temp_blocks_t));
    for (int i = 0; i < TEMP_BLOCK_COUNT; i++) {
        temp_inst->lastLit[i] = -1;
    }

    // Blocks fill from TOP_MARGIN down to just above the icon area
    int temp_start_y = TOP_MARGIN;

    for (int i = 0; i < TEMP_BLOCK_COUNT; i++) {
        lv_obj_t *blk = lv_obj_create(cui_dials);
        lv_obj_remove_style_all(blk);
        lv_obj_set_size(blk, TEMP_BLOCK_W, TEMP_BLOCK_H);
        lv_obj_set_pos(blk, 0, temp_start_y + i * (TEMP_BLOCK_H + TEMP_BLOCK_GAP));
        lv_obj_clear_flag(blk, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(blk, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(blk, lv_color_hex(TEMP_COLOR_OFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(blk, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        temp_inst->blocks[i] = blk;
    }

    // Thermometer icon — bottom-left corner, directly below the block column
    lv_obj_t *cui_tempIcon = lv_img_create(cui_dials);
    lv_img_set_src(cui_tempIcon, &ui_img_1951499226);
    lv_obj_set_pos(cui_tempIcon, 4, 480 - ICON_AREA_H);
    lv_obj_add_flag(cui_tempIcon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(cui_tempIcon, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(cui_tempIcon, 140);
    lv_obj_set_style_img_recolor(cui_tempIcon, lv_color_hex(0xF62C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(cui_tempIcon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── [6] TEMPTEXT — live readout to the RIGHT of the left block column ─────
    // x=62 places it just outside the 58px block column so it does NOT overlap.
    // Red colour matches the temperature theme.
    lv_obj_t *cui_tempText = lv_label_create(cui_dials);
    lv_obj_set_pos(cui_tempText, 62, 8);
    lv_obj_set_size(cui_tempText, 60, LV_SIZE_CONTENT);
    lv_obj_clear_flag(cui_tempText, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(cui_tempText, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(cui_tempText, lv_color_hex(0xF62C2C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_tempText, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(cui_tempText, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(cui_tempText, "0\xC2\xB0" "C");

    // ── [1] TEMPGAUGE — hidden functional bar (driven by DefaultUI.cpp) ───────
    lv_obj_t *cui_tempGauge = lv_bar_create(cui_dials);
    lv_bar_set_range(cui_tempGauge, 0, TEMP_BAR_RANGE);
    lv_bar_set_value(cui_tempGauge, 0, LV_ANIM_OFF);
    lv_obj_set_size(cui_tempGauge, 1, 1);
    lv_obj_set_pos(cui_tempGauge, -10, -10);   // off-screen
    lv_obj_add_flag(cui_tempGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(cui_tempGauge, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_user_data(cui_tempGauge, temp_inst);
    lv_obj_add_event_cb(cui_tempGauge, temp_bar_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(cui_tempGauge, bar_delete_cb,     LV_EVENT_DELETE,        NULL);

    // ═════════════════════════════════════════════════════════════════════════
    // RIGHT SIDE — Pressure blocks (full height, top to bottom)
    // ═════════════════════════════════════════════════════════════════════════

    pres_blocks_t *pres_inst = (pres_blocks_t *)lv_mem_alloc(sizeof(pres_blocks_t));
    lv_memset_00(pres_inst, sizeof(pres_blocks_t));
    for (int i = 0; i < PRES_BLOCK_COUNT; i++) {
        pres_inst->lastLit[i] = -1;
    }

    int pres_start_y = TOP_MARGIN;
    int pres_x = 800 - PRES_BLOCK_W;  // flush to right edge

    for (int i = 0; i < PRES_BLOCK_COUNT; i++) {
        lv_obj_t *blk = lv_obj_create(cui_dials);
        lv_obj_remove_style_all(blk);
        lv_obj_set_size(blk, PRES_BLOCK_W, PRES_BLOCK_H);
        lv_obj_set_pos(blk, pres_x, pres_start_y + i * (PRES_BLOCK_H + PRES_BLOCK_GAP));
        lv_obj_clear_flag(blk, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(blk, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(blk, lv_color_hex(PRES_COLOR_OFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(blk, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        pres_inst->blocks[i] = blk;
    }

    // Pressure gauge icon — bottom-right corner, directly below the block column
    lv_obj_t *cui_Image6 = lv_img_create(cui_dials);
    lv_img_set_src(cui_Image6, &ui_img_1220767159);
    lv_obj_set_pos(cui_Image6, pres_x + 4, 480 - ICON_AREA_H);
    lv_obj_add_flag(cui_Image6, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(cui_Image6, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(cui_Image6, 140);
    lv_obj_set_style_img_recolor(cui_Image6, lv_color_hex(0x2CA4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(cui_Image6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ── [5] PRESSURETEXT — live readout to the LEFT of the right block column ─
    // pres_x=742; placing at pres_x-64=678 keeps the 60px label clear of blocks.
    // Blue colour matches the pressure theme.
    lv_obj_t *cui_pressureText = lv_label_create(cui_dials);
    lv_obj_set_pos(cui_pressureText, pres_x - 64, 8);
    lv_obj_set_size(cui_pressureText, 60, LV_SIZE_CONTENT);
    lv_obj_clear_flag(cui_pressureText, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(cui_pressureText, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(cui_pressureText, lv_color_hex(0x2CA4F6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_pressureText, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(cui_pressureText, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(cui_pressureText, "0\nbar");

    // ── [3] PRESSUREGAUGE — hidden functional bar ─────────────────────────────
    lv_obj_t *cui_pressureGauge = lv_bar_create(cui_dials);
    lv_bar_set_range(cui_pressureGauge, 0, PRES_BAR_RANGE);
    lv_bar_set_value(cui_pressureGauge, 0, LV_ANIM_OFF);
    lv_obj_set_size(cui_pressureGauge, 1, 1);
    lv_obj_set_pos(cui_pressureGauge, -10, -10);   // off-screen
    lv_obj_add_flag(cui_pressureGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(cui_pressureGauge, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_user_data(cui_pressureGauge, pres_inst);
    lv_obj_add_event_cb(cui_pressureGauge, pres_bar_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(cui_pressureGauge, bar_delete_cb,     LV_EVENT_DELETE,        NULL);

    // ── Targets (hidden, required by DefaultUI.cpp) ───────────────────────────
    lv_obj_t *cui_tempTarget = lv_img_create(cui_dials);
    lv_obj_add_flag(cui_tempTarget, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *cui_pressureTarget = lv_img_create(cui_dials);
    lv_obj_add_flag(cui_pressureTarget, LV_OBJ_FLAG_HIDDEN);

    // ── Register children at canonical indices ────────────────────────────────
    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_DIALS_NUM);
    children[UI_COMP_DIALS_DIALS]          = cui_dials;
    children[UI_COMP_DIALS_TEMPGAUGE]      = cui_tempGauge;
    children[UI_COMP_DIALS_TEMPTARGET]     = cui_tempTarget;
    children[UI_COMP_DIALS_PRESSUREGAUGE]  = cui_pressureGauge;
    children[UI_COMP_DIALS_PRESSURETARGET] = cui_pressureTarget;
    children[UI_COMP_DIALS_PRESSURETEXT]   = cui_pressureText;
    children[UI_COMP_DIALS_TEMPTEXT]       = cui_tempText;
    children[UI_COMP_DIALS_TEMPICON]       = cui_tempIcon;
    children[UI_COMP_DIALS_IMAGE6]         = cui_Image6;
    lv_obj_add_event_cb(cui_dials, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_dials, del_component_child_event_cb, LV_EVENT_DELETE, children);
    return cui_dials;
}
