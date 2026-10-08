#include "clock_time_block.h"

int clock_time(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体

    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_draw_pic("./PIC/闹钟.bmp", 112, 200);
    lcd_show_text_32x32(175, 410, text[54], font_color);//闹
    lcd_show_text_32x32(217, 410, text[55], font_color);//钟
    lcd_draw_pic("./PIC/秒表.bmp", 412, 200);
    lcd_show_text_32x32(475, 410, text[17], font_color);//秒
    lcd_show_text_32x32(517, 410, text[56], font_color);//表
    lcd_draw_pic("./PIC/计时.bmp", 712, 200);
    lcd_show_text_32x32(775, 410, text[57], font_color);//计
    lcd_show_text_32x32(817, 410, text[19], font_color);//时

    while(1)
    {
        int x=-1, y=-1;
        ts_get_axes(&x, &y);
        if(x == -1 || y == -1)
            continue;

        if(ts_click_area(x, y, 50, 50, 80, 80))
        {
            pop_state();
            return 0;
        }
        else
        {
            if(ts_click_area(x, y, 112, 200, 200, 200))
            {
                printf("点击闹钟\n");
                alarm_clock(color);
            }
            else if(ts_click_area(x, y, 412, 200, 200, 200))
            {
                printf("点击秒表\n");
                stopwatch(color);
            }
            else if(ts_click_area(x, y, 712, 200, 200, 200))
            {
                printf("点击计时\n");
                timer(color);
            }
            else
                continue;

            lcd_color_block_drawing(0, 0, 1024, 600, color);
            lcd_draw_pic("./PIC/back.bmp", 50, 50);
            lcd_draw_pic("./PIC/闹钟.bmp", 112, 200);
            lcd_show_text_32x32(175, 410, text[54], font_color);//闹
            lcd_show_text_32x32(217, 410, text[55], font_color);//钟
            lcd_draw_pic("./PIC/秒表.bmp", 412, 200);
            lcd_show_text_32x32(475, 410, text[17], font_color);//秒
            lcd_show_text_32x32(517, 410, text[56], font_color);//表
            lcd_draw_pic("./PIC/计时.bmp", 712, 200);
            lcd_show_text_32x32(775, 410, text[57], font_color);//计
            lcd_show_text_32x32(817, 410, text[19], font_color);//时
        }
    }
}
