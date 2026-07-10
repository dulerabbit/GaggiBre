/**
 * @file      LV_Helper.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co.,
 * Ltd
 * @date      2024-01-22
 *
 * GaggiBre: on wide panels (e.g. Waveshare 800×480) LVGL keeps the upstream
 * 480×480 EEZ coordinate space and the flush/touch paths letterbox into the
 * physical panel. Native 800×480 EEZ layouts land in a later phase.
 */
#include "LV_Helper.h"
#include <cstring>

#if LV_VERSION_CHECK(9, 0, 0)
#error "Currently not supported 9.x"
#endif

// Upstream EEZ UI is authored for a 480×480 circular display.
static constexpr uint16_t kEezUiWidth = 480;
static constexpr uint16_t kEezUiHeight = 480;

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_color_t *buf = NULL;
static lv_color_t *buf1 = NULL;
static Display *s_board = nullptr;
static bool s_letterbox = false;
static int16_t s_letterboxX = 0;

static void clearLetterboxBars(Display &board, int16_t xOffset) {
    // Paint the unused left/right columns black once so pillarboxes aren't garbage.
    //
    // Waveshare/LilyGo pushColors() forwards to esp_lcd_panel_draw_bitmap(),
    // which takes exclusive (x_start, y_start, x_end, y_end) — NOT width/height.
    // Passing a width as x_end (e.g. x_start=640, x_end=160) can hang the RGB
    // panel bring-up and leave a permanently blank screen.
    const uint16_t panelW = board.width();
    const uint16_t panelH = board.height();
    if (panelW <= kEezUiWidth) {
        return;
    }

    const uint16_t leftEnd = static_cast<uint16_t>(xOffset);
    const uint16_t rightStart = static_cast<uint16_t>(xOffset + kEezUiWidth);
    if (leftEnd == 0 || rightStart >= panelW) {
        return;
    }

    const uint16_t barWidth = leftEnd; // centered: left and right bars match
    const size_t stripPixels = static_cast<size_t>(barWidth) * panelH;
    auto *strip = static_cast<uint16_t *>(ps_malloc(stripPixels * sizeof(uint16_t)));
    if (strip == nullptr) {
        return;
    }
    memset(strip, 0, stripPixels * sizeof(uint16_t));
    board.pushColors(0, 0, leftEnd, panelH, strip);
    board.pushColors(rightStart, 0, panelW, panelH, strip);
    free(strip);
}

/* Display flushing */
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    auto *board = static_cast<Display *>(disp_drv->user_data);
    const int16_t x1 = area->x1 + (s_letterbox ? s_letterboxX : 0);
    const int16_t y1 = area->y1;
    const int16_t x2 = area->x2 + 1 + (s_letterbox ? s_letterboxX : 0);
    const int16_t y2 = area->y2 + 1;
    board->pushColors(static_cast<uint16_t>(x1), static_cast<uint16_t>(y1), static_cast<uint16_t>(x2),
                      static_cast<uint16_t>(y2), reinterpret_cast<uint16_t *>(color_p));
    lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    static int16_t x, y;
    auto *board = static_cast<Display *>(indev_driver->user_data);
    uint8_t touched = board->getPoint(&x, &y, 1);
    if (touched) {
        if (s_letterbox) {
            x = static_cast<int16_t>(x - s_letterboxX);
            if (x < 0 || x >= static_cast<int16_t>(kEezUiWidth) || y < 0 || y >= static_cast<int16_t>(kEezUiHeight)) {
                data->state = LV_INDEV_STATE_REL;
                return;
            }
        }
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
        return;
    }
    data->state = LV_INDEV_STATE_REL;
}

#if LV_USE_LOG
void lv_log_print_g_cb(const char *buf) {
    Serial.println(buf);
    Serial.flush();
}
#endif

String lvgl_helper_get_fs_filename(String filename) {
    static String path;
    path = String("A") + ":" + (filename);
    return path;
}

const char *lvgl_helper_get_fs_filename(const char *filename) {
    static String path;
    path = String("A") + ":" + String(filename);
    return path.c_str();
}

void beginLvglHelper(Display &board, bool debug) {

    lv_init();

#if LV_USE_LOG
    if (debug) {
        lv_log_register_print_cb(lv_log_print_g_cb);
    }
#endif

    s_board = &board;
    s_letterbox = board.width() > 481;
    s_letterboxX = s_letterbox ? static_cast<int16_t>((board.width() - kEezUiWidth) / 2) : 0;

    // LVGL coordinate space matches the EEZ project (480×480). On wide panels
    // the flush path letterboxes into the physical framebuffer.
    const uint16_t uiW = s_letterbox ? kEezUiWidth : board.width();
    const uint16_t uiH = s_letterbox ? kEezUiHeight : board.height();

    if (s_letterbox) {
        // Partial PSRAM draw buffers for 800×480-class panels.
        const size_t partial_lines = 360;
        const size_t partial_size = static_cast<size_t>(uiW) * partial_lines * sizeof(lv_color_t);
        buf = (lv_color_t *)ps_malloc(partial_size);
        assert(buf);
        buf1 = (lv_color_t *)ps_malloc(partial_size);
        assert(buf1);
        lv_disp_draw_buf_init(&draw_buf, buf, buf1, uiW * partial_lines);
        clearLetterboxBars(board, s_letterboxX);
    } else {
        size_t lv_buffer_size = static_cast<size_t>(uiW) * uiH * sizeof(lv_color_t);
        buf = (lv_color_t *)ps_malloc(lv_buffer_size);
        assert(buf);

        if (!board.supportsDirectMode()) {
            buf1 = (lv_color_t *)ps_malloc(lv_buffer_size);
            assert(buf1);
        }

        lv_disp_draw_buf_init(&draw_buf, buf, buf1, uiW * uiH);
    }

    /*Initialize the display*/
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = uiW;
    disp_drv.ver_res = uiH;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = board.prefersFullRefresh() ? 1 : 0;
    disp_drv.direct_mode = board.supportsDirectMode();
    disp_drv.user_data = &board;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_drv.user_data = &board;
    lv_indev_drv_register(&indev_drv);
}
