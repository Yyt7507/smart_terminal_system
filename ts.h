#ifndef __TOUCH_SCREEN_H__
#define __TOUCH_SCREEN_H__

#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../cJSON/cJSON.h"
#include "lcd.h"

#define			TOUCH				0
#define			SWIPE_UP			1
#define			SWIPE_DOWN			2
#define			SWIPE_LEFT			3
#define			SWIPE_RIGHT			4
#define         MAX_STACK           10

extern int ts_fd;
extern int state_stack[MAX_STACK];
extern int stack_top;

enum {
    STATE_MAIN = 0,      // 主界面
    STATE_WEATHER = 1,   // 天气
    STATE_CALENDAR = 2,  // 日历
    STATE_CLOCK = 3,     // 时钟
    STATE_LAMP = 4,      // 灯光控制
    STATE_AIR = 5,       // 空气状态
};

void push_state(int new_state);
int pop_state(void);
int get_current_state(void);
int ts_init(void);
int ts_close(void);
int ts_get_axes(int *x, int *y);
bool ts_click_area(int x, int y, int x_start, int y_start, int width, int height);
int ts_detect_touch_gestures(int *x, int *y);
bool pwd(color_t color);


int clock_time(color_t color);

int lamp(color_t color);

int air(color_t color);





#endif