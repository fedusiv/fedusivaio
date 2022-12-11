#include "esp_system.h"
#include "driver/spi_master.h"

#include "memory_manager.h"
#include "audio/audio_config.h"
#include "display/display_config.h"


// memory segments
// Audio field
float audio_samples_buffer_l[SAMPLES_BUFFER_SIZE];
float audio_samples_buffer_r[SAMPLES_BUFFER_SIZE];
uint32_t audio_samples_buffer_send[SAMPLES_BUFFER_SIZE];

// Display field
uint16_t * display_frame_buffer;
DRAM_ATTR uint16_t display_dma_buffer[SCREEN_WIDTH * SCREEN_SEND_HEIGHT];
uint16_t * display_soundwave_graph;

void init_memory_manager()
{
    display_frame_buffer = (uint16_t *)heap_caps_malloc(SCREEN_SIZE * sizeof(uint16_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    if(display_frame_buffer == NULL)
    {
        printf("Cannot allocate display_frame_buffer\n");
        abort();
    }
    display_soundwave_graph = (uint16_t *)heap_caps_malloc(SAMPLES_BUFFER_SIZE * 2 * sizeof(uint16_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    if(display_soundwave_graph  == NULL)
    {
        printf("Cannot allocate display_soundwave_graph \n");
        abort();
    }
}


float * get_audio_samples_l()
{
    return audio_samples_buffer_l;
}

float * get_audio_samples_r()
{
    return audio_samples_buffer_r;
}

uint32_t * get_memory_audio_send_buffer()
{
    return audio_samples_buffer_send;
}

uint16_t * get_display_frame_buffer()
{
    return display_frame_buffer;
}

uint16_t * get_display_dma_buffer()
{
    return display_dma_buffer;
}

uint16_t * get_display_soundwave_graph()
{
    return display_soundwave_graph;
}
