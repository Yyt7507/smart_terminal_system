#include "main_interface.h"

int main_interface(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体
    int time_x_start=5;
    int time_y_start=5;

    stack_top = 0;
    push_state(STATE_MAIN);

    time_t now;
    struct tm *local =NULL;
    now = time(NULL);
    if (now == (time_t)-1)
     {
        perror("time error");
        return 1;
    }
    local = localtime(&now);
    if (local == NULL) 
    {
        perror("localtime error");
        return 1;
    }
    lcd_color_block_drawing(time_x_start, time_y_start, 120, 36, color);
    show_time(local, time_x_start, time_y_start, font_color);

    lcd_draw_pic("./PIC/weather.bmp", 37, 50);
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(59+37+(36+10)*i, 260, text[9+i], font_color);
    lcd_draw_pic("./PIC/calendar.bmp", 287, 50);
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(59+287+(36+10)*i, 260, text[7+i], font_color);
    lcd_draw_pic("./PIC/clock.bmp", 537, 50);
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(59+537+(36+10)*i, 260, text[19+i], font_color);
    lcd_draw_pic("./PIC/lamp.bmp", 787, 50);
    for(int i=0; i<4; i++)
        lcd_show_text_32x32(13+787+(36+10)*i, 260, text[21+i], font_color);
    lcd_draw_pic("./PIC/air.bmp", 37, 350);
    for(int i=0; i<4; i++)
        lcd_show_text_32x32(13+37+(36+10)*i, 560, text[25+i], font_color);

    while(1)
    {
        int x=-1, y=-1;
        ts_get_axes(&x, &y);
        if(ts_click_area(x, y, 37, 50, 200, 200))
        {
            push_state(STATE_WEATHER);
            weather(color);
            main_interface(color);
            return 0;
        }
        else if(ts_click_area(x, y, 287, 50, 200, 200))
        {
            push_state(STATE_CALENDAR);
            calendar(color);
            main_interface(color);
            return 0;
        }
        else if(ts_click_area(x, y, 537, 50, 200, 200))
        {
            push_state(STATE_CLOCK);
            clock_time(color);
            main_interface(color);
            return 0;
        }
        else if(ts_click_area(x, y, 787, 50, 200, 200))
        {
            push_state(STATE_LAMP);
            lamp(color);
            main_interface(color);
            return 0;
        }
        else if(ts_click_area(x, y, 37, 350, 200, 200))
        {
            push_state(STATE_AIR);
            air(color);
            main_interface(color);
            return 0;
        }
    }
}
