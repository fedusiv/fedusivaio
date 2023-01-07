#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "types.h"
#include "timer.h"
#include "system_message.h"
#include "audio_module.h"
#include "audio_config.h"
#include "audio_hw.h"
#include "sound_engine.h"
#include "../memory/memory_manager.h"

// allocated fields
audio_sample_packed_u * sample_pack;
static float * sample_l;
static float * sample_r;

static sys_msg_t * message; // pointer to current message on interation
static u32 s_last_send_tick;
static u32 s_send_period_wait;
float note_freq = 0.0;

static void audio_process();
static void set_note_state(u8 * data, u8 state);
static void process_message();

void set_note_state(u8 * data, u8 state)
{
    audio_note_e note;
    uint16_t note_id;

    memcpy(&note, data, sizeof(audio_note_e));
    note_id = get_note_id(note);
    if(state)
    {
        synth_note_on(note_id);
    }
    else
    {
        synth_note_off(note_id);
    }
}


void process_message()
{
    switch(message->op_code)
    {
        case OP_NOTE_SET_ON:
            set_note_state(message->data,1);
            break;
        case OP_NOTE_SET_OFF:
            set_note_state(message->data,0);
            break;
        default:
            break;
    }
}


void audio_process()
{
    u32 curr_tick, send_time;

    curr_tick = get_sys_tick();
    if(curr_tick - s_last_send_tick > s_send_period_wait)
    {
        s_last_send_tick = get_sys_tick();
        send_time = get_sys_tick();
        audio_send(sample_pack);
        send_time = get_sys_tick() - send_time;
        if(send_time > 100)
        {
            s_send_period_wait = SAMPLE_SEND_PERIOD;
        }
        else
        {
            s_send_period_wait = 4000;
        }
        synth_process(sample_l,sample_r); // after transfer calculate next buffer
        audio_pack_samples(sample_pack, sample_l, sample_r);
    }
}

void xAudioTask(void * task_parameter)
{

    i2s_init();
    sound_engine_init();
    sample_l = get_audio_samples_l();
    sample_r = get_audio_samples_r();
    sample_pack = (audio_sample_packed_u*)get_memory_audio_send_buffer();

    // prepare first zero samples to be sent
    synth_process(sample_l,sample_r);
    audio_pack_samples(sample_pack, sample_l, sample_r);
    s_send_period_wait = SAMPLE_SEND_PERIOD;
    while (1) {
        audio_process();
        // threads communication region
        message = NULL;
        pull_message(MSG_DST_AU, &message);
        if(message != NULL)
        {
            process_message();
            relese_message(message);
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }

}
