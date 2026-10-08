#ifndef __STOPWATCH_BLOCK_H__
#define __STOPWATCH_BLOCK_H__

#include "ts.h"

void *stopwatch_timer_thread(void *arg);
void show_stopwatch_digit(int num, int x, int y, color_t color);
void show_stopwatch_time(color_t color);
int stopwatch(color_t color);

#endif