#ifndef __SOUND_ADSR_H__
#define __SOUND_ADSR_H__
#include <stdint.h>

typedef enum _adsr_state_e
{
    ADSR_ATTACK,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE,
} adsr_state_e;

typedef struct _adsr_struct_t
{
    adsr_state_e state;
    float last_ampl;
    
    float attack_duration;
    float attack_step;
    float attack_ampl;

    float decay_duration;
    float decay_step;
    float decay_ampl;

    float release_duration;
    float release_step;

} adsr_struct_t;

void adsr_init(adsr_struct_t * parameter, adsr_struct_t * adsr);
float adsr_process(adsr_struct_t * adsr);

#endif // __SOUND_ADSR_H__
