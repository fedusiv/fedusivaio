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
    u32 sample_pos;
}sound_osc_struct_t;


void init_osc();
float calculate_osc(sound_osc_struct_t * osc_array, u16 note_id);

#endif // __SOUND_OSC_H__
