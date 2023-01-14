#ifndef __SOUND_OSC_H__
#define __SOUND_OSC_H__

#include "types.h"
#include "audio_config.h"

typedef struct _sound_osc_struct_t
{
    u8 active;
    audio_waveform_types_e osc_type;
    float amp;
    i8 pitch;
}sound_osc_struct_t;


void init_osc();
u32 calculate_osc(sound_osc_struct_t * osc_array, u32 sample_pos, u16 note_id, float * signal);

#endif // __SOUND_OSC_H__
