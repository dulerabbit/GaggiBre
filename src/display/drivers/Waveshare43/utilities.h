#pragma once

// Waveshare ESP32-S3-Touch-LCD-4.3C hardware pin assignments.

// Lower pixel clock gives the PSRAM DMA more bandwidth headroom per line,
// reducing HSYNC slip (horizontal shake) on the 800×480 panel.
#define WS43_PCLK_HZ (12000000UL)
#define WS43_BOARD_TFT_WIDTH 800
#define WS43_BOARD_TFT_HEIGHT 480

// RGB transfer tuning to reduce tearing/artifacts on ESP32-S3 RGB panels.
#define WS43_RGB_NUM_FBS 2
#define WS43_RGB_BOUNCE_BUF_SIZE_PX (WS43_BOARD_TFT_WIDTH * 10)

#define WS43_TFT_HSYNC 46
#define WS43_TFT_VSYNC 3
#define WS43_TFT_DE 5
#define WS43_TFT_PCLK 7

// RGB565 data bus: B[4:0], G[5:0], R[4:0]
#define WS43_TFT_D0 14
#define WS43_TFT_D1 38
#define WS43_TFT_D2 18
#define WS43_TFT_D3 17
#define WS43_TFT_D4 10
#define WS43_TFT_D5 39
#define WS43_TFT_D6 0
#define WS43_TFT_D7 45
#define WS43_TFT_D8 48
#define WS43_TFT_D9 47
#define WS43_TFT_D10 21
#define WS43_TFT_D11 1
#define WS43_TFT_D12 2
#define WS43_TFT_D13 42
#define WS43_TFT_D14 41
#define WS43_TFT_D15 40

#define WS43_HSYNC_PULSE_WIDTH 4
#define WS43_HSYNC_BACK_PORCH 8
#define WS43_HSYNC_FRONT_PORCH 8

#define WS43_VSYNC_PULSE_WIDTH 4
#define WS43_VSYNC_BACK_PORCH 8
#define WS43_VSYNC_FRONT_PORCH 8

#define WS43_TOUCH_SDA 8
#define WS43_TOUCH_SCL 9
#define WS43_TOUCH_INT 4

// CH422G fixed I2C sub-addresses.
#define WS43_CH422G_ADDR_CFG 0x24
// CH422G WR-IO sub-address is 0x38 (0x70 >> 1) on Waveshare 4.3 examples.
#define WS43_CH422G_ADDR_OUT 0x38
// Optional fallback used by some earlier custom drivers.
#define WS43_CH422G_ADDR_OUT_FALLBACK 0x23

#define WS43_CH422G_BIT_TP_RST (1u << 1)
#define WS43_CH422G_BIT_DISP_EN (1u << 2)
#define WS43_CH422G_BIT_LCD_RST (1u << 3)

// Official Waveshare 4.3 config uses backlight ON level = HIGH.
#define WS43_BACKLIGHT_ON_LEVEL 1
