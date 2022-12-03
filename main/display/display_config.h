#ifndef __DISPLAY_CONFIG_H__
#define __DISPLAY_CONFIG_H__

#include <stdlib.h>

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320
#define SCREEN_SIZE SCREEN_HEIGHT * SCREEN_WIDTH
#define SCREEN_SEND_HEIGHT 20

typedef struct _color_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

#endif //__DISPLAY_CONFIG_H__
