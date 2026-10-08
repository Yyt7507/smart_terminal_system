#include "alarm_clock_block.h"

typedef struct {
    int month;
    int day;
    int hour;
    int min;
    int sec;
    bool enabled;
} AlarmClock;
#define     MAX_ALARMS      5
static AlarmClock alarms[MAX_ALARMS];
static int alarm_count = 0;

typedef struct {
    int hour;
    int min;
    int sec;
    bool weekdays[7];  // 0=周日, 1=周一, 2=周二, 3=周三, 4=周四, 5=周五, 6=周六
    bool enabled;
} WeekAlarmClock;
#define MAX_WEEK_ALARMS 10
static WeekAlarmClock week_alarms[MAX_WEEK_ALARMS];
static int week_alarm_count = 0;

#if 1
int alarm_select_month(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体
    color_t black={0, 0, 0};
    lcd_show_text_32x32(402, 50, text[66], font_color);//请
    lcd_show_text_32x32(444, 50, text[67], font_color);//选
    lcd_show_text_32x32(486, 50, text[68], font_color);//择
    lcd_show_text_32x32(528, 50, text[53], font_color);//月
    lcd_show_text_32x32(570, 50, text[69], font_color);//份
    char num_month[4][3]={
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12}
    };
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_color_block_drawing(356, 132, 312, 416, font_color);

    for(int j=0; j<4; j++) 
    {
        for(int i=0; i<3; i++) 
        {
            int x = 356 + 104*i + 40;
            int y = 132 + 104*j + 35;
            if(num_month[j][i] >= 10) 
            {
                lcd_show_text_16x32(x, y, ch[(num_month[j][i] / 10)], black);
                lcd_show_text_16x32(x + 18, y, ch[(num_month[j][i] % 10)], black);
            } 
            else
                lcd_show_text_16x32(x + 9, y, ch[num_month[j][i]], black);
        }
    }

    while(1) 
    {
        int x = -1, y = -1;
        ts_get_axes(&x, &y);
        if(ts_click_area(x, y, 50, 50, 80, 80))
            return -1;
        for(int j=0; j<4; j++)
        {
            for(int i=0; i<3; i++)
            {
                if(ts_click_area(x, y, 356+104*i, 132+104*j, 104, 104))
                    return num_month[j][i];
            }
        } 
    }   
}

int alarm_input_number(color_t color, char *tip)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_draw_pic("./PIC/number1.bmp", 272, 120);
    char keyboard[4][3] = {
		{'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'d', '0', 'o'}
	};
	char input[3] = {0};

    lcd_show_text_32x32(402, 42, text[66], font_color);//请
    lcd_show_text_32x32(444, 42, text[70], font_color);//输
    lcd_show_text_32x32(486, 42, text[71], font_color);//入
    if(strcmp(tip, "日期")==0)
    {
        lcd_show_text_32x32(528, 42, text[7], font_color);//日
        lcd_show_text_32x32(570, 42, text[51], font_color);//期
    }
    else if(strcmp(tip, "小时")==0)
    {
        lcd_show_text_32x32(528, 42, text[18], font_color);//小
        lcd_show_text_32x32(570, 42, text[19], font_color);//时
    }
    else if(strcmp(tip, "分钟")==0)
    {
        lcd_show_text_32x32(528, 42, text[72], font_color);//分
        lcd_show_text_32x32(570, 42, text[20], font_color);//钟
    }
    else if(strcmp(tip, "秒钟")==0)
    {
        lcd_show_text_32x32(528, 42, text[17], font_color);//秒
        lcd_show_text_32x32(570, 42, text[20], font_color);//钟
    }

	int x,y;
	int i,j,n = 0;
	while(1)
	{
		ts_get_axes(&x,&y);
		printf("<%d,%d>\n",x,y);
        if(ts_click_area(x, y, 50, 50, 80, 80)) 
            return -1;
		for(j = 0;j < 4;j++)
		{
			for(i = 0;i < 3;i++)
			{
				if(ts_click_area(x, y, 272+160*i, 120+120*j, 160, 120))
				{
					if(keyboard[j][i] == 'd')
					{
						if(n == 0)
                        {
							input[0] = 0;
							printf("input is empty\n");
                        }
						else
							input[--n] = 0;
					}
					else if(keyboard[j][i] == 'o')
					{
                        if(n == 0) 
                            printf("请先输入数字\n");
                        else 
                        {
                            int result = 0;
                            for(int k = 0; k < n; k++) 
                                result = result * 10 + (input[k] - '0');
                            return result;
                        }
					}
					else
					{
                        if(n < 2) 
                        {
                            input[n++] = keyboard[j][i];
                            input[n] = '\0';
                        } 
                        else 
                            printf("最多输入2位\n");
                    }
			    }
		    }
        }
        lcd_color_block_drawing(495, 81, 34, 32, color);
        if(n==1)
            lcd_show_text_16x32(495, 81, ch[input[0] - '0'], font_color);
        else if(n==2) 
        {
            lcd_show_text_16x32(495, 81, ch[input[0] - '0'], font_color);
            lcd_show_text_16x32(513, 81, ch[input[1] - '0'], font_color);
        }
	}
}

