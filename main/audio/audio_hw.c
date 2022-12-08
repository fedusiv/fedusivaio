#include <math.h>
#include "audio/audio_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/gpio.h"

#include "audio_hw.h"
#include "gpio_config.h"

void audio_send(audio_sample_packed_u * sample_pack)
{
    esp_err_t err;
    size_t bytes_written = 0;

    err = i2s_write(I2S_NUM_0, (uint8_t*)&sample_pack[0].sample, 4 * SAMPLES_BUFFER_SIZE, &bytes_written, portMAX_DELAY);
}

void i2s_init()
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .tx_desc_auto_clear = false,
        .dma_buf_count = 8,
        .dma_buf_len = 64
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = GPIO_I2S_BCLK ,
        .ws_io_num = GPIO_I2S_WS,
        .data_out_num = GPIO_I2S_DO,
        .mck_io_num = -1,
        .data_in_num = I2S_PIN_NO_CHANGE,
    };
    
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);   //install and start i2s driver
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

