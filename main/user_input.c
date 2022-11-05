#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "hal/gpio_types.h"
#include "user_input.h"
#include "gpio_config.h"
#include "system_message.h"

typedef struct
{
    uint8_t off_state;
    uint8_t prev_state;
    uint8_t encoder;
    buttons_id_e id;
}button_status_t;

static button_status_t buttons_status[BUTTON_ID_MAX];

static void gpio_init();
static void buttons_init();
static void process_buttons();

void xUserInputTask(void * task_parameter)
{
    gpio_init();
    buttons_init();
    while(1)
    {
        process_buttons();
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

static void buttons_init()
{
    button_status_t * current;

    for(buttons_id_e i = 0; i < BUTTON_ID_MAX; i++)
    {
        current = &buttons_status[i];
        switch(i)
        {
            case BUTTON_ID_C:
            case BUTTON_ID_D:
            case BUTTON_ID_E:
            case BUTTON_ID_F:
            case BUTTON_ID_G:
                current->off_state = 0;
                current->encoder = 0;
                break;
            case BUTTON_ID_ENC_A:
            case BUTTON_ID_ENC_B:
                current->off_state = 1;
                current->encoder = 1;
                break;
            case BUTTON_ID_ENC_SWT:
                current->off_state = 1;
                current->encoder = 0;
                break;
            default:
                break;
        }
        current->prev_state = current->off_state;
        current->id = i;
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
    button_status_t * current_button;

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
        current_button = &buttons_status[i];
        current_state = (read_result >> i) & 1U;
        if(current_state != current_button ->prev_state)
        {

        }
        current_button->prev_state = current_state;

    }

}
