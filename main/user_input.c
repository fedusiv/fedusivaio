#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "user_input.h"
#include "gpio_config.h"
#include "system_message.h"

static void encoder_init();
static void encoder_process();

typedef struct
{
    uint32_t pin_clk;
    uint32_t pin_data;
    uint32_t pin_switch;
    uint8_t clk_value;
    uint8_t data_value;
    uint8_t switch_debouce;
}encoder_status_t;

encoder_status_t encoder_1;

void xUserInputTask(void * task_parameter)
{
    encoder_init();
    while(1)
    {
        encoder_process();
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}


static void encoder_init()
{
    
    encoder_1.pin_clk = ENCODER_B;
    encoder_1.pin_data = ENCODER_A;
    encoder_1.pin_switch = ENCODER_SWT;
    gpio_set_direction(encoder_1.pin_clk, GPIO_MODE_INPUT);
    gpio_set_direction(encoder_1.pin_data, GPIO_MODE_INPUT);
    gpio_set_direction(encoder_1.pin_switch, GPIO_MODE_INPUT);

    encoder_1.switch_debouce = 0;
    encoder_1.clk_value = gpio_get_level(encoder_1.pin_clk);
}

static void encoder_process()
{
    uint8_t current_clk_value;

    // Rotation
    current_clk_value = gpio_get_level(encoder_1.pin_clk);
    if(current_clk_value != encoder_1.clk_value && current_clk_value == 1)
    {
        // encoder Tick appeared
        encoder_1.data_value = gpio_get_level(encoder_1.pin_data);
        if( encoder_1.data_value != current_clk_value )
        {
            // CCW
            put_message(OP_ENCODER_CCW);
        }
        else
        {
            //CW
            put_message(OP_ENCODER_CW);
        }
    }
    encoder_1.clk_value = current_clk_value;

    // Button
    if( gpio_get_level(encoder_1.pin_switch) )
    {
        encoder_1.switch_debouce++;
        if(encoder_1.switch_debouce > 50)
        {
            encoder_1.switch_debouce = 0;
            // Button pressed
            put_message(OP_BUTTON_PRESSED);
        }
    }
}