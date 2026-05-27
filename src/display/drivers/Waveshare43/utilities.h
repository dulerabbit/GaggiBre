#pragma once

// ============================================================
// Waveshare ESP32-S3-Touch-LCD-4.3C — hardware pin assignments
// 800×480 RGB565 parallel panel (ST7262), GT911 touch,
// CH422G I2C GPIO expander, ES8311 + ES7210 audio codecs.
// ============================================================

// ─── RGB Panel ───────────────────────────────────────────────
#define WS43_PCLK_HZ            (16000000UL)   // 16 MHz pixel clock
#define WS43_BOARD_TFT_WIDTH    800
#define WS43_BOARD_TFT_HEIGHT   480

#define WS43_TFT_HSYNC   46
#define WS43_TFT_VSYNC    3
#define WS43_TFT_DE       5
#define WS43_TFT_PCLK     7

// RGB565 data bus — B[4:0], G[5:0], R[4:0]
#define WS43_TFT_D0    14   // B0
#define WS43_TFT_D1    38   // B1
#define WS43_TFT_D2    18   // B2
#define WS43_TFT_D3    17   // B3
#define WS43_TFT_D4    10   // B4
#define WS43_TFT_D5    39   // G0
#define WS43_TFT_D6     0   // G1
#define WS43_TFT_D7    45   // G2
#define WS43_TFT_D8    48   // G3
#define WS43_TFT_D9    47   // G4
#define WS43_TFT_D10   21   // G5
#define WS43_TFT_D11    1   // R0
#define WS43_TFT_D12    2   // R1
#define WS43_TFT_D13   42   // R2
#define WS43_TFT_D14   41   // R3
#define WS43_TFT_D15   40   // R4

// Horizontal sync timing
#define WS43_HSYNC_PULSE_WIDTH   4
#define WS43_HSYNC_BACK_PORCH    8
#define WS43_HSYNC_FRONT_PORCH   8

// Vertical sync timing
#define WS43_VSYNC_PULSE_WIDTH   4
#define WS43_VSYNC_BACK_PORCH   16
#define WS43_VSYNC_FRONT_PORCH  16

// ─── GT911 Capacitive Touch ──────────────────────────────────
// I2C shared with CH422G expander on the same bus.
// Touch INT (GPIO 4) is also I2S MCLK — use polling mode (INT = NC).
#define WS43_TOUCH_SDA   8
#define WS43_TOUCH_SCL   9

// ─── CH422G I2C GPIO Expander ────────────────────────────────
// The CH422G uses fixed I2C sub-addresses:
//   0x24 (7-bit) — system/mode register  (write 0x01 = all outputs)
//   0x23 (7-bit) — output data register  (write bit field)
#define WS43_CH422G_ADDR_CFG   0x24   // configure all pins as outputs
#define WS43_CH422G_ADDR_OUT   0x23   // set output bit field

// Bit positions in the CH422G output byte:
//   bit 1 → EXIO1 = TP_RST   (active-LOW: 0 = reset, 1 = active)
//   bit 2 → EXIO2 = DISP_EN  (active-HIGH: 1 = backlight on)
//   bit 3 → EXIO3 = LCD_RST  (active-LOW: 0 = reset, 1 = active)
#define WS43_CH422G_BIT_TP_RST   (1u << 1)
#define WS43_CH422G_BIT_DISP_EN  (1u << 2)
#define WS43_CH422G_BIT_LCD_RST  (1u << 3)

// ─── Audio — ES8311 playback codec + ES7210 quad-mic ADC ─────
// I2C shared with touch/expander on SDA=8, SCL=9.
// GPIO 4 doubles as I2S MCLK and (hardware) touch INT — touch uses polling.
#define WS43C_I2S_MCLK    4    // Master clock  (also GT911 INT pin — use touch in polling mode)
#define WS43C_I2S_BCLK   44    // Bit clock
#define WS43C_I2S_WS     16    // Word select (LR clock)
#define WS43C_I2S_DOUT   15    // Data out  → ES8311 (speaker DAC)
#define WS43C_I2S_DIN    43    // Data in   ← ES7210 (mic ADC)

#define WS43C_ES8311_I2C_ADDR   0x18
#define WS43C_ES7210_I2C_ADDR   0x40
