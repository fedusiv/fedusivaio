#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string.h>
#include "app_manager.h"
#include "system_message.h"
#include "audio/audio_config.h"

static sys_msg_t * message; // pointer to current message on interation

static void input_process();


void xAppManagerTask(void * task_parameter)
{

    system_message_module_init();

    while(1)
    {
        message = NULL;
        pull_message(MSG_DST_APP, &message);
        if(message != NULL)
        {
            input_process();
            relese_message(message);
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
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
            break;
        case INPUT_OP_BUTTON_PRESSED:
            printf("Button %d pressed\n", act.id);
            switch(act.id)
            {
                case 5:
                    note = NOTE_C;
                    break;
                case 4:
                    note = NOTE_D;
                    break;
                case 3:
                    note = NOTE_E;
                    break;
                case 2:
                    note = NOTE_F;
                    break;
                case 1:
                    note = NOTE_G;
                    break;
                default:
                    note = NOTE_ZERO;
                    break;
            }
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

    create_message(OP_PLAY_NOTE, &note, MSG_DST_AU);
    printf("Note %d requested\n", note);

    
}

