#ifndef  __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__ 

// if look into gpio pinout on official website. It related to name column or gpioX name

#define DISPLAY_SPI_CS_PIN 10
#define DISPLAY_SPI_MOSI_PIN 11
#define DISPLAY_SPI_MISO_PIN  14
#define DISPLAY_SPI_CLK_PIN   12
#define DISPLAY_RST_PIN    13
#define DISPLAY_DC_PIN     7
#define DISPLAY_BCLK_PIN     6

#define GPIO_I2S_BCLK 4
#define GPIO_I2S_WS  5 // word select or left right select
#define GPIO_I2S_DO   15 // data out

#define INPUT_SIN 18
#define INPUT_CLK 17
#define INPUT_TRIG 16

#define JOYSTICK_X_PIN 3
#define JOYSTICK_Y_PIN 8
#define JOYSTICK_X_ADC_CH 2
#define JOYSTICK_Y_ADC_CH 7


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
    BUTTON_ID_ENC_1,
    BUTTON_ID_JS_1, // on joystick switch need to be pulled up. In this case reading will be provided.
    BUTTON_ID_MAX
}buttons_id_e;

typedef enum
{
    ENCODER_ID_1,
    ENCODER_ID_MAX
}encoders_id_e;


#endif // __GPIO_CONFIG_H__ 
