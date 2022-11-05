#ifndef  __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__ 

#define DISPLAY_SPI_CS_PIN 10
#define DISPLAY_SPI_MOSI_PIN 11
#define DISPLAY_SPI_MISO_PIN  14
#define DISPLAY_SPI_CLK_PIN   12
#define DISPLAY_RST_PIN    6
#define DISPLAY_DC_PIN     7
#define DISPLAY_BCLK_PIN     21

#define I2S_OUT_BCLK 4
#define I2S_OUT_WS 5
#define I2S_OUT_DO 15

#define ENCODER_A 16
#define ENCODER_B 17
#define ENCODER_SWT 18

#define INPUT_SIN 16
#define INPUT_CLK 17
#define INPUT_TRIG 18


typedef enum
{
    BUTTON_ID_ENC_SWT,
    BUTTON_ID_ENC_B,
    BUTTON_ID_ENC_A,
    BUTTON_ID_G,
    BUTTON_ID_F,
    BUTTON_ID_E,
    BUTTON_ID_D,
    BUTTON_ID_C,
    BUTTON_ID_MAX
}buttons_id_e;


#endif // __GPIO_CONFIG_H__ 
