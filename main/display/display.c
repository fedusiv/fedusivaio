#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "display.h"
#include "ili9341.h"

#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240

DRAM_ATTR uint16_t frame_buffer[SCREEN_HEIGHT*20];

void xDisplayTask(void * task_parameter)
{
    uint16_t color = convert_color(0,0,0xff);
    init_display();
    memset(frame_buffer, color, sizeof(uint16_t) * SCREEN_HEIGHT * 20); 
    uint16_t start_line = 0;
    while(1)
    {
        for(int i = start_line; i < 120 + start_line; i += 20){
        send_lines(i, frame_buffer);
        }
        
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}
