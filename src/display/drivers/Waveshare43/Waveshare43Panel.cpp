#include "Waveshare43Panel.h"
#include "utilities.h"
#include <Wire.h>
#include <driver/gpio.h>

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------
Waveshare43Panel::Waveshare43Panel()
    : _brightness(0), _has_init(false), _ch422g_state(0),
      _panelDrv(nullptr), _touchDrv(nullptr) {}

Waveshare43Panel::~Waveshare43Panel() {
    if (_panelDrv) {
        esp_lcd_panel_del(_panelDrv);
        _panelDrv = nullptr;
    }
    if (_touchDrv) {
        delete _touchDrv;
        _touchDrv = nullptr;
    }
}

// ---------------------------------------------------------------------------
// begin() — one-time initialisation
// ---------------------------------------------------------------------------
bool Waveshare43Panel::begin() {
    if (_has_init) {
        return true;
    }

    // Shared I2C bus: GT911 touch, CH422G expander, ES8311/ES7210 audio codecs
    Wire.begin(WS43_TOUCH_SDA, WS43_TOUCH_SCL);
    delay(10);

    // Put LCD and touch panel into reset, backlight off
    ch422gInit();                                         // configure CH422G: all outputs
    ch422gWrite(0x00);                                    // everything deasserted / off
    delay(20);

    // Release LCD reset, turn backlight on — touch still held in reset
    ch422gWrite(WS43_CH422G_BIT_LCD_RST | WS43_CH422G_BIT_DISP_EN);
    delay(100);

    // Release touch reset
    ch422gWrite(WS43_CH422G_BIT_LCD_RST | WS43_CH422G_BIT_DISP_EN | WS43_CH422G_BIT_TP_RST);
    delay(50);

    _brightness = 255;   // backlight is on

    if (!initTouch()) {
        Serial.println(F("[Waveshare43] Touch chip not found — continuing without touch"));
    }

    initBUS();

    _has_init = true;
    return true;
}

// ---------------------------------------------------------------------------
// CH422G I2C GPIO expander helpers
// ---------------------------------------------------------------------------
// The CH422G uses fixed I2C addresses instead of a register-based protocol:
//   0x24 (7-bit) → write mode configuration (0x01 = all GPIOs as outputs)
//   0x23 (7-bit) → write output data byte
// ---------------------------------------------------------------------------
void Waveshare43Panel::ch422gInit() {
    Wire.beginTransmission(WS43_CH422G_ADDR_CFG);
    Wire.write(0x01);   // all IOs configured as push-pull outputs
    Wire.endTransmission();
}

void Waveshare43Panel::ch422gWrite(uint8_t bits) {
    _ch422g_state = bits;
    Wire.beginTransmission(WS43_CH422G_ADDR_OUT);
    Wire.write(bits);
    Wire.endTransmission();
}

// ---------------------------------------------------------------------------
// Backlight — CH422G EXIO2 (DISP_EN, active-HIGH)
// Hardware is on/off only (no PWM on the CH422G expander output).
// level 0 = off, any non-zero = on.
// ---------------------------------------------------------------------------
void Waveshare43Panel::setBrightness(uint8_t level) {
    _brightness = level;
    if (level == 0) {
        ch422gWrite(_ch422g_state & ~WS43_CH422G_BIT_DISP_EN);
    } else {
        ch422gWrite(_ch422g_state | WS43_CH422G_BIT_DISP_EN);
    }
}

uint8_t Waveshare43Panel::getBrightness() const {
    return _brightness;
}

// ---------------------------------------------------------------------------
// GT911 capacitive touch
// RST is driven through CH422G EXIO1 (already managed by begin()).
// INT (GPIO 4) is shared with I2S MCLK — use polling mode (INT = NC).
// ---------------------------------------------------------------------------
bool Waveshare43Panel::initTouch() {
    auto *drv = new TouchDrvGT911();
    drv->setPins(GPIO_NUM_NC, GPIO_NUM_NC);   // polling mode — no RST/INT GPIO
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

// ---------------------------------------------------------------------------
// RGB parallel panel (ST7262, 16-bit RGB565)
// ---------------------------------------------------------------------------
void Waveshare43Panel::initBUS() {
    if (_panelDrv) {
        return;
    }

    const esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings = {
            .pclk_hz           = WS43_PCLK_HZ,
            .h_res             = WS43_BOARD_TFT_WIDTH,
            .v_res             = WS43_BOARD_TFT_HEIGHT,
            .hsync_pulse_width = WS43_HSYNC_PULSE_WIDTH,
            .hsync_back_porch  = WS43_HSYNC_BACK_PORCH,
            .hsync_front_porch = WS43_HSYNC_FRONT_PORCH,
            .vsync_pulse_width = WS43_VSYNC_PULSE_WIDTH,
            .vsync_back_porch  = WS43_VSYNC_BACK_PORCH,
            .vsync_front_porch = WS43_VSYNC_FRONT_PORCH,
            .flags = {
                .pclk_active_neg = 1,   // data driven on falling edge of PCLK
            },
        },
        .data_width        = 16,   // RGB565
        .psram_trans_align = 64,
        .hsync_gpio_num    = WS43_TFT_HSYNC,
        .vsync_gpio_num    = WS43_TFT_VSYNC,
        .de_gpio_num       = WS43_TFT_DE,
        .pclk_gpio_num     = WS43_TFT_PCLK,
        .data_gpio_nums = {
            WS43_TFT_D0,  WS43_TFT_D1,  WS43_TFT_D2,  WS43_TFT_D3,
            WS43_TFT_D4,  WS43_TFT_D5,  WS43_TFT_D6,  WS43_TFT_D7,
            WS43_TFT_D8,  WS43_TFT_D9,  WS43_TFT_D10, WS43_TFT_D11,
            WS43_TFT_D12, WS43_TFT_D13, WS43_TFT_D14, WS43_TFT_D15,
        },
        .disp_gpio_num     = GPIO_NUM_NC,
        .on_frame_trans_done = nullptr,
        .user_ctx          = nullptr,
        .flags = {
            .fb_in_psram = 1,   // frame-buffer allocated in PSRAM
        },
    };

    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &_panelDrv));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(_panelDrv));
    ESP_ERROR_CHECK(esp_lcd_panel_init(_panelDrv));
}

// ---------------------------------------------------------------------------
// Display interface
// ---------------------------------------------------------------------------
uint16_t Waveshare43Panel::width()  { return WS43_BOARD_TFT_WIDTH; }
uint16_t Waveshare43Panel::height() { return WS43_BOARD_TFT_HEIGHT; }

void Waveshare43Panel::pushColors(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *data) {
    assert(_panelDrv);
    esp_lcd_panel_draw_bitmap(_panelDrv, x, y, x + w, y + h, data);
}

// ---------------------------------------------------------------------------
// Touch interface
// ---------------------------------------------------------------------------
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
