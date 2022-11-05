#include <stdio.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "audio_module.h"
#include "user_input.h"
#include "system_message.h"
#include "display/display.h"


TaskHandle_t userInputHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;
TaskHandle_t audioTaskHandle = NULL;

void app_main(void)
{
    printf("Hello world!\n");

    system_message_module_init();

    xTaskCreatePinnedToCore(xAudioTask, "AudioTask", 8092, NULL, 4, &audioTaskHandle , 0);
    xTaskCreatePinnedToCore(xUserInputTask, "UserInputTask", 4096, NULL, 3, &userInputHandle , 0);
    xTaskCreatePinnedToCore(xDisplayTask, "DisplayTask", 8092, NULL, 4, &displayTaskHandle , 1);


    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
