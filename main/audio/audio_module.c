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

// allocated fields
static audio_sample_packed_u sample_pack[SAMPLES_BUFFER_SIZE];
static float sample_l[SAMPLES_BUFFER_SIZE];
static float sample_r[SAMPLES_BUFFER_SIZE];

static sys_msg_t * message; // pointer to current message on interation
float note_freq = 0.0;

static void play_note(uint8_t* data);
static void process_message();

void play_note(uint8_t* data)
{
    audio_note_e note;

    memcpy(&note, data, sizeof(audio_note_e));
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
    sound_engine_init();


    while (1) {
        message = NULL;
        pull_message(MSG_DST_AU, &message);
        if(message != NULL)
        {
            process_message();
            relese_message(message);
        }
        generate_sound(sample_l, sample_r, sample_pack);
        audio_send(sample_pack);
    }

}
