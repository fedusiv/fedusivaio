#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdlib.h>

void init_memory_manager();
float * get_audio_samples_r();
float * get_audio_samples_l();
uint16_t * get_display_frame_buffer();
uint16_t * get_display_dma_buffer();
uint16_t * get_display_soundwave_graph();
uint32_t * get_memory_audio_send_buffer();

#endif // __MEMORY_MANAGER_H__
