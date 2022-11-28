#include <stdio.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "audio/audio_module.h"
#include "user_input.h"
#include "app/app_manager.h"
#include "display/display.h"


TaskHandle_t appManagerTaskHandle = NULL;
TaskHandle_t userInputHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;
TaskHandle_t audioTaskHandle = NULL;

void app_main(void)
{
    printf("Hello world!\n");

    xTaskCreatePinnedToCore(xAppManagerTask, "ApplicationManagerTask", 8092, NULL, 4, &appManagerTaskHandle, 0);
    xTaskCreatePinnedToCore(xAudioTask, "AudioTask", 8092, NULL, 2, &audioTaskHandle, 1);
    xTaskCreatePinnedToCore(xUserInputTask, "UserInputTask", 4096, NULL, 3, &userInputHandle, 0);
    //xTaskCreatePinnedToCore(xDisplayTask, "DisplayTask", 8092, NULL, 4, &displayTaskHandle, 1);


    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
