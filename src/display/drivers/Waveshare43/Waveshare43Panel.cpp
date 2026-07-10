#include "Waveshare43Panel.h"
#include "utilities.h"
#include <Wire.h>
#include <driver/gpio.h>

static bool ws43_on_frame_trans_done(esp_lcd_panel_handle_t panel, esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx) {
    (void)panel;
    (void)edata;

    auto sem = static_cast<SemaphoreHandle_t>(user_ctx);
    if (!sem) {
        return false;
    }

    BaseType_t highTaskAwoken = pdFALSE;
    xSemaphoreGiveFromISR(sem, &highTaskAwoken);
    return highTaskAwoken == pdTRUE;
}

Waveshare43Panel::Waveshare43Panel()
    : _brightness(0), _has_init(false), _ch422g_state(0),
      _panelDrv(nullptr), _frameDoneSem(nullptr), _touchDrv(nullptr) {}

Waveshare43Panel::~Waveshare43Panel() {
    if (_panelDrv) {
        esp_lcd_panel_del(_panelDrv);
        _panelDrv = nullptr;
    }
    if (_frameDoneSem) {
        vSemaphoreDelete(_frameDoneSem);
        _frameDoneSem = nullptr;
    }
    if (_touchDrv) {
        delete _touchDrv;
        _touchDrv = nullptr;
    }
}

bool Waveshare43Panel::begin() {
    if (_has_init) {
        return true;
    }

    Wire.begin(WS43_TOUCH_SDA, WS43_TOUCH_SCL);
    delay(10);

    ch422gInit();

    // Start from the same baseline as Waveshare's ESP-Panel config:
    // all expander-controlled outputs high, then pulse reset lines.
    _ch422g_state = WS43_CH422G_BIT_TP_RST | WS43_CH422G_BIT_LCD_RST;
#if WS43_BACKLIGHT_ON_LEVEL
    _ch422g_state |= WS43_CH422G_BIT_DISP_EN;
#endif
    ch422gWrite(_ch422g_state);
    delay(10);

    // LCD reset pulse: low -> high.
    ch422gWrite(_ch422g_state & ~WS43_CH422G_BIT_LCD_RST);
    delay(10);
    ch422gWrite(_ch422g_state | WS43_CH422G_BIT_LCD_RST);
    delay(100);

    // Touch reset pulse while TP_INT is held low to select GT911 address mode.
    gpio_set_direction(static_cast<gpio_num_t>(WS43_TOUCH_INT), GPIO_MODE_OUTPUT);
    gpio_set_level(static_cast<gpio_num_t>(WS43_TOUCH_INT), 0);
    delay(10);
    ch422gWrite(_ch422g_state & ~WS43_CH422G_BIT_TP_RST);
    delay(100);
    ch422gWrite(_ch422g_state | WS43_CH422G_BIT_TP_RST);
    delay(200);
    gpio_reset_pin(static_cast<gpio_num_t>(WS43_TOUCH_INT));

    _brightness = 255;

    if (!initTouch()) {
        Serial.println(F("[Waveshare43] Touch chip not found, continuing without touch"));
    }

    initBUS();

    _has_init = true;
    return true;
}

void Waveshare43Panel::ch422gInit() {
    Wire.beginTransmission(WS43_CH422G_ADDR_CFG);
    Wire.write(0x01);
    Wire.endTransmission();
}

void Waveshare43Panel::ch422gWrite(uint8_t bits) {
    _ch422g_state = bits;

    Wire.beginTransmission(WS43_CH422G_ADDR_OUT);
    Wire.write(bits);
    uint8_t rc = Wire.endTransmission();

    if (rc != 0) {
        // Keep compatibility with early custom implementations that used 0x23.
        Wire.beginTransmission(WS43_CH422G_ADDR_OUT_FALLBACK);
        Wire.write(bits);
        Wire.endTransmission();
    }
}

void Waveshare43Panel::setBrightness(uint8_t level) {
    // WS43 backlight is effectively binary (on/off through CH422G).
    // Guard against persisted UI settings of 0 that can immediately black out
    // the panel after boot on some boards.
    if (level == 0) {
        level = 1;
    }

    _brightness = level;
    if (_brightness == 0) {
#if WS43_BACKLIGHT_ON_LEVEL
        ch422gWrite(_ch422g_state & ~WS43_CH422G_BIT_DISP_EN);
#else
        ch422gWrite(_ch422g_state | WS43_CH422G_BIT_DISP_EN);
#endif
    } else {
#if WS43_BACKLIGHT_ON_LEVEL
        ch422gWrite(_ch422g_state | WS43_CH422G_BIT_DISP_EN);
#else
        ch422gWrite(_ch422g_state & ~WS43_CH422G_BIT_DISP_EN);
#endif
    }
}

