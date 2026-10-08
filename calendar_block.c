#include "calendar_block.h"

//蔡勒公式（计算某年某月某日是星期几，0=星期日，1=星期一...）
int zeller(int year, int month, int day)
{
    //把 1 月、2 月当作上一年的 13 月、14 月
    if (month < 3) 
    {
        month += 12;
        year--;
    }

    //计算世纪数 c 和年份 y
    int c = year / 100;
    int y = year % 100;

    //蔡勒公式核心计算
    int w = (y + y/4 + c/4 - 2*c + 26*(month+1)/10 + day - 1) % 7;
    return (w + 7) % 7;
}
// 判断是否为闰年：能被4整除可能是闰年，但如果能被100整除则不是闰年；除非能被400整除就一定是闰年
bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
// 获取某年某月的天数
int get_days_in_month(int year, int month)
{
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) 
        return 29;
    return days[month - 1];
}

void show_calendar_data(int num, int x_start, int y_start, color_t color)
{
    if(num/10!=0)
        lcd_show_text_16x32(x_start, y_start, ch[num/10], color);
    lcd_show_text_16x32(x_start + 18, y_start, ch[num%10], color);
}

int calendar(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t background_color={255, 255, 255};//白色背景
    color_t font_color={0, 0, 0};//黑色字体
    color_t gray={204, 204, 204}; 
    lcd_draw_pic("./PIC/back.bmp", 50, 50);

    static int disp_year = 0;
    static int disp_month = 0;
    static int disp_day = 0;

    if (disp_year == 0) 
    {
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        disp_year = local->tm_year + 1900;
        disp_month = local->tm_mon + 1;
        disp_day = local->tm_mday;
    }

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    int cur_year = local->tm_year + 1900;
    int cur_month = local->tm_mon + 1;
    int cur_day = local->tm_mday;

    int days_in_month = get_days_in_month(disp_year, disp_month);
    int first_day = zeller(disp_year, disp_month, 1);  // 1号是星期几

    int x_start=250, y_start = 0;
    int width =75, height=75;
    lcd_color_block_drawing(x_start, y_start, 525, 600, background_color);

    show_calendar_data(disp_year/100, x_start+20, y_start+20, font_color);
    show_calendar_data(disp_year%100, x_start+56, y_start+20, font_color);
    lcd_show_text_32x32(x_start+92, y_start+20, text[52], font_color);//年

    show_calendar_data(disp_month, x_start+126, y_start+20, font_color);
    lcd_show_text_32x32(x_start+162, y_start+20, text[53], font_color);//月

    char *weekdays[] = {"日", "一", "二", "三", "四", "五", "六"};
    for (int i = 0; i < 7; i++) 
    {
        if(i==0)
            lcd_show_text_32x32(x_start+21+75*i, y_start+95, text[7], font_color);
        else
        lcd_show_text_32x32(x_start+21+75*i, y_start+95, text[i], font_color); 
    }

    int date_num = 1;
    for (int row = 0; row < 6; row++) 
    {
        for (int col = 0; col < 7; col++) 
        {
            int x_data=x_start+20+75*col;
            int y_data=y_start+170+75*row;

            // 计算当前格子对应的星期/日期
            int day_offset = row * 7 + col;
            if (day_offset < first_day) 
            {
                // 上个月的日期（显示为灰色）
                int prev_month = disp_month - 1;
                int prev_year = disp_year;
                if (prev_month < 1) 
                {
                    prev_month = 12;
                    prev_year--;
                }
                int prev_days = get_days_in_month(prev_year, prev_month);
                int prev_day = prev_days - first_day + day_offset + 1;
                show_calendar_data(prev_day, x_data, y_data, gray);
            } 
            else if (date_num <= days_in_month) 
            {
                // 本月的日期
                bool is_today = (disp_year == cur_year && disp_month == cur_month && date_num == cur_day);
                if (is_today) 
                {
                    color_t yellow = {160, 250, 255};
                    lcd_color_block_drawing(x_data-21, y_data-21, 75, 75, yellow);// 今天用高亮颜色
                    show_calendar_data(date_num, x_data, y_data, font_color);
                } 
                else 
                    show_calendar_data(date_num, x_data, y_data, font_color);
                date_num++;
            } 
            else 
            {
                // 下个月的日期（显示为灰色）
                int next_day = date_num - days_in_month;
                show_calendar_data(next_day, x_data, y_data, gray);
                date_num++;
            }
        }
    }

    while (1) 
    {
        int x = -1, y = -1;
        int gesture = ts_detect_touch_gestures(&x, &y);
        if (ts_click_area(x, y, 50, 50, 80, 80)) 
        {
            pop_state();
            return 0;
        }
        else if (gesture == SWIPE_UP)//上滑：下一个月
        {            
            disp_month++;
            if (disp_month > 12) 
            {
                disp_month = 1;
                disp_year++;
            }
            calendar(color);
            return 0;
        } 
        else if (gesture == SWIPE_DOWN)//下滑：上一个月
        {            
            disp_month--;
            if (disp_month < 1) 
            {
                disp_month = 12;
                disp_year--;
            }
            calendar(color);
            return 0;
        } 
        else if (gesture == SWIPE_LEFT)//左滑：下一年
        {            
            disp_year++;
            calendar(color);
            return 0;
        } 
        else if (gesture == SWIPE_RIGHT)//右滑：上一年
        {            
            disp_year--;
            calendar(color);
            return 0;
        }
    }
    return 0;
}
