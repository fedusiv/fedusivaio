#ifndef __ILI_9341_H__
#define __ILI_9341_H__

#include <inttypes.h>

void init_display();
uint16_t convert_color(uint8_t r, uint8_t g, uint8_t b);
void send_lines(int ypos, uint16_t *linedata);

#endif // __ILI_9341_H__
