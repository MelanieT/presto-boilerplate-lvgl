#include "st7701.hpp"
#include "lvgl.h"
#include "SquareLineWorkspace/SquareLine_Project/ui/ui.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "cstdio"
#include "pimoroni_i2c.hpp"
#include "drivers/ft6x36/ft6x36.h"

using namespace pimoroni;

#define FRAME_WIDTH 240
#define FRAME_HEIGHT 240
static const uint BACKLIGHT = 45;
static const uint LCD_CLK = 26;
static const uint LCD_CS = 28;
static const uint LCD_DAT = 27;
static const uint LCD_DC = -1;
static const uint LCD_D0 = 1;

uint16_t back_buffer[FRAME_WIDTH * FRAME_HEIGHT];
uint16_t front_buffer[FRAME_WIDTH * FRAME_HEIGHT];

ST7701* presto;

void dummy_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    data->state = LV_INDEV_STATE_RELEASED;
}

int main()
{
    set_sys_clock_khz(240000, true);
    stdio_init_all();

    gpio_init(LCD_CS);
    gpio_put(LCD_CS, 1);
    gpio_set_dir(LCD_CS, 1);

    printf("Hello\n");

    presto = new ST7701(FRAME_WIDTH, FRAME_HEIGHT, ROTATE_0,
                        SPIPins{spi1, LCD_CS, LCD_CLK, LCD_DAT, PIN_UNUSED, LCD_DC, BACKLIGHT}, back_buffer);

    printf("Init: ...");
    presto->init();
    printf("Init: OK\n");

    static I2C i2c(30, 31, 100000);
    static FT6X36 touch(&i2c, 0x48);

    /*************************** LVGL ***********************************/

    lv_init();

    printf("LV Init done\r\n");

    lv_display_t *display = lv_display_create(FRAME_WIDTH, FRAME_HEIGHT);

    printf("LV display created\r\n");

    lv_display_set_buffers(display, front_buffer, back_buffer, sizeof(front_buffer), LV_DISPLAY_RENDER_MODE_DIRECT);

    printf("LV buffers created and set\r\n");

    lv_display_set_flush_cb(display, [](lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
        for (int i = area->y1 * FRAME_WIDTH * 2 ; i <= area->y2 * FRAME_WIDTH * 2 ; i += FRAME_WIDTH * 2)
        {
            for (int x = area->x1 * 2 ; x <= area->x2 * 2 ; x += 2)
            {
                uint8_t tmp = px_map[i + x];
                px_map[i + x] = px_map[i + x +1];
                px_map[i + x + 1] = tmp;
            }
        }
        presto->set_framebuffer((uint16_t *) px_map);
        lv_display_flush_ready(display);
    });

    printf("LV flush callback set\r\n");

//    xpt2046_init();

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, [](lv_indev_t *drv, lv_indev_data_t *data) {
        touch.ft6x36_read(drv, data);
    });

    /********************************************************************/

    printf("Init UI\r\n");

    ui_init();

    printf("Entering main loop\r\n");

    while (true)
    {
        lv_timer_handler();
        sleep_ms(1);
        lv_tick_inc(1);
    }
}
