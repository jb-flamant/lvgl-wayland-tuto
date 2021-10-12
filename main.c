/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>

#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_drivers/indev/mousewheel.h"

#include "lv_drivers/wayland/wayland.h"

#include "lvgl/examples/lv_examples.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();
    wayland_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init();

    lv_example_get_started_1();

    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    wayland_deinit();
    lv_deinit();

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{

/* Add a display */
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf1[WAYLAND_HOR_RES * WAYLAND_VER_RES];
  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, WAYLAND_HOR_RES * WAYLAND_VER_RES);

  /* Create a display */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = WAYLAND_HOR_RES;
  disp_drv.ver_res = WAYLAND_VER_RES;
  disp_drv.flush_cb = wayland_flush;
  lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

  /*Add a keyboard*/
  lv_indev_drv_t indev_drv_kb;
  lv_indev_drv_init(&indev_drv_kb);
  indev_drv_kb.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_kb.read_cb = wayland_keyboard_read;
  lv_indev_drv_register(&indev_drv_kb);

//  Add touchscreen:
    lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touch.read_cb = wayland_touch_read;
    lv_indev_drv_register(&indev_drv_touch);

//  Add mouse:
    lv_indev_drv_t indev_drv_mouse;
    lv_indev_drv_init(&indev_drv_mouse);
    indev_drv_mouse.type = LV_INDEV_TYPE_POINTER;
    indev_drv_mouse.read_cb = wayland_pointer_read;
    lv_indev_drv_register(&indev_drv_mouse);

//  Add mouse wheel as encoder:
    lv_indev_drv_t indev_drv_mousewheel;
    lv_indev_drv_init(&indev_drv_mousewheel);
    indev_drv_mousewheel.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_mousewheel.read_cb = wayland_pointeraxis_read;
    lv_indev_drv_register(&indev_drv_mousewheel);

}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void *data) {
    (void)data;

    while(1) {
        SDL_Delay(5);
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}
