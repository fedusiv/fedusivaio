#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "system_message.h"
#include "audio_module.h"
#include "audio_config.h"
#include "sound_engine.h"
#include "common.h"

static sys_msg_t * message; // pointer to current message on interation
static float s_buffer[SAMPLES_BUFFER_SIZE];


void play_note(audio_note_e note)
{
    float note_freq = get_note_freq(note, OCTAVE_4);
    generate_sound(note_freq, s_buffer);
    audio_send(s_buffer);
}

void xAudioTask(void * task_parameter)
{
    sys_msg_op_code_e op_code;

    while (1) {
        message = NULL;
        pull_message(MSG_DST_AU, &message);
        if(message != NULL)
        {
            relese_message(message);
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }

}
