#include <stdio.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#include "config.h" 
#include "display.h"



TaskHandle_t displayTaskHandle = NULL;

void app_main(void)
{
    xTaskCreatePinnedToCore(xDisplayTask, "DisplayTask", DISPLAY_TASK_STACK_SIZE, NULL, 4, &displayTaskHandle, 1); 
}