void add_alarm(int month, int day, int hour, int min, int sec)
{
    if(alarm_count >= MAX_ALARMS) 
    {
        printf("闹钟已满，最多 %d 个\n", MAX_ALARMS);
        return;
    }
    
    alarms[alarm_count].month = month;
    alarms[alarm_count].day = day;
    alarms[alarm_count].hour = hour;
    alarms[alarm_count].min = min;
    alarms[alarm_count].sec = sec;
    alarms[alarm_count].enabled = true;
    alarm_count++;
    printf("闹钟已添加: %d月%d日 %02d:%02d:%02d\n", month, day, hour, min, sec);
}

void delete_alarm(int index)
{
    if(index < 0 || index >= alarm_count) 
        return;
    
    for(int i = index; i < alarm_count - 1; i++) 
        alarms[i] = alarms[i + 1];
    alarm_count--;
    printf("闹钟已删除\n");
}

int show_alarm_list(color_t color)
{
    color_t font_color = {255, 255, 255};
    color_t black={0, 0, 0};
    color_t gray={204, 204, 204}; 
    lcd_color_block_drawing(130, 130, 800, 350, color);
    
    if(alarm_count == 0) 
        return -1;
    
    static int delete_index = -1;
    delete_index = -1;
    
    for(int i=0; i<alarm_count && i<5; i++) 
    {
        int y_pos = 50 + 86 * i;
        lcd_color_block_drawing(180, y_pos, 676, 70, font_color);
        lcd_show_text_32x32(200, y_pos + 20, text[73], black);//待
        lcd_show_text_32x32(234, y_pos + 20, text[74], black);//响
        lcd_show_text_32x32(268, y_pos + 20, text[75], black);//铃
        lcd_show_text_32x32(302, y_pos + 20, text[76], black);//的
        lcd_show_text_32x32(336, y_pos + 20, text[54], black);//闹
        lcd_show_text_32x32(370, y_pos + 20, text[55], black);//钟
        
        // 月份
        if(alarms[i].month >= 10) 
        {
            lcd_show_text_16x32(432, y_pos + 20, ch[alarms[i].month / 10], black);
            lcd_show_text_16x32(450, y_pos + 20, ch[alarms[i].month % 10], black);
        } 
        else 
            lcd_show_text_16x32(441, y_pos + 20, ch[alarms[i].month], black);
        lcd_show_text_32x32(468, y_pos + 20, text[53], black);  // 月
        
        // 日期
        if(alarms[i].day >= 10) {
            lcd_show_text_16x32(502, y_pos + 20, ch[alarms[i].day / 10], black);
            lcd_show_text_16x32(520, y_pos + 20, ch[alarms[i].day % 10], black);
        } else {
            lcd_show_text_16x32(511, y_pos + 20, ch[alarms[i].day], black);
        }
        lcd_show_text_32x32(538, y_pos + 20, text[7], black);  // 日
        
        // 时分秒
        lcd_show_text_16x32(572, y_pos + 20, ch[alarms[i].hour / 10], black);
        lcd_show_text_16x32(590, y_pos + 20, ch[alarms[i].hour % 10], black);
        lcd_show_text_16x32(608, y_pos + 20, ch[10], black);  // :
        lcd_show_text_16x32(626, y_pos + 20, ch[alarms[i].min / 10], black);
        lcd_show_text_16x32(644, y_pos + 20, ch[alarms[i].min % 10], black);
        lcd_show_text_16x32(662, y_pos + 20, ch[10], black);  // :
        lcd_show_text_16x32(680, y_pos + 20, ch[alarms[i].sec / 10], black);
        lcd_show_text_16x32(698, y_pos + 20, ch[alarms[i].sec % 10], black);
        
        lcd_color_block_drawing(750, y_pos, 106, 72, gray);
        lcd_show_text_32x32(770, y_pos + 15, text[77], font_color);  // 删
        lcd_show_text_32x32(804, y_pos + 15, text[78], font_color);  // 除
    }
    return delete_index;
}

