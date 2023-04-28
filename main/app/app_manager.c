#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string.h>
#include "app_manager.h"
#include "system_message.h"
#include "audio/audio_config.h"
#include "gpio_config.h"
#include "timer.h"

static sys_msg_t * message; // pointer to current message on interation

static void input_process();
static void cmd_process(); // full process
static audio_note_e convert_button_to_note(buttons_id_e button);


void xAppManagerTask(void * task_parameter)
{
    system_message_module_init();

    while(1)
    {
        message = NULL;
        pull_message(MSG_DST_APP, &message);
        if(message != NULL)
        {
            cmd_process();
            relese_message(message);
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}


static void cmd_process()
{
    switch(message->op_code)
    {
        case OP_AUDIO_SAMPLES_PROCESSED:
            create_message(OP_AUDIO_SAMPLES_PROCESSED, NULL, MSG_DST_GUI, 10);
            break;

        // Input operations
        case OP_USER_INPUT_ACTION:
            input_process();
            break;
                   
        default:
            break;
    }
}

static void input_process()
{
    audio_note_e note = NOTE_ZERO; 

    input_action_t act;
    memcpy(&act, message->data, sizeof(input_action_t));
    switch (act.opcode) {
        case INPUT_OP_ZERO:
            printf("Button %d zero\n", act.id);
            break;
        case INPUT_OP_BUTTON_RELEASED:
            printf("Button %d released\n", act.id);
            note = convert_button_to_note(act.id);
            create_message(OP_NOTE_SET_OFF, (uint8_t*)&note, MSG_DST_AU, 10);
            break;
        case INPUT_OP_BUTTON_PRESSED:
            printf("Button %d pressed\n", act.id);
            if(act.id == BUTTON_ID_ENC_1 || act.id == BUTTON_ID_JS_1)   // temporary
            {
                break;
            }
            note = convert_button_to_note(act.id);
            create_message(OP_NOTE_SET_ON, (uint8_t*)&note, MSG_DST_AU, 10);
            break;
        case INPUT_OP_ENCODER_CCW:
            printf("Encoder %d ccw\n", act.id);
            break;
        case INPUT_OP_ENCODER_CW:
            printf("Encoder %d cw\n", act.id);
            break;
        default:
            break;
    }

}


static audio_note_e convert_button_to_note(buttons_id_e button)
{
    audio_note_e note;

    switch(button)
    {
        case BUTTON_ID_C:
            note = NOTE_C;
            break;
        case BUTTON_ID_Cb:
            note = NOTE_Cb;
            break;
        case BUTTON_ID_D:
            note = NOTE_D;
            break;
        case BUTTON_ID_Db:
            note = NOTE_Db;
            break;
        case BUTTON_ID_E:
            note = NOTE_E;
            break;
        case BUTTON_ID_F:
            note = NOTE_F;
            break;
        case BUTTON_ID_Fb:
            note = NOTE_Fb;
            break;
        case BUTTON_ID_G:
            note = NOTE_G;
            break;
        case BUTTON_ID_Gb:
            note = NOTE_Gb;
            break;
        case BUTTON_ID_A:
            note = NOTE_A;
            break;
        case BUTTON_ID_Ab:
            note = NOTE_Ab;
            break;
        case BUTTON_ID_B:
            note = NOTE_B;
            break;
        default:
            note = NOTE_ZERO;
            break;
    }

    return note;
}

