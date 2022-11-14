#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "audio_config.h"

static float sine_wave(float freq_hz, float d_time);

float angular_freq(float freq)
{
    return PI2 * freq;
}

float sine_wave(float freq_hz, float d_time)
{
   return sinf( freq_hz * d_time); 
}

void generate_sound(float note_freq, float * data_buffer)
{
    float sample;
    float w = 0.0;
    float d_time;
    float d_time_step = 0.0;

    d_time_step = 1.0 / SAMPLE_RATE;

    w = angular_freq(note_freq);

    for(int i = 0; i < SAMPLES_BUFFER_SIZE; i+= AMOUNT_OF_CHANNELS)
    {
        sample = sine_wave(w, d_time);
        data_buffer[i] =  sample;
        data_buffer[i+1] = sample;
        d_time += d_time_step;
        if( d_time * w / 44100 > PI2)
        {
            d_time = 0;
        }

    }

}
