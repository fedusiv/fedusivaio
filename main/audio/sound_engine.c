#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "audio_config.h"
#include "sound_adsr.h"
#include "sound_osc.h"

typedef struct _synth_voice_t
{
    u8 active;
    u16 note_id;
    adsr_struct_t adsr;
    sound_osc_struct_t osc[OSC_AMOUNT];
} synth_voice_t;

static synth_voice_t s_synth_voices[POLYPHONY_AMOUNT];
static adsr_struct_t s_adsr_parameters;
static audio_octaves_e s_current_octave;
static sound_osc_struct_t s_osc_array[OSC_AMOUNT];
static float s_common_volume;

synth_voice_t  * get_free_synth_voice(uint16_t note_id);

void synth_note_on(uint16_t note_id)
{
    synth_voice_t   * voice;

    voice = get_free_synth_voice(note_id);
    if(voice == NULL)
    {
        // no available synth voice
        return;
    }
    voice->active = 1;
    // initialize oscs
    memcpy(voice->osc, s_osc_array, sizeof(sound_osc_struct_t) * OSC_AMOUNT);
    voice->note_id = note_id;
    adsr_init(&s_adsr_parameters, &voice->adsr);

}

void synth_note_off(uint16_t note_id)
{
    synth_voice_t * voice;
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


void synth_process(float * sample_l, float * sample_r)
{
    float signal = 0.0f;
    float adsr_ampl = 0.0f;
    uint16_t i_s, i_v; // index sample, index voice
    synth_voice_t  * voice;

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
            // All osc calculation is in this function
            signal = calculate_osc(voice->osc, voice->note_id);

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
    }

}

synth_voice_t * get_free_synth_voice(uint16_t note_id)
{
    synth_voice_t * res = NULL;
    uint8_t i = 0;

    for(i=0; i < POLYPHONY_AMOUNT; i++)
    {
        if(s_synth_voices[i].active)
        {
            if(s_synth_voices[i].note_id == note_id)
            {
                // this note is already sounds, so we return this voice for it.
                res = &s_synth_voices[i];
                break;
            }

        }
        else if(res == NULL)
        {
            res = &s_synth_voices[i];
        }
    }
    return res;
}

void sound_engine_init()
{
    uint16_t i = 0;

    init_osc();

    for(i=0; i < POLYPHONY_AMOUNT; i++)
    {
        s_synth_voices[i].active = 0;
    }

    s_current_octave = OCTAVE_5;
    s_common_volume = 0.2;

    s_adsr_parameters.attack_duration = 0.1f;
    s_adsr_parameters.attack_ampl = 1.0f;
    s_adsr_parameters.decay_duration = 0.1f;
    s_adsr_parameters.decay_ampl = 0.4f;
    s_adsr_parameters.release_duration = 0.1f;

    s_osc_array[0].active = 1;
    s_osc_array[0].amp = 1.0f;
    s_osc_array[0].osc_type = WAVEFORM_TYPE_SINE;
    s_osc_array[0].pitch = 0;
    s_osc_array[0].sample_pos = 0;

    s_osc_array[1].active = 1;
    s_osc_array[1].amp = .3f;
    s_osc_array[1].osc_type = WAVEFORM_TYPE_SAW;
    s_osc_array[1].pitch = -12;
    s_osc_array[1].sample_pos = 0;

    s_osc_array[2].active = 0;
    s_osc_array[2].amp = .3f;
    s_osc_array[2].osc_type = WAVEFORM_TYPE_SQUARE;
    s_osc_array[2].pitch = 12;
    s_osc_array[2].sample_pos = 0;

    s_osc_array[3].active = 0;
    s_osc_array[3].sample_pos = 0;
}

uint16_t get_note_id(audio_note_e note)
{
    return (s_current_octave - OCTAVE_1) * 12 + note + NOTE_OFFSET;
}
