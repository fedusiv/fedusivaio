#ifndef __SOUND_ENGINE_H__
#define __SOUND_ENGINE_H__

#include "audio_config.h"

void sound_engine_init();
void synth_process(float * sample_l, float * sample_r);
uint16_t get_note_id(audio_note_e note);
void synth_note_on(uint16_t note_id);
void synth_note_off(uint16_t note_id);

#endif // __SOUND_ENGINE_H__
