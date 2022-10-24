#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "user_input.h"


void xUserInputTask(void * task_parameter)
{
    int counter = 0;

    //gpio_set_direction(GPIO_NUM_26, GPIO_MODE_INPUT);

    while(1)
    {
        int level = gpio_get_level(GPIO_NUM_26);
        if(level == 1)
        {
            counter++;
            if(counter > 5)
            {
                counter = 0;
            }
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}
