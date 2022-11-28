#ifndef  __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__ 

#define DISPLAY_SPI_CS_PIN 10
#define DISPLAY_SPI_MOSI_PIN 11
#define DISPLAY_SPI_MISO_PIN  14
#define DISPLAY_SPI_CLK_PIN   12
#define DISPLAY_RST_PIN    6
#define DISPLAY_DC_PIN     7
#define DISPLAY_BCLK_PIN     21

#define GPIO_I2S_BCLK 4
#define GPIO_I2S_WS  5 // word select or left right select
#define GPIO_I2S_DO   15 // data out

#define INPUT_SIN 16
#define INPUT_CLK 17
#define INPUT_TRIG 18


typedef enum
{
    BUTTON_ID_B,
    BUTTON_ID_Ab,
    BUTTON_ID_A,
    BUTTON_ID_Gb,
    BUTTON_ID_G,
    BUTTON_ID_Fb,
    BUTTON_ID_F,
    BUTTON_ID_E,
    BUTTON_ID_Db,
    BUTTON_ID_D,
    BUTTON_ID_Cb,
    BUTTON_ID_C,
    BUTTON_ID_MAX
}buttons_id_e;

typedef enum
{
    ENCODER_ID_1,
    ENCODER_ID_MAX
}encoders_id_e;


#endif // __GPIO_CONFIG_H__ 
