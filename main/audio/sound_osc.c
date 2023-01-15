#include <math.h>
#include <stdio.h>

#include "sound_osc.h"
#include "audio_config.h"
#include "types.h"
#include "memory_manager.h"

static float * s_waveform_data[WAVEFORM_TYPE_MAX];
static uint32_t notes_pitch[NOTES_COUNT];

void init_osc()
{
    u16 i;
    float tmp_value;
    uint32_t tmp_value2;

    get_audio_osc_waveforms(s_waveform_data); // obtain memory

    // calculate all oscillators type.
    for(i = 0; i < WAVEFORM_CNT; i++)
    {
        tmp_value = sinf(i * PI2 / WAVEFORM_CNT);
        s_waveform_data[WAVEFORM_TYPE_SINE][i] = tmp_value;
        s_waveform_data[WAVEFORM_TYPE_SAW][i] = (2.0f * ((float)i) / ((float)WAVEFORM_CNT)) - 1.0f;
        s_waveform_data[WAVEFORM_TYPE_SQUARE][i] = (i > (WAVEFORM_CNT / 2)) ? 1 : -1;
    }

    for(i = 0; i < NOTES_COUNT; i++)
    {
        tmp_value = ((pow(2.0f, (float)(i) / 12.0f) * NOTE_FREQ_BASE)); 
        tmp_value2 = (u32)(tmp_value * ((float)(1ULL << 32ULL) / ((float)SAMPLE_RATE)));
        notes_pitch[i] = tmp_value2;
    }

}

float calculate_osc(sound_osc_struct_t * osc_array, u16 note_id)
{
    float cur_signal = 0.0f;

    sound_osc_struct_t * osc;
    for(u8 i = 0; i < OSC_AMOUNT; i++)
    {
        osc = &osc_array[i];
        if(osc->active == 0)
        {
            continue;
        }
        osc->sample_pos += notes_pitch[note_id + osc->pitch]; // TODO: need to add limit of existing limit for octaves
        cur_signal += osc->amp * s_waveform_data[osc->osc_type][WAVEFORM_I(osc->sample_pos)];
    }

    return cur_signal;
}
