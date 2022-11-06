#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "hal/gpio_types.h"
#include "user_input.h"
#include "gpio_config.h"
#include "system_message.h"

#define PRESS_BUTTON_DEBOUNCE 5
#define RELEASE_BUTTON_DEBOUNCE 2

typedef struct
{
    uint8_t off_state; // in which state button is unpressed, logic 0
    uint8_t prev_state; // previous state of button
    uint8_t curr_db_press; // current debouce for pressing
    uint8_t curr_db_release; // current debouce for releasing
    uint8_t pos_in_register; // position in shift register to be read
    buttons_id_e id; // button id
}button_state_t;

typedef struct
{
    uint8_t prev_state_a;   // previous of a state (data pin)
    uint8_t prev_state_b;   // previous state of a b pin(clk pin)
    uint8_t pos_in_register; // position in shift register, assume that it takes two position, next will be also for this encoder
    encoders_id_e id; // encoder id
}encoder_state_t;

button_state_t buttons_state[BUTTON_ID_MAX]=
{
    {0,0,0,0,7, BUTTON_ID_C},
    {0,0,0,0,6, BUTTON_ID_D},
    {0,0,0,0,5, BUTTON_ID_E},
    {0,0,0,0,4, BUTTON_ID_F},
    {0,0,0,0,3, BUTTON_ID_G},
    {1,1,0,0,1, BUTTON_ID_ENC_1},
};

static void gpio_init();
static void process_buttons();

void xUserInputTask(void * task_parameter)
{
    gpio_init();
    while(1)
    {
        process_buttons();
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}


static void gpio_init()
{
    gpio_set_direction(INPUT_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(INPUT_TRIG, GPIO_MODE_OUTPUT);
    gpio_set_direction(INPUT_SIN, GPIO_MODE_INPUT);

    gpio_set_pull_mode(INPUT_CLK,GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(INPUT_TRIG,GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(INPUT_SIN,GPIO_PULLDOWN_ONLY);
}

static void process_buttons()
{
    uint8_t current_state = 0;
    uint8_t read_gpio = 0;
    uint16_t read_result = 0x00;
    uint8_t provide_btn_change = 0; // flag. if 0 do not need to do anything. if 1 need to set, that button changed it's state
    button_state_t * button;

    gpio_set_level(INPUT_CLK, 1);
    gpio_set_level(INPUT_TRIG, 0);
    gpio_set_level(INPUT_CLK, 0);
    gpio_set_level(INPUT_TRIG, 1);
    // now state is saved

    // reading peripheral
    for(buttons_id_e i = 0; i < BUTTON_ID_MAX; i++)
    {
        read_gpio  = gpio_get_level(INPUT_SIN);
        read_result = (read_result & ~(1UL << i)) | (read_gpio << i);
        gpio_set_level(INPUT_CLK, 1);
        gpio_set_level(INPUT_CLK, 0);
    }

    // logic parsing
    // 74165hc sends from h to a 
    for(buttons_id_e i = 0; i < BUTTON_ID_MAX; i++)
    {
        provide_btn_change = 0; // reset flag
        button = &buttons_state[i];
        current_state = (read_result >> button->pos_in_register) & 1U;
        // operation of checking that button changed it's state
        if(current_state != button->prev_state)
        {
            // operation for pressing button
            if(current_state != buttons_state->off_state)
            {
                button->curr_db_press++;
                if(button->curr_db_press >= PRESS_BUTTON_DEBOUNCE)
                {
                    provide_btn_change = 1;
                }
            }
            else // operation for release button
            {
                button->curr_db_release++;
                if(button->curr_db_press >= RELEASE_BUTTON_DEBOUNCE )
                {
                    provide_btn_change = 1;
                }
            }
        }

        if(provide_btn_change)
        {
            button->prev_state = current_state;
            // send notification to application manager
        }

    }

}
