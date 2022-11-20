#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "system_message.h"
#include "audio_module.h"
#include "audio_config.h"
#include "audio_hw.h"
#include "sound_engine.h"
#include "common.h"

static sys_msg_t * message; // pointer to current message on interation
static float s_buffer[SAMPLES_BUFFER_SIZE] = {0.0};

static void play_note(uint8_t* data);
static void process_message();

void play_note(uint8_t* data)
{
    audio_note_e note;

    memcpy(&note, data, sizeof(audio_note_e));

    float note_freq = get_note_freq(note, OCTAVE_4);
    generate_sound(note_freq, s_buffer);
}

void process_message()
{
    switch(message->op_code)
    {
        case OP_PLAY_NOTE:
            play_note(message->data);
            break;
        default:
            break;
    }
}

void xAudioTask(void * task_parameter)
{
    i2s_init();
    
    while (1) {
        message = NULL;
        pull_message(MSG_DST_AU, &message);
        if(message != NULL)
        {
            process_message();
            relese_message(message);
        }
        audio_send(s_buffer);
    }

}
