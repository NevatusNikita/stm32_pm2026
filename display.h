#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_cmd(uint8_t cmd);
void display_data(uint8_t data);
void display_init(void);
void display_clear(void);
void display_draw_chessboard(void);

#endif