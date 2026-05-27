#include "VoicePlugin.h"

#ifdef GAGGIMATE_VOICE_ENABLED

#include <display/core/Controller.h>
#include <display/core/PluginManager.h>
#include <display/core/constants.h>

// ============================================================
// Board-specific I2S pin definitions
// ============================================================
#if defined(WS43C_BOARD)
// Waveshare ESP32-S3-Touch-LCD-4.3C
// ES7210 quad-channel mic ADC → I2S DIN=43, BCLK=44, WS=16, MCLK=4
// ES8311 playback codec       → I2S DOUT=15 (not needed for voice capture only)
#  define VOICE_I2S_MCLK  4
#  define VOICE_I2S_BCLK  44
#  define VOICE_I2S_WS    16
#  define VOICE_I2S_DIN   43
#  define VOICE_I2S_DOUT  15
#  define VOICE_I2S_NUM   I2S_NUM_0
#elif defined(ELECROW_ADVANCE43)
// Elecrow CrowPanel Advance 4.3"
// INMP441 digital microphone — separate I2S bus, no MCLK required
#  define VOICE_I2S_MCLK  I2S_PIN_NO_CHANGE
#  define VOICE_I2S_BCLK  19
#  define VOICE_I2S_WS     2
#  define VOICE_I2S_DIN   20
#  define VOICE_I2S_DOUT  I2S_PIN_NO_CHANGE
#  define VOICE_I2S_NUM   I2S_NUM_1
#else
#  error "VoicePlugin: define WS43C_BOARD or ELECROW_ADVANCE43 to select I2S pins"
#endif

// ============================================================
// Wake word + command table
// ============================================================
// Wake word: "HILEXIN" = built-in "Hi ESP" model (no training needed).
// To use "Hey Bre": train a custom model at https://oai.espressif.com ,
// flash it to the model partition, and set the WakeNet model name here.
//
// Multiple phrasings map to the same command_id so recognition is more
// robust in a noisy kitchen environment.
static const sr_cmd_t kSrCommands[] = {
    { VoicePlugin::CMD_BREW_COFFEE, "brew coffee"  },
    { VoicePlugin::CMD_BREW_COFFEE, "make coffee"  },
    { VoicePlugin::CMD_BREW_COFFEE, "start brewing"},
    { VoicePlugin::CMD_STOP,        "stop"         },
    { VoicePlugin::CMD_STOP,        "cancel"       },
    { VoicePlugin::CMD_STEAM,       "steam"        },
    { VoicePlugin::CMD_STEAM,       "start steam"  },
};
static constexpr size_t kSrCommandCount = sizeof(kSrCommands) / sizeof(kSrCommands[0]);

// ============================================================
// Static instance pointer (so static callback can reach *this)
// ============================================================
VoicePlugin *VoicePlugin::s_instance = nullptr;

// ============================================================
// Constructor / setup / loop
// ============================================================
VoicePlugin::VoicePlugin() {
    s_instance = this;
}

void VoicePlugin::setup(Controller *ctrl, PluginManager *pm) {
    controller    = ctrl;
    pluginManager = pm;

    initI2S();

    if (!i2s) {
        Serial.println(F("[VoicePlugin] I2S init failed — voice control disabled"));
        return;
    }

    // Register the event callback and start the wake-word engine.
    // SR_CHANNELS_STEREO = two I2S channels (required by ESP-SR even for mono mics;
    // the input_format "MN" tells ESP-SR to use ch-0 as mic and ignore ch-1).
    ESP_SR.onEvent(srEventCallback);
    if (!ESP_SR.begin(*i2s, kSrCommands, kSrCommandCount,
                      SR_CHANNELS_STEREO, SR_MODE_WAKEWORD, "MN")) {
        Serial.println(F("[VoicePlugin] ESP_SR.begin() failed — check CONFIG_MODEL_IN_FLASH"));
        return;
    }

    ready = true;
    Serial.println(F("[VoicePlugin] Voice recognition active. Say 'Hi ESP' to wake."));
}

void VoicePlugin::loop() {
    // ESP_SR runs its own FreeRTOS task; nothing to poll here.
}

// ============================================================
// I2S initialisation
// ============================================================
void VoicePlugin::initI2S() {
    i2s = new I2SClass();
    i2s->setPins(VOICE_I2S_BCLK, VOICE_I2S_WS, VOICE_I2S_DIN,
                 VOICE_I2S_DOUT, VOICE_I2S_MCLK);

    // 16 kHz mono (fed as stereo to ESP-SR with "MN" format) at 16-bit depth.
    if (!i2s->begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT,
                    I2S_SLOT_MODE_STEREO, I2S_STD_SLOT_BOTH)) {
        Serial.println(F("[VoicePlugin] I2S begin failed"));
        delete i2s;
        i2s = nullptr;
    }
}

// ============================================================
// ESP-SR event callback (called from the SR task)
// ============================================================
void VoicePlugin::srEventCallback(sr_event_t event, int command_id, int /*phrase_id*/) {
    if (s_instance) {
        s_instance->onSrEvent(event, command_id);
    }
}

void VoicePlugin::onSrEvent(sr_event_t event, int command_id) {
    switch (event) {
        case SR_EVENT_WAKEWORD:
            Serial.println(F("[VoicePlugin] Wake word detected — listening for command"));
            pluginManager->trigger("voice:wakeword");
            break;

        case SR_EVENT_COMMAND:
            switch (command_id) {
                case CMD_BREW_COFFEE:
                    Serial.println(F("[VoicePlugin] Command: brew coffee"));
                    pluginManager->trigger("voice:command:brew");
                    if (controller) {
                        controller->setMode(MODE_BREW);
                    }
                    break;

                case CMD_STOP:
                    Serial.println(F("[VoicePlugin] Command: stop"));
                    pluginManager->trigger("voice:command:stop");
                    pluginManager->trigger("controller:process:stop");
                    break;

                case CMD_STEAM:
                    Serial.println(F("[VoicePlugin] Command: steam"));
                    pluginManager->trigger("voice:command:steam");
                    if (controller) {
                        controller->setMode(MODE_STEAM);
                    }
                    break;

                default:
                    break;
            }
            break;

        case SR_EVENT_TIMEOUT:
            Serial.println(F("[VoicePlugin] Command timeout — back to wake-word mode"));
            break;

        default:
            break;
    }
}

#endif // GAGGIMATE_VOICE_ENABLED
