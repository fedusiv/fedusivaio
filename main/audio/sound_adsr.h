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

typedef struct _adsr_element_t
{
    float duration; // whole duration
    float step; // step to change amplitude
    float amplitude; // destination amplitude
    float ampl_step; // how amplitude changes in one step
} adsr_element_t;

typedef struct _adsr_struct_t
{
    adsr_state_e state;
    adsr_element_t attack;
    adsr_element_t decay;
    adsr_element_t release;
} adsr_struct_t;

#endif // __SOUND_ADSR_H__
