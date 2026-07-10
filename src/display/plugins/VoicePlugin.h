#ifndef VOICE_PLUGIN_H
#define VOICE_PLUGIN_H

// ============================================================
// VoicePlugin — "Hey Bre" wake word + 3 brew commands
//
// Enabled by -DGAGGIMATE_VOICE_ENABLED in the build.
//
// Hardware requirements:
//   • Waveshare 4.3C  (-DWS43C_BOARD)
//       ES7210 quad-mic ADC via I2S: MCLK=4, BCLK=44, WS=16, DIN=43
//       ES8311 playback codec:       I2S DOUT=15
//       Codec I2C: SDA=8, SCL=9
//   • Elecrow Advance 4.3"  (-DELECROW_ADVANCE43)
//       INMP441 I2S microphone:  BCLK=19, WS=2, DIN=20
//
// Wake word: "Hi ESP" (built-in HILEXIN model, shipped with arduino-esp32 >=3.0)
//            → submit "Hey Bre" to https://oai.espressif.com to get a custom model
//
// Commands (MultiNet):
//   0 — "brew coffee"  → start current brew profile
//   1 — "stop"         → stop active process
//   2 — "steam"        → switch to steam mode
//
// Build requirements:
//   • espressif32 >= 6.x  (arduino-esp32 3.x, ESP-IDF 5.x)
//   • CONFIG_MODEL_IN_FLASH=y  or  CONFIG_MODEL_IN_SDCARD=y
//     Add to platformio.ini:
//       board_build.sdkconfig_options = CONFIG_MODEL_IN_FLASH=y
//   • The ESP_SR library is auto-included with arduino-esp32 3.x for ESP32-S3.
// ============================================================

#ifdef GAGGIMATE_VOICE_ENABLED

#include <display/core/Plugin.h>
#include <ESP_I2S.h>
#include <ESP_SR.h>

class VoicePlugin : public Plugin {
  public:
    VoicePlugin();
    void setup(Controller *controller, PluginManager *pluginManager) override;
    void loop() override;

  private:
    Controller    *controller    = nullptr;
    PluginManager *pluginManager = nullptr;

    I2SClass *i2s = nullptr;
    bool      ready = false;

    void initI2S();
    static void srEventCallback(sr_event_t event, int command_id, int phrase_id);

    // Command IDs matching the sr_commands[] table
    static constexpr int CMD_BREW_COFFEE = 0;
    static constexpr int CMD_STOP        = 1;
    static constexpr int CMD_STEAM       = 2;

    // Singleton-style pointer so the static callback can reach the plugin instance
    static VoicePlugin *s_instance;
    void onSrEvent(sr_event_t event, int command_id);
};

#endif // GAGGIMATE_VOICE_ENABLED
#endif // VOICE_PLUGIN_H
