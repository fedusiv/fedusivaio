#include <stdio.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "audio_module.h"


TaskHandle_t displayTaskHandle = NULL;
TaskHandle_t audioTaskHandle = NULL;

void app_main(void)
{
    printf("Hello world!\n");

    xTaskCreatePinnedToCore(xAudioTask, "AudioTask", 8092, NULL, 4, &audioTaskHandle , 1);

    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
