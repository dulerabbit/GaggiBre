#pragma once

#include <lvgl.h>

// Runtime adapter: upstream EEZ stays 480×480 (circular). On wide panels
// (Waveshare 800×480) expand screens and replace circular side gauges with
// full-height left (temp) / right (pressure) tick readouts — Gaggimate look,
// rectangular geometry.
namespace WideLayout {

constexpr int32_t kTickShort = 10; // standby / menu — dot-like
constexpr int32_t kTickLong = 50;  // brew-class — ~2× circular long

// Shared wide play/pause control (brew / water / grind / Manual Brew).
// Native 40px asset — do not enlarge imgbtn width (LVGL tiles the mid image).
constexpr lv_coord_t kPlaySize = 40;
constexpr lv_coord_t kPlayX = (800 - kPlaySize) / 2; // 380
constexpr lv_coord_t kPlayY = 480 - kPlaySize - 40;  // 400 — below stats, clear of side icons
// ~1cm on 4.3" 800×480 (~95mm active width) — swipe-back corridor beside play.
constexpr lv_coord_t kSwipeBackMarginPx = 84;

bool isActive();

// Call once after ui_init(). No-op on 480×480 boards.
void apply();

// Drive the lit tick range (0..1). Safe to call when inactive.
void update(float tempFraction, float pressureFraction);

// Menu↔brew tick-length morph (short↔long on vertical dials).
void setTickLength(int32_t len);
void animateTickLength(int32_t fromLen, int32_t toLen, uint32_t durationMs);

// Add full-height L/R tick columns to an arbitrary screen (e.g. Manual Brew).
// Idempotent per parent pointer.
void addStandaloneTicks(lv_obj_t *parent);

} // namespace WideLayout
