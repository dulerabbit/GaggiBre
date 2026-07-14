#!/usr/bin/env python3
"""Convert docs/assets/gaggibre-logo.png → EEZ ui_image_logo.c for standby splash."""

from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "docs" / "assets" / "gaggibre-logo.png"
OUT_C = ROOT / "src" / "display" / "ui" / "default" / "eez" / "images" / "ui_image_logo.c"

# Circular LilyGo standby (480×480): bake at SquareLine-stable on-screen width
# so zoom 256 fits inside the round bezel. Wide 4.3 uses eez43's own logo.
TARGET_W = 380
PAD = 8


def rgb565le(red: int, green: int, blue: int) -> tuple[int, int]:
    value = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3)
    return value & 0xFF, (value >> 8) & 0xFF


def content_bbox(img: Image.Image) -> tuple[int, int, int, int]:
    """Tight box around opaque pixels (transparent source)."""
    pixels = img.load()
    min_x, min_y = img.width, img.height
    max_x, max_y = 0, 0
    for y in range(img.height):
        for x in range(img.width):
            _r, _g, _b, alpha = pixels[x, y]
            if alpha < 8:
                continue
            min_x = min(min_x, x)
            min_y = min(min_y, y)
            max_x = max(max_x, x)
            max_y = max(max_y, y)
    if max_x < min_x:
        return (0, 0, img.width, img.height)
    min_x = max(0, min_x - PAD)
    min_y = max(0, min_y - PAD)
    max_x = min(img.width, max_x + PAD + 1)
    max_y = min(img.height, max_y + PAD + 1)
    return (min_x, min_y, max_x, max_y)


src = Image.open(SOURCE).convert("RGBA")
src = src.crop(content_bbox(src))

w, h = src.size
scale = TARGET_W / w
new_w = max(1, int(round(w * scale)))
new_h = max(1, int(round(h * scale)))
img = src.resize((new_w, new_h), Image.Resampling.LANCZOS)

pixels = img.load()
data: list[int] = []
for y in range(img.height):
    for x in range(img.width):
        red, green, blue, alpha = pixels[x, y]
        # Real alpha only — keep white GAGGI / red BRE; transparent stays clear (black standby).
        if alpha == 0:
            red, green, blue = 0, 0, 0
        lo, hi = rgb565le(red, green, blue)
        data.extend((lo, hi, alpha))

hex_bytes = [f"0x{value:02X}" for value in data]
lines = [", ".join(hex_bytes[i : i + 24]) + "," for i in range(0, len(hex_bytes), 24)]
body = "\n".join("    " + line for line in lines)

text = f"""// Auto-generated from docs/assets/gaggibre-logo.png ({new_w}x{new_h})
// Do not hand-edit; re-run: python scripts/make_gaggibre_logo.py
#ifdef __has_include
#if __has_include("lvgl.h")
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_IMG_LOGO
#define LV_ATTRIBUTE_IMG_IMG_LOGO
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_LOGO uint8_t img_logo_map[] = {{
{body}
}};

const lv_img_dsc_t img_logo = {{
    .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = {new_w},
    .header.h = {new_h},
    .data_size = {len(data)},
    .data = img_logo_map,
}};
"""

OUT_C.write_text(text, encoding="utf-8")
print(f"Wrote {OUT_C} ({new_w}x{new_h}, {len(data)} bytes)")
