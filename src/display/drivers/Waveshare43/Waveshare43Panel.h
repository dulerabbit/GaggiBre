#pragma once

#include <Arduino.h>

#ifndef BOARD_HAS_PSRAM
#error "PSRAM is required for the RGB panel frame-buffer. Enable OPI PSRAM in the board definition."
#endif

#include <Wire.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_rgb.h>
#include <esp_lcd_panel_vendor.h>

#include <display/drivers/common/Display.h>
#include <display/drivers/common/ext.h>

// Waveshare ESP32-S3-Touch-LCD-4.3C
// 800×480 RGB565 parallel panel (ST7262), GT911 touch via I2C,
// CH422G GPIO expander for reset/backlight control.
class Waveshare43Panel : public Display {
  public:
    Waveshare43Panel();
    ~Waveshare43Panel() override;

    // Initialise the panel, touch, and backlight.
    // Returns true on success.  Safe to call multiple times.
    bool begin();

    // Backlight: level 0 = off, 1–255 = on (hardware supports on/off only).
    void setBrightness(uint8_t level);
    uint8_t getBrightness() const;

    // Display interface ─────────────────────────────────────
    uint16_t width() override;
    uint16_t height() override;
    void pushColors(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *data) override;
    bool supportsDirectMode() override { return false; }

    // Touch interface ───────────────────────────────────────
    uint8_t getPoint(int16_t *x_array, int16_t *y_array, uint8_t get_point = 1) override;
    bool isPressed() const;

  private:
    bool _has_init;
    uint8_t _brightness;
    uint8_t _ch422g_state;      // current CH422G output byte (shadow register)

    esp_lcd_panel_handle_t _panelDrv;
    TouchDrvInterface *_touchDrv;

    // CH422G helpers
    void ch422gInit();
    void ch422gWrite(uint8_t bits);

    // Sub-system init
    bool initTouch();
    void initBUS();
};
