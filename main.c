/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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
static pthread_t thread1;
static void* tick_thread(void* data);

int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();
    lv_wayland_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init();

    pthread_create( &thread1, NULL, tick_thread,NULL);

    lv_example_get_started_1();

    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    pthread_join(thread1,NULL);
    lv_wayland_deinit();
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

    /* Create a display */
    lv_disp_t * disp = lv_wayland_create_window(WAYLAND_HOR_RES,
                                                WAYLAND_VER_RES,
                                                WAYLAND_SURF_TITLE,
                                                NULL);

}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static void *tick_thread(void* data) {
    (void) data;
    while(1) {
        usleep(5 * 1000);
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }
}
