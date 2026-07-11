from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SOURCE_PNG = ROOT / "ui" / "assets" / "tachometer-fast-40x40.png"
OUT_PNG = ROOT / "ui" / "assets" / "manual-pressure-80x80.png"
OUT_C = ROOT / "src" / "display" / "ui" / "default" / "lvgl" / "images" / "ui_img_manual_pressure_80x80.c"


def rgb565le(red, green, blue):
    value = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3)
    return value & 0xFF, (value >> 8) & 0xFF


src = Image.open(SOURCE_PNG).convert("RGBA")
# Upscale from the exact dial source icon so the menu icon matches shape while remaining crisp.
img = src.resize((80, 80), Image.Resampling.LANCZOS)
OUT_PNG.parent.mkdir(parents=True, exist_ok=True)
img.save(OUT_PNG)

pixels = img.load()
data = []
for y in range(img.height):
    for x in range(img.width):
        red, green, blue, alpha = pixels[x, y]
        lo, hi = rgb565le(red, green, blue)
        data.extend((lo, hi, alpha))

hex_bytes = [f"0x{value:02X}" for value in data]
lines = []
for index in range(0, len(hex_bytes), 24):
    lines.append("    " + ", ".join(hex_bytes[index:index + 24]) + ",")

text = "// Auto-generated from ui/assets/manual-pressure-80x80.png\n"
text += '#include "../ui.h"\n\n'
text += "#ifndef LV_ATTRIBUTE_MEM_ALIGN\n#define LV_ATTRIBUTE_MEM_ALIGN\n#endif\n\n"
text += "const LV_ATTRIBUTE_MEM_ALIGN uint8_t ui_img_manual_pressure_80x80_data[] = {\n"
text += "\n".join(lines)
text += "\n};\n\n"
text += "const lv_img_dsc_t ui_img_manual_pressure_80x80 = {\n"
text += "    .header.always_zero = 0,\n"
text += "    .header.w = 80,\n"
text += "    .header.h = 80,\n"
text += "    .data_size = sizeof(ui_img_manual_pressure_80x80_data),\n"
text += "    .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,\n"
text += "    .data = ui_img_manual_pressure_80x80_data,\n"
text += "};\n"
OUT_C.write_text(text, encoding="utf-8")
print(OUT_PNG)
print(OUT_C)
