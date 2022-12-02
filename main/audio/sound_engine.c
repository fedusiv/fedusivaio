#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "audio_config.h"
#include "sound_adsr.h"

typedef struct _synth_voice_t
{
    uint8_t active;
    uint16_t note_id;
    uint32_t sample_pos;
    adsr_struct_t adsr;
} synth_voice;

static uint32_t notes_pitch[NOTES_COUNT];
static float sinewave_form[WAVEFORM_CNT];
static synth_voice s_synth_voices[POLYPHONY_AMOUNT];
static adsr_struct_t s_adsr_parameters;
static audio_octaves_e s_current_octave;
static float s_common_volume;


static synth_voice * get_free_synth_voice();


void synth_note_on(uint16_t note_id)
{
    synth_voice * voice;

    voice = get_free_synth_voice();
    if(voice == NULL)
    {
        // no available synth voice
        return;
    }
    voice->active = 1;
    voice->note_id = note_id;
    voice->sample_pos = 0;
    adsr_init(&s_adsr_parameters, &voice->adsr);

}

void synth_note_off(uint16_t note_id)
{
    synth_voice * voice;
    for(uint8_t i = 0; i < POLYPHONY_AMOUNT; i++)
    {
        voice = &s_synth_voices[i];
        if(voice->active == 0)
        {
            continue;
        }
        if(voice->note_id == note_id)
        {
            voice->adsr.state = ADSR_RELEASE;
            break;
        }
    }
}


void synth_process(float * sample_l, float * sample_r, audio_sample_packed_u * sample_pack)
{
    float signal = 0.0f;
    float adsr_ampl = 0.0f;
    uint16_t i_s, i_v; // index sample, index voice
    synth_voice * voice;

    memset(sample_l, 0, sizeof(float) * SAMPLES_BUFFER_SIZE);
    memset(sample_r, 0, sizeof(float) * SAMPLES_BUFFER_SIZE);
    for(i_s = 0; i_s < SAMPLES_BUFFER_SIZE; i_s++)
    {
        for(i_v=0; i_v < POLYPHONY_AMOUNT; i_v++)
        {
            voice = &s_synth_voices[i_v];
            if(voice->active == 0)
            {
                continue;
            }
            voice->sample_pos += notes_pitch[voice->note_id];
            signal = sinewave_form[WAVEFORM_I(voice->sample_pos)];
            // adsr part
            if(i_s % ADSR_PROCESS_FREQ == 0)
            {
                adsr_ampl = adsr_process(&voice->adsr);
                if(adsr_ampl <= 0.0f)
                {
                    // turn off voice
                    adsr_ampl = 0.0f;
                    voice->active = 0;
                }
            }
            sample_l[i_s] += (adsr_ampl * signal * s_common_volume);
            sample_r[i_s] += (adsr_ampl * signal * s_common_volume);
        }
        sample_pack[i_s].channel[0] = (int16_t)(sample_l[i_s] * AUDIO_CONVERSION_VAL);
        sample_pack[i_s].channel[1] = (int16_t)(sample_r[i_s] * AUDIO_CONVERSION_VAL);
    }

}

synth_voice * get_free_synth_voice()
{
    synth_voice * res = NULL;
    uint8_t i = 0;

    for(i=0; i < POLYPHONY_AMOUNT; i++)
    {
        if(s_synth_voices[i].active == 0)
        {
            res = &s_synth_voices[i];
            break;
        }
    }
    return res;
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

    for(i=0; i < POLYPHONY_AMOUNT; i++)
    {
        s_synth_voices[i].active = 0;
    }

    s_current_octave = OCTAVE_4;
    s_common_volume = 0.2;

    s_adsr_parameters.attack_duration = 0.1f;
    s_adsr_parameters.attack_ampl = 1.0f;
    s_adsr_parameters.decay_duration = 0.1f;
    s_adsr_parameters.decay_ampl = 0.4f;
    s_adsr_parameters.release_duration = 0.2f;

}

uint16_t get_note_id(audio_note_e note)
{
    return (s_current_octave - OCTAVE_1) * 12 + note + NOTE_OFFSET;
}
