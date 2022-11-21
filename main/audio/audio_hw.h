#ifndef __AUDIO_HW_H__
#define __AUDIO_HW_H__

#include "audio_config.h"

void i2s_init();
void audio_send(float * buffer);
void play(float freq);
float get_note_freq(audio_note_e note, audio_octaves_e octave);
float * get_buffer();

#endif // __AUDIO_HW_H__
