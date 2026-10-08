#ifndef __CALENDAR_BLOCK_H__
#define __CALENDAR_BLOCK_H__

#include "ts.h"

int zeller(int year, int month, int day);
bool is_leap_year(int year);
int get_days_in_month(int year, int month);
void show_calendar_data(int num, int x_start, int y_start, color_t color);
int calendar(color_t color);

#endif