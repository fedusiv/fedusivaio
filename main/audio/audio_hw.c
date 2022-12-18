#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"

#include "audio_config.h"
#include "types.h"
#include "audio_hw.h"
#include "gpio_config.h"

static i2s_chan_handle_t tx_handle;

void audio_pack_samples(audio_sample_packed_u * sample_pack, float * sample_l, float * sample_r)
{
    u8 i;
    
    for(i = 0; i < SAMPLES_BUFFER_SIZE; i++)
    {
        sample_pack[i].channel[0] = (int16_t)(sample_l[i] * AUDIO_CONVERSION_VAL);
        sample_pack[i].channel[1] = (int16_t)(sample_r[i] * AUDIO_CONVERSION_VAL);
    }
}

void audio_send(audio_sample_packed_u * sample_pack)
{
    esp_err_t err;
    size_t bytes_written = 0;

    err = i2s_channel_write(tx_handle, (u8*)&sample_pack[0].sample, 4 * SAMPLES_BUFFER_SIZE, &bytes_written, 1000);
    if(err != ESP_OK)
    {
        printf("Issue with i2s write, error code is %d\n", err);
    }
}

void i2s_init()
{
    i2s_chan_config_t chan_cfg = 
    {
        .auto_clear = false,
        .id = I2S_NUM_0,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = 2,
        .dma_frame_num = SAMPLES_BUFFER_SIZE,
    };
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_handle, NULL));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = GPIO_I2S_BCLK ,
            .ws = GPIO_I2S_WS,
            .dout = GPIO_I2S_DO,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handle, &std_cfg));
    i2s_channel_enable(tx_handle);
}

