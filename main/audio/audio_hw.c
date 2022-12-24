#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"

#include "audio_config.h"
#include "audio_hw.h"
#include "gpio_config.h"

static i2s_chan_handle_t tx_handle;
static u32 sent_result = 0;
bool tx_on_sent_dma_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);

void audio_pack_samples(audio_sample_packed_u * sample_pack, float * sample_l, float * sample_r)
{
    u16 i;
    
    for(i = 0; i < SAMPLES_BUFFER_SIZE; i++)
    {
        sample_pack[i].channel[0] = (int16_t)(sample_l[i] * AUDIO_CONVERSION_VAL);
        sample_pack[i].channel[1] = (int16_t)(sample_r[i] * AUDIO_CONVERSION_VAL);
    }
}

void audio_send(audio_sample_packed_u * sample_pack)
{
    size_t bytes_written = 0;

    i2s_channel_write(tx_handle, (u8*)&sample_pack[0].sample, 4 * SAMPLES_BUFFER_SIZE, &bytes_written, portMAX_DELAY);
}


u32 get_sent_result()
{
    return sent_result;
}

bool tx_on_sent_dma_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
{
    sent_result++;
    return 0;
}

void i2s_init()
{   
    i2s_chan_config_t chan_cfg = 
    {
        .auto_clear = false,
        .id = I2S_NUM_0,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = 4,
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
    i2s_event_callbacks_t tx_handle_callbacks = {
        .on_sent = tx_on_sent_dma_callback,
    };
    

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handle, &std_cfg));
    i2s_channel_register_event_callback(tx_handle, &tx_handle_callbacks , NULL);
    i2s_channel_enable(tx_handle);
}

