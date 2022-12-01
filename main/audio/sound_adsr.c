#include <string.h>
#include "sound_adsr.h"
#include "audio_config.h"

static float s_adsr_time_step = (float)ADSR_PROCESS_FREQ / SAMPLE_RATE;

static float calculate_adsr_step(float duration, float start_ampl, float end_ampl);

float adsr_process(adsr_struct_t * adsr)
{
    float ampl = 0.0f;

    adsr->step++;
    switch(adsr->state)
    {
        case ADSR_ATTACK:
        {
            ampl = adsr->step * adsr->attack_step;
            if(ampl >= adsr->attack_ampl)
            {
                adsr->state = ADSR_DECAY;
                adsr->step = 0;
            }
            break;
        }
        case ADSR_DECAY:
        {
            ampl = adsr->step * adsr->decay_step;
            if(ampl <= adsr->decay_ampl)
            {
                adsr->state = ADSR_SUSTAIN;
                adsr->step = 0;
            }
            break;
        }
        case ADSR_RELEASE:
        {
            ampl = adsr->step * adsr->release_step;
            break;
        }
        
        case ADSR_SUSTAIN:
        default:
            ampl = adsr->decay_ampl; // sustain btw
            break;
    }

    return ampl;
}

float calculate_adsr_step(float duration, float start_ampl, float end_ampl)
{
    float step = 1.0f;
    uint32_t step_amount;
    // linear
    step_amount = duration / s_adsr_time_step;
    step = (end_ampl - start_ampl) / step_amount; 

    return step;
}

void adsr_init(adsr_struct_t * parameter, adsr_struct_t * adsr)
{

    memcpy(adsr, parameter, sizeof(adsr_struct_t));
    adsr->step = 0;
    adsr->state = ADSR_ATTACK;

    adsr->attack_step = calculate_adsr_step(adsr->attack_duration, 0.0f, adsr->attack_ampl);
    adsr->decay_step = calculate_adsr_step(adsr->decay_duration, adsr->attack_ampl, adsr->decay_ampl);
    adsr->release_step = calculate_adsr_step(adsr->release_duration, adsr->decay_ampl, 0.0f);

}