int alarm_clock_day(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体
    color_t gray={204, 204, 204}; 
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_color_block_drawing(462, 480, 100, 70, gray);
    lcd_show_text_32x32(475, 499, text[62], font_color);//添
    lcd_show_text_32x32(516, 499, text[63], font_color);//加

    show_alarm_list(color);
    
    while(1)
    {
        int x=-1, y=-1;
        ts_get_axes(&x, &y);
        if(x == -1 || y == -1)
            continue;

        if(ts_click_area(x, y, 50, 50, 80, 80))
            return 0;

        for(int i = 0; i < alarm_count && i < 5; i++) //点击删除
        {
            int y_pos = 50 + 86 * i;
            if(ts_click_area(x, y, 750, y_pos, 106, 72)) 
            {
                printf("点击删除闹钟 %d\n", i);
                delete_alarm(i);

                lcd_color_block_drawing(0, 0, 1024, 600, color);
                lcd_draw_pic("./PIC/back.bmp", 50, 50);
                lcd_color_block_drawing(462, 480, 100, 70, gray);
                lcd_show_text_32x32(475, 499, text[62], font_color);//添
                lcd_show_text_32x32(516, 499, text[63], font_color);//加
                show_alarm_list(color);
                break;
            }
        }

        if(ts_click_area(x, y, 462, 480, 100, 70))//点击添加
        {
            int selected_month = alarm_select_month(color);
            if(selected_month == -1) 
                continue;  
            int selected_day = alarm_input_number(color, "日期");
            if(selected_day == -1) 
                continue;
            if(selected_day < 1 || selected_day > 31) 
            {
                printf("日期无效: %d\n", selected_day);
                continue;
            }
            
            int selected_hour = alarm_input_number(color, "小时");
            if(selected_hour == -1) 
                continue;
            if(selected_hour < 0 || selected_hour > 23) 
            {
                printf("小时无效: %d\n", selected_hour);
                continue;
            }
            
            int selected_min = alarm_input_number(color, "分钟");
            if(selected_min == -1)
                continue;
            if(selected_min < 0 || selected_min > 59) 
            {
                printf("分钟无效: %d\n", selected_min);
                continue;
            }
            
            int selected_sec = alarm_input_number(color, "秒钟");
            if(selected_sec == -1)            
                continue;
            if(selected_sec < 0 || selected_sec > 59) 
            {
                printf("秒钟无效: %d\n", selected_sec);
                continue;
            }
            
            add_alarm(selected_month, selected_day, selected_hour, selected_min, selected_sec);
            
            lcd_color_block_drawing(0, 0, 1024, 600, color);
            lcd_draw_pic("./PIC/back.bmp", 50, 50);
            lcd_color_block_drawing(462, 480, 100, 70, gray);
            lcd_show_text_32x32(475, 498, text[62], font_color);
            lcd_show_text_32x32(517, 498, text[63], font_color);
            show_alarm_list(color);
        }
    }
}
#endif

