#ifndef __AUDIO_CONFIG_H__
#define __AUDIO_CONFIG_H__

#include <stdint.h>

#define SAMPLE_RATE 44100
#define AMOUNT_OF_CHANNELS 2
#define SAMPLE_SIZE 2 // size of one sample for 16 bit data
#define DMA_FRAME_NUM 24 // Amount of data for one WS Click (basically one channel)
#define SAMPLES_BUFFER_SIZE (AMOUNT_OF_CHANNELS * DMA_FRAME_NUM) // size of one data buffer, which should has two channels's data

#define WAVEFORM_RES    10UL
#define WAVEFORM_CNT    (1<<WAVEFORM_RES)
#define WAVEFORM_MSK    (WAVEFORM_CNT-1)
#define WAVEFORM_I(i)   (((i) >> (32 - WAVEFORM_RES)) & WAVEFORM_MSK)
#define NOTE_FREQ_BASE  27.50 // A0
#define NOTES_COUNT     128

#define PI               (3.14159265)
#define PI2              (6.28318530)
#define PIH              (0.63661977)
#define AUDIO_CONVERSION_VAL 16383.0f

typedef enum _audio_notes_e
{
    NOTE_ZERO,
    NOTE_C,
    NOTE_Cb,
    NOTE_D,
    NOTE_Db,
    NOTE_E,
    NOTE_F,
    NOTE_Fb,
    NOTE_G,
    NOTE_Gb,
    NOTE_A,
    NOTE_Ab,
    NOTE_B,
    NOTE_MAX
} audio_note_e;

typedef enum _audio_octaves_e
{
    OCTAVE_1,
    OCTAVE_2,
    OCTAVE_3,
    OCTAVE_4,
    OCTAVE_5,
    OCTAVE_6,
    OCTAVE_7,
    OCTAVE_8,
    OCTAVE_MAX
}audio_octaves_e;


typedef union _audio_sample_packed_u
{
    uint32_t sample;
    int16_t channel[2];
}audio_sample_packed_u;

#endif //__AUDIO_CONFIG_H__
