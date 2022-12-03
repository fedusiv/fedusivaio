#ifndef __ILI_9341_H__
#define __ILI_9341_H__

#include <inttypes.h>
#include "display_config.h"

void init_display();
uint16_t convert_color(color_t color);
void send_lines(int ypos, uint16_t *linedata);

#endif // __ILI_9341_H__