#if 2
void show_weekday_buttons(color_t color, bool selected[7])
{
    color_t font_color = {255, 255, 255};
    color_t black = {0, 0, 0};
    color_t selected_color = {100, 200, 100};  // 选中为绿色
    color_t unselected_color = {80, 80, 80};    // 未选中为灰色
    
    char *week_names[] = {"日", "一", "二", "三", "四", "五", "六"};
    int start_x = 237;
    int start_y = 250;
    int btn_width = 70;
    int btn_height = 70;
    int gap = 10;
    
    for(int i = 0; i < 7; i++)
    {
        int x = start_x + i * (btn_width + gap);
        int y = start_y;
        
        if(selected[i])
            lcd_color_block_drawing(x, y, btn_width, btn_height, selected_color);
        else 
            lcd_color_block_drawing(x, y, btn_width, btn_height, unselected_color);
        
        if(i==0)
            lcd_show_text_32x32(x+(btn_width-32)/2, y+(btn_height-32)/2, text[7], black);
        else
            lcd_show_text_32x32(x+(btn_width-32)/2, y+(btn_height-32)/2, text[i], black);
    }
}

bool select_weekdays_page(color_t color, bool selected[7])
{
    for(int i = 0; i < 7; i++) 
        selected[i] = true;
    
    while(1) 
    {
        lcd_color_block_drawing(0, 0, 1024, 600, color);
        color_t font_color = {255, 255, 255};
        color_t black = {0, 0, 0};
        color_t gray = {204, 204, 204};
        lcd_draw_pic("./PIC/back.bmp", 50, 50);
        
        lcd_show_text_32x32(412, 50, text[66], font_color);  // 请
        lcd_show_text_32x32(454, 50, text[67], font_color);  // 选
        lcd_show_text_32x32(496, 50, text[68], font_color);  // 择
        lcd_show_text_32x32(538, 50, text[61], font_color);  // 星
        lcd_show_text_32x32(580, 50, text[51], font_color);  // 期
        
        show_weekday_buttons(color, selected);
        
        color_t gray_btn = {204, 204, 204};
        lcd_color_block_drawing(462, 480, 100, 70, gray_btn);
        lcd_show_text_32x32(475, 499, text[64], font_color);  // 确
        lcd_show_text_32x32(516, 499, text[65], font_color);  // 定
        
        int x = -1, y = -1;
        ts_get_axes(&x, &y);
        if(x == -1 || y == -1) 
            continue;
        
        if(ts_click_area(x, y, 50, 50, 80, 80)) 
            return false; 
        
        int start_x = 237;
        int start_y = 250;
        int btn_width = 70;
        int btn_height = 70;
        int gap = 10;
        
        for(int i = 0; i < 7; i++) 
        {
            int bx = start_x + i * (btn_width + gap);
            int by = start_y;
            if(ts_click_area(x, y, bx, by, btn_width, btn_height)) 
            {
                selected[i] = !selected[i];
                
                lcd_color_block_drawing(start_x - 10, start_y - 10, 7 * (btn_width + gap) + 20, btn_height + 20, color);
                show_weekday_buttons(color, selected);
                break;
            }
        }
        
        if(ts_click_area(x, y, 462, 480, 100, 70)) 
        {
            bool has_selected = false;
            for(int i = 0; i < 7; i++) 
            {
                if(selected[i]) 
                {
                    has_selected = true;
                    break;
                }
            }
            if(!has_selected) 
            {
                printf("请至少选择一个星期\n");
                continue;
            }
            return true;
        }
    }
}

void show_weekday_names(bool weekdays[7], int x, int y, color_t color)
{
    char *week_names[] = {"日", "一", "二", "三", "四", "五", "六"};
    color_t gray_color = {150, 150, 150};
    color_t black = {0, 0, 0};
    
    for(int i = 0; i < 7; i++) 
    {
        lcd_show_text_32x32(x, y, text[61], black);//星
        lcd_show_text_32x32(x+34, y, text[51], black);//期
        if(weekdays[i]==false)
        {
            if(i==0)
                lcd_show_text_32x32(x+68, y, text[7], black);
            else
                lcd_show_text_32x32(x+68, y, text[i], black);
        }
    }
}

void add_week_alarm(int hour, int min, int sec, bool weekdays[7])
{
    if(week_alarm_count >= MAX_WEEK_ALARMS) 
    {
        printf("星期闹钟已满，最多 %d 个\n", MAX_WEEK_ALARMS);
        return;
    }
    
    week_alarms[week_alarm_count].hour = hour;
    week_alarms[week_alarm_count].min = min;
    week_alarms[week_alarm_count].sec = sec;
    for(int i = 0; i < 7; i++) 
        week_alarms[week_alarm_count].weekdays[i] = weekdays[i];
    week_alarms[week_alarm_count].enabled = true;
    week_alarm_count++;
    printf("星期闹钟已添加\n");
}

