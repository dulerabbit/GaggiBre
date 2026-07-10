#include "Waveshare43Driver.h"
#include <Wire.h>
#include <display/drivers/Waveshare43/utilities.h>
#include <display/drivers/common/LV_Helper.h>

Waveshare43Driver *Waveshare43Driver::instance = nullptr;

// Probe the CH422G I2C GPIO expander on SDA=8 / SCL=9.
bool Waveshare43Driver::isCompatible() {
    Wire.begin(WS43_TOUCH_SDA, WS43_TOUCH_SCL);
    Wire.beginTransmission(WS43_CH422G_ADDR_CFG);
    bool ok = (Wire.endTransmission() == 0);
    Wire.end();
    return ok;
}

void Waveshare43Driver::init() {
    Serial.println(F("Waveshare43Driver: initialising 4.3C panel"));
    if (!panel.begin()) {
        for (int i = 0; i < 20; i++) {
            Serial.println(F("Error: failed to initialise Waveshare 4.3C panel"));
            delay(1000);
        }
        ESP.restart();
    }
    beginLvglHelper(panel);
}
