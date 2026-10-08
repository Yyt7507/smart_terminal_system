#ifndef __TIMER_BLOCK_H__
#define __TIMER_BLOCK_H__

#include "ts.h"

void *timer_countdown_thread(void *arg);
void show_timer_digit(int num, int x, int y, color_t color);
void show_timer_time(color_t color);
int timer_input_number(color_t color, char *tip);
int timer(color_t color);

#endif