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

class Waveshare43Panel : public Display {
  public:
    Waveshare43Panel();
    ~Waveshare43Panel() override;

    bool begin();

    void setBrightness(uint8_t level);
    uint8_t getBrightness() const;

    uint16_t width() override;
    uint16_t height() override;
    void pushColors(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *data) override;
    bool supportsDirectMode() override { return false; }

    // Keep LVGL in partial-refresh mode.  The installed Arduino-ESP32 RGB API
    // has no framebuffer-swap support, so the panel driver gates these dirty
    // rectangle copies at frame completion to reduce live-scan tearing.
    bool prefersFullRefresh() override { return false; }

    uint8_t getPoint(int16_t *x_array, int16_t *y_array, uint8_t get_point = 1) override;
    bool isPressed() const;

  private:
    bool _has_init;
    uint8_t _brightness;
    uint8_t _ch422g_state;

    esp_lcd_panel_handle_t _panelDrv;
    SemaphoreHandle_t _frameDoneSem;
    TouchDrvInterface *_touchDrv;

    void ch422gInit();
    void ch422gWrite(uint8_t bits);

    bool initTouch();
    void initBUS();
};
