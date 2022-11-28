#ifndef __AUDIO_HW_H__
#define __AUDIO_HW_H__

#include "audio_config.h"

void i2s_init();
void audio_send(audio_sample_packed_u * sample_pack);

#endif // __AUDIO_HW_H__