uint8_t Waveshare43Panel::getBrightness() const { return _brightness; }

bool Waveshare43Panel::initTouch() {
    auto *drv = new TouchDrvGT911();
    drv->setPins(GPIO_NUM_NC, GPIO_NUM_NC);

    bool result = drv->begin(Wire, GT911_SLAVE_ADDRESS_L, WS43_TOUCH_SDA, WS43_TOUCH_SCL);
    if (!result) {
        result = drv->begin(Wire, GT911_SLAVE_ADDRESS_H, WS43_TOUCH_SDA, WS43_TOUCH_SCL);
    }

    if (result) {
        _touchDrv = drv;
        log_i("[Waveshare43] GT911 touch initialised");
        return true;
    }

    delete drv;
    _touchDrv = nullptr;
    return false;
}

void Waveshare43Panel::initBUS() {
    if (_panelDrv) {
        return;
    }

    // The installed Arduino-ESP32 RGB API exposes one PSRAM framebuffer plus a
    // frame-done callback, but no num_fbs/framebuffer-swap support. Gate dirty
    // rectangle copies to frame completion to avoid writing into the live scan
    // at arbitrary positions.
    _frameDoneSem = xSemaphoreCreateBinary();
    assert(_frameDoneSem);

    const esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings = {
            .pclk_hz = WS43_PCLK_HZ,
            .h_res = WS43_BOARD_TFT_WIDTH,
            .v_res = WS43_BOARD_TFT_HEIGHT,
            .hsync_pulse_width = WS43_HSYNC_PULSE_WIDTH,
            .hsync_back_porch = WS43_HSYNC_BACK_PORCH,
            .hsync_front_porch = WS43_HSYNC_FRONT_PORCH,
            .vsync_pulse_width = WS43_VSYNC_PULSE_WIDTH,
            .vsync_back_porch = WS43_VSYNC_BACK_PORCH,
            .vsync_front_porch = WS43_VSYNC_FRONT_PORCH,
            .flags = {
                .pclk_active_neg = 1,
            },
        },
        .data_width = 16,
        .sram_trans_align = 4,
        .psram_trans_align = 64,
        .hsync_gpio_num = WS43_TFT_HSYNC,
        .vsync_gpio_num = WS43_TFT_VSYNC,
        .de_gpio_num = WS43_TFT_DE,
        .pclk_gpio_num = WS43_TFT_PCLK,
        .data_gpio_nums = {
            WS43_TFT_D0, WS43_TFT_D1, WS43_TFT_D2, WS43_TFT_D3,
            WS43_TFT_D4, WS43_TFT_D5, WS43_TFT_D6, WS43_TFT_D7,
            WS43_TFT_D8, WS43_TFT_D9, WS43_TFT_D10, WS43_TFT_D11,
            WS43_TFT_D12, WS43_TFT_D13, WS43_TFT_D14, WS43_TFT_D15,
        },
        .disp_gpio_num = GPIO_NUM_NC,
        .on_frame_trans_done = ws43_on_frame_trans_done,
        .user_ctx = _frameDoneSem,
        .flags = {
            .fb_in_psram = 1,
        },
    };

    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &_panelDrv));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(_panelDrv));
    ESP_ERROR_CHECK(esp_lcd_panel_init(_panelDrv));
}

uint16_t Waveshare43Panel::width() { return WS43_BOARD_TFT_WIDTH; }
uint16_t Waveshare43Panel::height() { return WS43_BOARD_TFT_HEIGHT; }

void Waveshare43Panel::pushColors(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *data) {
    assert(_panelDrv);
    // No per-flush semaphore wait: with partial LVGL draw buffers there are
    // multiple flushes per frame; blocking each one for vsync would stall the
    // render loop.  PSRAM bandwidth contention (the root cause of HSYNC slip)
    // is addressed by keeping the LVGL draw buffers in internal SRAM instead.
    esp_lcd_panel_draw_bitmap(_panelDrv, x, y, w, h, data);
}

uint8_t Waveshare43Panel::getPoint(int16_t *x_array, int16_t *y_array, uint8_t get_point) {
    if (_touchDrv) {
        return _touchDrv->getPoint(x_array, y_array, get_point);
    }
    return 0;
}

bool Waveshare43Panel::isPressed() const {
    if (_touchDrv) {
        return _touchDrv->isPressed();
    }
    return false;
}
