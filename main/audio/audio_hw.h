#ifndef __AUDIO_HW_H__
#define __AUDIO_HW_H__

#include "audio_config.h"
#include "types.h"

void i2s_init();
void audio_send(audio_sample_packed_u * sample_pack);
void audio_pack_samples(audio_sample_packed_u * sample_pack, float * sample_l, float * sample_r);
u32 get_sent_result();

#endif // __AUDIO_HW_H__
