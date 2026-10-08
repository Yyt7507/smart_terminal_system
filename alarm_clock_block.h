#ifndef __ALARM_CLOCK_BLOCK_H__
#define __ALARM_CLOCK_BLOCK_H__

#include "ts.h"

int alarm_select_month(color_t color);
int alarm_input_number(color_t color, char *tip);
void add_alarm(int month, int day, int hour, int min, int sec);
void delete_alarm(int index);
int show_alarm_list(color_t color);
int alarm_clock_day(color_t color);

void show_weekday_buttons(color_t color, bool selected[7]);
bool select_weekdays_page(color_t color, bool selected[7]);
void show_weekday_names(bool weekdays[7], int x, int y, color_t color);
void add_week_alarm(int hour, int min, int sec, bool weekdays[7]);
void delete_week_alarm(int index);
void show_week_alarm_list(color_t color);
int alarm_clock_week(color_t color);
int alarm_clock(color_t color);

#endif