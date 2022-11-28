#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "audio_config.h"

static uint32_t notes_pitch[NOTES_COUNT];
static float sinewave_form[WAVEFORM_CNT];

void generate_sound(float * sample_l, float * sample_r, audio_sample_packed_u * sample_pack)
{
    uint32_t sample_pos = 0;
    float signal = 0.0f;

    memset(sample_l, 0, sizeof(float) * SAMPLES_BUFFER_SIZE);
    memset(sample_r, 0, sizeof(float) * SAMPLES_BUFFER_SIZE);
    for(int i = 0; i < SAMPLES_BUFFER_SIZE; i++)
    {
        sample_pos += notes_pitch[24];
        signal = sinewave_form[WAVEFORM_I(sample_pos)];
        sample_l[i] = signal * 0.1f;
        sample_r[i] = signal * 0.1f;
        sample_pack[i].channel[0] = (int16_t)(sample_l[i] * AUDIO_CONVERSION_VAL);
        sample_pack[i].channel[1] = (int16_t)(sample_r[i] * AUDIO_CONVERSION_VAL);
    }

}


void sound_engine_init()
{
    float tmp_value;
    uint32_t tmp_value2;
    uint16_t i = 0;

    for(i = 0; i < WAVEFORM_CNT; i++)
    {
        tmp_value = sinf(i * PI2 / WAVEFORM_CNT);
        sinewave_form[i] = tmp_value;
    }

    for(i = 0; i < NOTES_COUNT; i++)
    {
        tmp_value = ((pow(2.0f, (float)(i) / 12.0f) * NOTE_FREQ_BASE)); 
        tmp_value2 = (uint32_t)(tmp_value * ((float)(1ULL << 32ULL) / ((float)SAMPLE_RATE)));
        notes_pitch[i] = tmp_value2;
    }
}
