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
    init_display();
    memset(frame_buffer, 0xFDA0, sizeof(uint16_t) * SCREEN_HEIGHT * 20); 
    while(1)
    {
        for(int i = 0; i < 240; i += 20){
        send_lines(i, frame_buffer);
        }
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}
