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
    static u32 last_process;
    u32 current_tick, current_duration;
    u32 cc,cc1;

    current_tick = get_sys_tick();
    current_duration = current_tick - last_process;

    if(current_duration < SAMPLE_SEND_PERIOD)
    {
        vTaskDelay(1/portTICK_PERIOD_MS);
        return;
    }

    // before sending pack everything
    audio_pack_samples(sample_pack, sample_l, sample_r);
    cc = get_sys_tick();
    audio_send(sample_pack);
    cc1 = get_sys_tick();
    printf("send %u\n", cc1-cc);
    last_process = get_sys_tick(); // take time of last transfer procedure
    synth_process(sample_l,sample_r); // after transfer calculate next buffer
    // once we calculated samples we can display them
    //create_message(OP_AUDIO_SAMPLES_PROCESSED, NULL, MSG_DST_APP,0); 
    

}


void xAudioTask(void * task_parameter)
{

    i2s_init();
    sound_engine_init();
    sample_l = get_audio_samples_l();
    sample_r = get_audio_samples_r();
    sample_pack = (audio_sample_packed_u*)get_memory_audio_send_buffer();

    while (1) {
        // threads communication region
        message = NULL;
        pull_message(MSG_DST_AU, &message);
        if(message != NULL)
        {
            process_message();
            relese_message(message);
        }
        // processing and sending audio data
        audio_process();
    }

}
