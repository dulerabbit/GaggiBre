#pragma once

#include <stdint.h>

class Display {
  public:
    virtual ~Display() = default;

    Display() : _rotation(0) {};
    virtual void pushColors(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *data) = 0;
    virtual uint16_t width() = 0;
    virtual uint16_t height() = 0;
    virtual uint8_t getPoint(int16_t *x, int16_t *y, uint8_t get_point) = 0;
    virtual bool supportsDirectMode() = 0;
    // Wide RGB panels (e.g. Waveshare 800×480) may prefer a different refresh
    // strategy; default keeps upstream LilyGo/Amoled behavior.
    virtual bool prefersFullRefresh() { return false; }
    // RGB panels with a live framebuffer can align each LVGL refresh batch to
    // a frame boundary. Other display types require no synchronization.
    virtual void waitForFrameBoundary() {}

  protected:
    uint8_t _rotation;
};
