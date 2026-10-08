#ifndef __LCD_H__
#define __LCD_H__

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <time.h>
#include <math.h>

typedef struct{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}color_t;

typedef struct{
    color_t color;
    uint8_t a;
}pix_t;

extern int lcd_fd;
extern pix_t (*pix)[1024];
extern unsigned char ch[][64];
extern unsigned char text[][128];

int lcd_init(void);
int lcd_close(void);
int lcd_clear(void);
int lcd_color_block_drawing(int x_start, int y_start, int width, int height, color_t color);
int lcd_draw_pic(char *picpath, int x_start, int y_start);
int lcd_show_text_16x32(int x_start, int y_start, unsigned char buf[16], color_t color);
int lcd_show_text_32x32(int x_start, int y_start, unsigned char buf[], color_t color);
int get_digit_count(int num);
void show_data(struct tm *local, int data_x_start, int data_y_start, color_t color);
void show_time(struct tm *local, int time_x_start, int time_y_start, color_t color);
int lcd_screen_off(color_t color);
int lcd_screen_lock(color_t color);

#endif