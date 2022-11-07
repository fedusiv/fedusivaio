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
#define SHIFT_REGISTER_OUTPUTS 8 // amount of all outputs connected to shift register

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
    uint8_t prev_state_a;   // previous of state a pin
    uint8_t prev_state_b;   // previous state of b pin
    uint8_t pos_a; // position of a pin of encoder in register
    uint8_t pos_b; // position of b pin of encoder in register
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

encoder_state_t encoders_state[ENCODER_ID_MAX]=
{
    {0,0,2,1,ENCODER_ID_1},
};

static void gpio_init();
static void process_buttons();
static void buttons_encoders_init();
static uint32_t hw_read_inputs();

void xUserInputTask(void * task_parameter)
{
    gpio_init();
    buttons_encoders_init();
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

static void buttons_encoders_init()
{
    uint32_t read_result = 0x00;
    encoder_state_t * encoder;

    // set starting values for encoders
    read_result = hw_read_inputs();
    for(encoders_id_e i = 0; i < ENCODER_ID_MAX; i++)
    {
        encoder = &encoders_state[i];
        encoder->prev_state_a = (read_result >> encoder->pos_a) & 1U;
        encoder->prev_state_b = (read_result >> encoder->pos_b) & 1U;
    }
}

static uint32_t hw_read_inputs()
{
    uint8_t read_gpio = 0;
    uint32_t read_result = 0x00;

    gpio_set_level(INPUT_CLK, 1);
    gpio_set_level(INPUT_TRIG, 0);
    gpio_set_level(INPUT_CLK, 0);
    gpio_set_level(INPUT_TRIG, 1);
    // now state is saved(locked) inside shift register

    // reading peripheral
    for(int i = 0; i < SHIFT_REGISTER_OUTPUTS; i++)
    {
        read_gpio  = gpio_get_level(INPUT_SIN);
        read_result = (read_result & ~(1UL << i)) | (read_gpio << i);
        gpio_set_level(INPUT_CLK, 1);
        gpio_set_level(INPUT_CLK, 0);
    }

    return read_result;
}

static void process_buttons()
{
    uint8_t current_state = 0; // for button
    uint8_t current_state_a = 0; // for encoder
    uint8_t current_state_b = 0;// for encoder
    uint32_t read_result = 0x00;
    uint8_t provide_btn_change = 0; // flag. if 0 do not need to do anything. if 1 need to set, that button changed it's state
    button_state_t * button;
    encoder_state_t * encoder;

    // reading inputs
    read_result = hw_read_inputs();

    // logic parsing
    // Encoders parsing
    for(encoders_id_e i = 0; i < ENCODER_ID_MAX; i++)
    {
        encoder = &encoders_state[i];
        current_state_a = (read_result >> encoder->pos_a) & 1U;
        current_state_b = (read_result >> encoder->pos_b) & 1U; 
        if(current_state_a != encoder->prev_state_a)
        {
            if(current_state_b != current_state_a)
            {
                // ccw send
            }
            else
            {
                // cw send
            }
        }
        encoder->prev_state_a = current_state_a;
        encoder->prev_state_b = current_state_b;
    }
    // Buttons parsing
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

