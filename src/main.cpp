#include "st7701.hpp"
#include "lvgl.h"
#include "SquareLineWorkspace/SquareLine_Project/ui/ui.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "cstdio"
#include "pimoroni_i2c.hpp"
#include "drivers/ft6x36/ft6x36.h"
extern "C" {
#include "sfe_psram.h"
#include "sfe_pico_alloc.h"
}

using namespace pimoroni;

#define FRAME_WIDTH 480
#define FRAME_HEIGHT 480
#define DRAW_BUF_SIZE (FRAME_WIDTH * FRAME_HEIGHT * sizeof(uint16_t) / 10)

static const uint BACKLIGHT = 45;
static const uint LCD_CLK = 26;
static const uint LCD_CS = 28;
static const uint LCD_DAT = 27;
static const uint LCD_DC = -1;
static const uint LCD_D0 = 1;

uint16_t front_buffer[FRAME_WIDTH * FRAME_HEIGHT];
uint16_t *draw_buffer;

ST7701* presto;

static void memory_stats()
{
    size_t mem_size = sfe_mem_size();
    size_t mem_used = sfe_mem_used();
    printf("\tMemory pool - Total: 0x%X (%u)  Used: 0x%X (%u) - %3.2f%%\n", mem_size, mem_size, mem_used, mem_used,
           (float)mem_used / (float)mem_size * 100.0);

    size_t max_block = sfe_mem_max_free_size();
    printf("\tMax free block size: 0x%X (%u) \n", max_block, max_block);
}

int main()
{
    set_sys_clock_khz(240000, true);
    stdio_init_all();

    gpio_init(LCD_CS);
    gpio_put(LCD_CS, 1);
    gpio_set_dir(LCD_CS, 1);

    sleep_ms(200);

    sfe_setup_psram(47);
    sfe_pico_alloc_init();

    memory_stats();

    draw_buffer = (uint16_t *)malloc(DRAW_BUF_SIZE);

    presto = new ST7701(FRAME_WIDTH, FRAME_HEIGHT, ROTATE_0,
                        SPIPins{spi1, LCD_CS, LCD_CLK, LCD_DAT, PIN_UNUSED, LCD_DC, BACKLIGHT}, front_buffer);

    printf("Init: ...");
    presto->init();
    printf("Init: OK\n");

    static I2C i2c(30, 31, 100000);
    static FT6X36 touch(&i2c, 0x48);

    /*************************** LVGL ***********************************/

    lv_init();

    lv_display_t *display = lv_display_create(FRAME_WIDTH, FRAME_HEIGHT);

    lv_display_set_buffers(display, draw_buffer, nullptr, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(display, [](lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
        long w = area->x2 - area->x1 + 1;
        auto outbuf = (uint8_t *)front_buffer;

        for (int i = area->y1 * FRAME_WIDTH * 2, ii = 0 ; i <= area->y2 * FRAME_WIDTH * 2 ; i += FRAME_WIDTH * 2, ii += w * 2)
        {
            for (int x = area->x1 * 2, xi = 0 ; x <= area->x2 * 2 ; x += 2, xi += 2)
            {
                outbuf[i + x] = px_map[ii + xi + 1];
                outbuf[i + x + 1] = px_map[ii + xi];
            }
        }
        lv_display_flush_ready(display);
    });

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, [](lv_indev_t *drv, lv_indev_data_t *data) {
        touch.ft6x36_read(drv, data);
    });

    /********************************************************************/

    // Uncomment if using SquareLineStudio, else create your UI here
    // ui_init();

    while (true)
    {
        lv_timer_handler();
        sleep_ms(1);
        lv_tick_inc(1);
    }
}
