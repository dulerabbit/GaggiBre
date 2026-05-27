#ifndef WAVESHARE43DRIVER_H
#define WAVESHARE43DRIVER_H

#include "Driver.h"
#include <display/drivers/Waveshare43/Waveshare43Panel.h>

// Driver for the Waveshare ESP32-S3-Touch-LCD-4.3C (800×480 RGB parallel
// panel).  isCompatible() probes the CH422G GPIO expander at its system
// register I2C address (0x24) on SDA=8 / SCL=9 — unique to this board.
class Waveshare43Driver : public Driver {
  public:
    bool isCompatible() override;
    void init() override;
    void setBrightness(int brightness) override { panel.setBrightness(static_cast<uint8_t>(brightness)); }
    bool supportsSDCard() override { return false; }
    bool installSDCard() override { return false; }

    static Waveshare43Driver *getInstance() {
        if (instance == nullptr) {
            instance = new Waveshare43Driver();
        }
        return instance;
    }

  private:
    static Waveshare43Driver *instance;
    Waveshare43Panel panel;
    Waveshare43Driver() {}
};

#endif // WAVESHARE43DRIVER_H
