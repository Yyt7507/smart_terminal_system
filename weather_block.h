#ifndef __WEATHER_BLOCK_H__
#define __WEATHER_BLOCK_H__

#include "ts.h"

int timeToch(const char *str, color_t color, int x_start);
int dataToch(const char *str, color_t color, int x_start);
int wind_directionTotext(const char *str, color_t color, int x_start);
int wind_powerTotext(const char *str, color_t color, int x_start);
int weatherTopic(const char *str, int x_start, int y_start);
int temperatureToch(const char *str, color_t color, int x_start);
int weather_24hours(color_t color);
int weather_7days(color_t color);
int weather(color_t color);

#endif