void delete_week_alarm(int index)
{
    if(index < 0 || index >= week_alarm_count)
        return;
    
    for(int i = index; i < week_alarm_count - 1; i++) 
        week_alarms[i] = week_alarms[i + 1];
    week_alarm_count--;
    printf("星期闹钟已删除\n");
}

void show_week_alarm_list(color_t color)
{
    color_t font_color = {255, 255, 255};
    color_t black = {0, 0, 0};
    color_t gray = {204, 204, 204};
    char *week_names[] = {"日", "一", "二", "三", "四", "五", "六"};
    lcd_color_block_drawing(130, 130, 800, 350, color);
    
    if(week_alarm_count == 0) 
        return;
    
    for(int i = 0; i < week_alarm_count && i < 4; i++) 
    {
        int y_pos = 140 + 100 * i;
        lcd_color_block_drawing(180, y_pos, 676, 80, font_color);
        
        lcd_show_text_16x32(200, y_pos + 25, ch[week_alarms[i].hour / 10], black);
        lcd_show_text_16x32(218, y_pos + 25, ch[week_alarms[i].hour % 10], black);
        lcd_show_text_16x32(236, y_pos + 25, ch[10], black);  // :
        lcd_show_text_16x32(254, y_pos + 25, ch[week_alarms[i].min / 10], black);
        lcd_show_text_16x32(272, y_pos + 25, ch[week_alarms[i].min % 10], black);
        lcd_show_text_16x32(290, y_pos + 25, ch[10], black);  // :
        lcd_show_text_16x32(308, y_pos + 25, ch[week_alarms[i].sec / 10], black);
        lcd_show_text_16x32(326, y_pos + 25, ch[week_alarms[i].sec % 10], black);
        
        show_weekday_names(week_alarms[i].weekdays, 365, y_pos + 25, color);
        
        lcd_color_block_drawing(750, y_pos, 106, 82, gray);
        lcd_show_text_32x32(770, y_pos + 20, text[77], font_color);  // 删
        lcd_show_text_32x32(804, y_pos + 20, text[78], font_color);  // 除
    }
}

int alarm_clock_week(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color = {255, 255, 255};
    color_t gray = {204, 204, 204};
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    
    lcd_color_block_drawing(462, 480, 100, 70, gray);
    lcd_show_text_32x32(475, 499, text[62], font_color);  // 添
    lcd_show_text_32x32(516, 499, text[63], font_color);  // 加

    show_week_alarm_list(color);
    
    while(1) 
    {
        int x = -1, y = -1;
        ts_get_axes(&x, &y);
        if(x == -1 || y == -1) 
            continue;
        
        if(ts_click_area(x, y, 50, 50, 80, 80)) 
            return 0;
        
        for(int i = 0; i < week_alarm_count && i < 4; i++) 
        {
            int y_pos = 140 + 100 * i;
            if(ts_click_area(x, y, 750, y_pos + 5, 106, 72)) 
            {
                printf("点击删除星期闹钟 %d\n", i);
                delete_week_alarm(i);
                
                lcd_color_block_drawing(0, 0, 1024, 600, color);
                lcd_draw_pic("./PIC/back.bmp", 50, 50);
                lcd_color_block_drawing(462, 480, 100, 70, gray);
                lcd_show_text_32x32(475, 499, text[62], font_color);
                lcd_show_text_32x32(516, 499, text[63], font_color);
                show_week_alarm_list(color);
                break;
            }
        }
        
        if(ts_click_area(x, y, 462, 480, 100, 70)) 
        {
            bool selected_weekdays[7] = {false};
            if(!select_weekdays_page(color, selected_weekdays))
                continue; 
            
            printf("选中的星期: ");
            for(int i = 0; i < 7; i++) 
                if(selected_weekdays[i]) 
                    printf("%d ", i);
            printf("\n");
            
            int selected_hour = alarm_input_number(color, "小时");
            if(selected_hour == -1) 
                continue;
            if(selected_hour < 0 || selected_hour > 23) 
            {
                printf("小时无效: %d\n", selected_hour);
                continue;
            }
            
            int selected_min = alarm_input_number(color, "分钟");
            if(selected_min == -1)
                continue;
            if(selected_min < 0 || selected_min > 59) 
            {
                printf("分钟无效: %d\n", selected_min);
                continue;
            }
            
            int selected_sec = alarm_input_number(color, "秒钟");
            if(selected_sec == -1)            
                continue;
            if(selected_sec < 0 || selected_sec > 59) 
            {
                printf("秒钟无效: %d\n", selected_sec);
                continue;
            }
            
            add_week_alarm(selected_hour, selected_min, selected_sec, selected_weekdays);

            lcd_color_block_drawing(0, 0, 1024, 600, color);
            lcd_draw_pic("./PIC/back.bmp", 50, 50);
            lcd_color_block_drawing(462, 480, 100, 70, gray);
            lcd_show_text_32x32(475, 499, text[62], font_color);
            lcd_show_text_32x32(516, 499, text[63], font_color);
            show_week_alarm_list(color);
        }
    }
}
#endif

