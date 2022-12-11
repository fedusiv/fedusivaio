#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"

#include "display.h"
#include "display_config.h"
#include "ili9341.h"
#include "memory/memory_manager.h"
#include "app/system_message.h" 
#include "op_codes.h"
#include "gui_types.h"

static sys_msg_t * message;
static uint16_t * s_screen_buffer;
static uint16_t * s_dma_buffer;
static int16_t * s_graph_data_buffer;

static void display_process_msg();
static void screen_update();
static void fill_screen(color_t color);
static void display_draw_soundwave_graph();
static void draw_pixel_area(vector2_t start_point, vector2_t end_point, color_t color);

void xDisplayTask(void * task_parameter)
{
    init_display(); // hardware init
    s_screen_buffer =  get_display_frame_buffer();
    s_dma_buffer = get_display_dma_buffer();
    color_t color = 
    {
        .r = 0x00,
        .g = 0x00,
        .b = 0x00,
    };
    fill_screen(color);
    color.r = 0xf7;
    vector2_t s = {.x = 50, .y = 50};
    vector2_t e = {.x = 300, .y = 150};
    draw_pixel_area(s,e,color);

    while(1)
    {
        message = NULL;
        pull_message(MSG_DST_GUI, &message);
        if(message != NULL)
        {
            display_process_msg();
            relese_message(message);
        }
        screen_update();
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}


static void display_process_msg()
{
    switch(message->op_code)
    {
        case OP_AUDIO_SAMPLES_PROCESSED:
            //display_draw_soundwave_graph();
            break;
        default:
            break;
    }
}

static void display_draw_soundwave_graph()
{
    static uint8_t count = 0;

    uint32_t * data_buf;
    int16_t *buffer =  s_graph_data_buffer + (count * 48);

    data_buf = get_memory_audio_send_buffer();
    for(uint8_t i = 0; i < 48; i++)
    {
        buffer[i] = data_buf[i] >> 16;
    }
    
    count++;
    if(count < 2)
    {
        return;
    }

    // draw
    count = 0;
    for(uint8_t i = 0; i < 96; i++)
    {
        
    }

    
}

void screen_update()
{
    uint16_t line_num;
    uint16_t * screen_buf_pnt;

    screen_buf_pnt = s_screen_buffer;

    for(line_num = 0; line_num < SCREEN_HEIGHT; line_num+= SCREEN_SEND_HEIGHT)
    {
        memcpy(s_dma_buffer, screen_buf_pnt, SCREEN_SEND_HEIGHT * SCREEN_WIDTH * 2);
        // blocking function
        send_lines(line_num, s_dma_buffer);
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

static void draw_pixel_area(vector2_t start_point, vector2_t end_point, color_t color)
{
    uint16_t width, height;
    uint16_t * start_pos;
    uint16_t colour;

    colour = convert_color(color);
    width = end_point.x - start_point.x;
    height = end_point.y - start_point.y;
    start_pos = s_screen_buffer + (SCREEN_WIDTH * start_point.y) + start_point.x;
    for(uint8_t y = 0; y < height; y++)
    {
        for(uint8_t x = 0; x < width; x++)
        {
            *start_pos = SPI_SWAP_DATA_TX(colour,16);
            start_pos++; // increase pointer to pixel place by 1 in width
        }
        start_pos += SCREEN_WIDTH - width;
    }

}

