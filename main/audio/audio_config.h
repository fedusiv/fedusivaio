#ifndef __AUDIO_CONFIG_H__
#define __AUDIO_CONFIG_H__

#include <stdint.h>

// One sample for 44100 sample rate lenght is 22.67 microseconds
// Latency of 5ms should be alright
#define SAMPLE_ONE_INTERRUPT 220 // 200 * 22.67 microseconds = 4.988ms
#define SAMPLE_SEND_PERIOD 200 // in 10us
#define SAMPLES_BUFFER_SIZE  SAMPLE_ONE_INTERRUPT // amount of samples for one channel
#define SAMPLE_AMOUNT_OF_CHANNELS 2
#define SAMPLE_BYTES_SIZE 2 // for 16bit sample size
#define SAMPLE_RATE 44100
#define SAMPLES_BUFFER_SIZE_FULL (SAMPLE_AMOUNT_OF_CHANNELS * SAMPLES_BUFFER_SIZE * SAMPLE_BYTES_SIZE) // size of one data buffer, which should has two channels's data. Size in bytes

#define WAVEFORM_RES    10UL
#define WAVEFORM_CNT    (1<<WAVEFORM_RES)
#define WAVEFORM_MSK    (WAVEFORM_CNT-1)
#define WAVEFORM_I(i)   (((i) >> (32 - WAVEFORM_RES)) & WAVEFORM_MSK)
#define NOTE_FREQ_BASE  27.50 // A0
#define NOTES_COUNT     128
#define NOTE_OFFSET     3 // basically distance between NOTE_FREQ_BASE and C1

#define POLYPHONY_AMOUNT 4
#define ADSR_PROCESS_FREQ 4 // frequency of processing adsr, if 4 means, that adsr will process each 4th sample

#define PI               (3.14159265)
#define PI2              (6.28318530)
#define PIH              (0.63661977)
#define AUDIO_CONVERSION_VAL 16383.0f

typedef enum _audio_notes_e
{
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
    NOTE_ZERO,
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