int alarm_clock(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体

    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_draw_pic("./PIC/Day.bmp", 212, 180);
    lcd_show_text_32x32(204, 390, text[58], font_color);//按
    lcd_show_text_32x32(241, 390, text[9], font_color);//天
    lcd_show_text_32x32(278, 390, text[59], font_color);//设
    lcd_show_text_32x32(315, 390, text[60], font_color);//置
    lcd_show_text_32x32(352, 390, text[54], font_color);//闹
    lcd_show_text_32x32(389, 390, text[55], font_color);//钟
    lcd_draw_pic("./PIC/Week.bmp", 612, 180);
    lcd_show_text_32x32(585, 390, text[58], font_color);//按
    lcd_show_text_32x32(622, 390, text[61], font_color);//星
    lcd_show_text_32x32(659, 390, text[51], font_color);//期
    lcd_show_text_32x32(696, 390, text[59], font_color);//设
    lcd_show_text_32x32(733, 390, text[60], font_color);//置
    lcd_show_text_32x32(770, 390, text[54], font_color);//闹
    lcd_show_text_32x32(807, 390, text[55], font_color);//钟

    while(1)
    {
        int x=-1, y=-1;
        ts_get_axes(&x, &y);
        if(x==-1 ||y==-1)
            continue;

        if(ts_click_area(x, y, 50, 50, 80, 80))
        {
            pop_state();
            return 0;
        }
        else
        {
            if(ts_click_area(x, y, 212, 180, 200, 200))
            {
                printf("点击按天设置闹钟\n");
                alarm_clock_day(color);
            }
            else if(ts_click_area(x, y, 612, 180, 200, 200))
            {
                printf("点击按星期设置闹钟\n");
                alarm_clock_week(color);
            }
            else
                continue;

            lcd_color_block_drawing(0, 0, 1024, 600, color);
            lcd_draw_pic("./PIC/back.bmp", 50, 50);
            lcd_draw_pic("./PIC/Day.bmp", 212, 180);
            lcd_show_text_32x32(204, 390, text[58], font_color);//按
            lcd_show_text_32x32(241, 390, text[9], font_color);//天
            lcd_show_text_32x32(278, 390, text[59], font_color);//设
            lcd_show_text_32x32(315, 390, text[60], font_color);//置
            lcd_show_text_32x32(352, 390, text[54], font_color);//闹
            lcd_show_text_32x32(389, 390, text[55], font_color);//钟
            lcd_draw_pic("./PIC/Week.bmp", 612, 180);
            lcd_show_text_32x32(585, 390, text[58], font_color);//按
            lcd_show_text_32x32(622, 390, text[61], font_color);//星
            lcd_show_text_32x32(659, 390, text[51], font_color);//期
            lcd_show_text_32x32(696, 390, text[59], font_color);//设
            lcd_show_text_32x32(733, 390, text[60], font_color);//置
            lcd_show_text_32x32(770, 390, text[54], font_color);//闹
            lcd_show_text_32x32(807, 390, text[55], font_color);//钟
        }
    }
}
