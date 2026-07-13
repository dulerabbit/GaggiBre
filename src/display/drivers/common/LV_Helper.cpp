/**
 * @file      LV_Helper.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co.,
 * Ltd
 * @date      2024-01-22
 *
 * GaggiBre: LVGL uses the physical panel resolution. Waveshare 800×480 uses
 * the native generated eez43 tree; round boards retain the authored 480×480
 * upstream EEZ tree.
 */
#include "LV_Helper.h"
#include <cstring>

#if LV_VERSION_CHECK(9, 0, 0)
#error "Currently not supported 9.x"
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_color_t *buf = NULL;
static lv_color_t *buf1 = NULL;
static Display *s_board = nullptr;
static bool s_firstFlushInBatch = true;

/* Display flushing */
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    auto *board = static_cast<Display *>(disp_drv->user_data);
    if (s_firstFlushInBatch) {
        board->waitForFrameBoundary();
    }
    const int16_t x1 = area->x1;
    const int16_t y1 = area->y1;
    const int16_t x2 = area->x2 + 1;
    const int16_t y2 = area->y2 + 1;
    board->pushColors(static_cast<uint16_t>(x1), static_cast<uint16_t>(y1), static_cast<uint16_t>(x2),
                      static_cast<uint16_t>(y2), reinterpret_cast<uint16_t *>(color_p));
    s_firstFlushInBatch = lv_disp_flush_is_last(disp_drv);
    lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    static int16_t x, y;
    auto *board = static_cast<Display *>(indev_driver->user_data);
    uint8_t touched = board->getPoint(&x, &y, 1);
    if (touched) {
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
    const uint16_t uiW = board.width();
    const uint16_t uiH = board.height();

    // Full-frame buffers for <=480×480; partial lines for wide RGB panels.
    const bool usePartial = (static_cast<uint32_t>(uiW) * uiH) > (480u * 480u);
    if (usePartial) {
        const size_t partial_lines = 60;
        const size_t partial_size = static_cast<size_t>(uiW) * partial_lines * sizeof(lv_color_t);
        buf = (lv_color_t *)ps_malloc(partial_size);
        assert(buf);
        buf1 = (lv_color_t *)ps_malloc(partial_size);
        assert(buf1);
        lv_disp_draw_buf_init(&draw_buf, buf, buf1, uiW * partial_lines);
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
