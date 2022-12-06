#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"

#include "display.h"
#include "display_config.h"
#include "ili9341.h"

static uint16_t * s_screen_buffer;
DRAM_ATTR uint16_t s_frame_buffer[SCREEN_WIDTH * SCREEN_SEND_HEIGHT];

static void screen_update();
static void fill_screen(color_t color);

void xDisplayTask(void * task_parameter)
{
    init_display();
    s_screen_buffer = (uint16_t *)heap_caps_malloc(SCREEN_SIZE * sizeof(uint16_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    color_t color = 
    {
        .r = 0x00,
        .g = 0xfa,
        .b = 0xfa,
    };
    if(s_screen_buffer  == NULL)
    {
        printf("Memory for screen is not allocated\n");
        abort();
    }
    fill_screen(color);
    while(1)
    {
        screen_update();
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}


void screen_update()
{
    uint16_t line_num;
    uint16_t * screen_buf_pnt;

    screen_buf_pnt = s_screen_buffer;

    for(line_num = 0; line_num < SCREEN_HEIGHT; line_num+= SCREEN_SEND_HEIGHT)
    {
        memcpy(s_frame_buffer, screen_buf_pnt, SCREEN_SEND_HEIGHT * SCREEN_WIDTH * 2);
        // blocking function
        send_lines(line_num, s_frame_buffer);
        screen_buf_pnt += SCREEN_WIDTH * SCREEN_SEND_HEIGHT;
    }
}

void fill_screen(color_t color)
{
    uint16_t colour = convert_color(color);
    for(int i =0; i < SCREEN_SIZE; i++)
    {
        s_screen_buffer[i] = SPI_SWAP_DATA_TX(colour,16);
    }
}

