#ifndef __SOUND_ENGINE_H__
#define __SOUND_ENGINE_H__

#include "audio_config.h"

void sound_engine_init();
void generate_sound(float * sample_l, float * sample_r, audio_sample_packed_u * sample_pack);

#endif // __SOUND_ENGINE_H__
