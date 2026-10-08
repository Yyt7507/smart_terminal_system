#include "timer_block.h"

typedef struct {
    int hour;
    int min;
    int sec;
    bool running;
    bool finished;
    bool paused;
} Timer;
static Timer tmr = {0, 0, 0, false, false, false};
static int timer_running = 1;//进入计时界面
static volatile int timer_need_refresh = 0;

void *timer_countdown_thread(void *arg)
{
    while(timer_running)//进入到计时界面时timer_running==true
    {
        if(tmr.running && !tmr.finished)//点击开始按钮后sw.running==true，且倒计时没有完成finished==false
        {
            tmr.sec--;
            if(tmr.sec < 0) 
            {
                tmr.sec = 59;
                tmr.min--;
                if(tmr.min < 0) 
                {
                    tmr.min = 59;
                    tmr.hour--;
                    if(tmr.hour < 0) 
                    {
                        tmr.hour = 0;
                        tmr.min = 0;
                        tmr.sec = 0;
                        tmr.running = false;//停止倒计时
                        tmr.finished = true;//倒计时完成
                        timer_need_refresh = 1;
                        printf("倒计时结束！\n");
                    }
                }
            }
            timer_need_refresh = 1;//代表你要刷新一次lcd屏了
        }
        sleep(1);
    }
    return NULL;
}

void show_timer_digit(int num, int x, int y, color_t color)
{
    int tens = num / 10;
    int ones = num % 10;
    lcd_show_text_16x32(x, y, ch[tens], color);
    lcd_show_text_16x32(x + 18, y, ch[ones], color);
}

void show_timer_time(color_t color)
{
    color_t white = {255, 255, 255};
    color_t black = {0, 0, 0};
    color_t font_color = {0, 0, 0};
    color_t red = {0, 0, 255}; 
    
    if(tmr.finished) 
        font_color = red;
    
    int total_width=150;
    int x_start=(1024-total_width)/2;
    int y_start=220;
    lcd_color_block_drawing(x_start-14, y_start-14, total_width+28, 32+28, white);
    
    show_timer_digit(tmr.hour, x_start, y_start, font_color);
    lcd_show_text_16x32(x_start+38, y_start, ch[10], black);
    show_timer_digit(tmr.min, x_start+58, y_start, font_color);
    lcd_show_text_16x32(x_start+96, y_start, ch[10], black);
    show_timer_digit(tmr.sec, x_start+116, y_start, font_color);
}

int timer_input_number(color_t color, char *tip)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color = {255, 255, 255};
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_draw_pic("./PIC/number1.bmp", 272, 120);
    char keyboard[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'d', '0', 'o'}
    };
    char input[3] = {0};

    lcd_show_text_32x32(412, 42, text[66], font_color);//请
    lcd_show_text_32x32(454, 42, text[70], font_color);//输
    lcd_show_text_32x32(496, 42, text[71], font_color);//入
    
    if(strcmp(tip, "小时") == 0) 
    {
        lcd_show_text_32x32(538, 42, text[18], font_color);
        lcd_show_text_32x32(580, 42, text[19], font_color);
    } 
    else if(strcmp(tip, "分钟") == 0) 
    {
        lcd_show_text_32x32(538, 42, text[72], font_color);
        lcd_show_text_32x32(580, 42, text[20], font_color);
    } 
    else if(strcmp(tip, "秒钟") == 0) 
    {
        lcd_show_text_32x32(538, 42, text[17], font_color);
        lcd_show_text_32x32(580, 42, text[20], font_color);
    }

    int x, y;
    int i, j, n = 0;
    while(1) 
    {
        ts_get_axes(&x, &y);
        if(ts_click_area(x, y, 50, 50, 80, 80)) 
            return -1;
        for(j = 0; j < 4; j++) 
        {
            for(i = 0; i < 3; i++) 
            {
                if(ts_click_area(x, y, 272 + 160*i, 120 + 120*j, 160, 120)) 
                {
                    if(keyboard[j][i] == 'd') 
                    {
                        if(n == 0) 
                        {
                            input[0] = 0;
                            printf("input is empty\n");
                        } 
                        else 
                        {
                            input[--n] = 0;
                        }
                    } 
                    else if(keyboard[j][i] == 'o') 
                    {
                        if(n == 0) 
                        {
                            printf("请先输入数字\n");
                        } 
                        else 
                        {
                            int result = 0;
                            for(int k = 0; k < n; k++) 
                            {
                                result = result * 10 + (input[k] - '0');
                            }
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
                        {
                            printf("最多输入2位\n");
                        }
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

int timer(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color = {255, 255, 255};
    color_t gray = {204, 204, 204};
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_show_text_32x32(475, 20, text[57], font_color);//计
    lcd_show_text_32x32(517, 20, text[19], font_color);//时

    tmr.hour = 0;
    tmr.min = 0;
    tmr.sec = 0;
    tmr.running = false;
    tmr.finished = false;
    tmr.paused = false;
    timer_running = 1;
    timer_need_refresh = 0;
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, timer_countdown_thread, NULL);
        
    show_timer_time(color);
    
    int btn_w = 120;
    int btn_h = 70;
    int gap = 30;
    int total_btn_w = btn_w * 3 + gap * 2;
    int btn_start_x = (1024 - total_btn_w) / 2;
    int btn_start_y = 400;
    
    color_t set_color = {80, 150, 255};
    lcd_color_block_drawing(btn_start_x, btn_start_y, btn_w, btn_h, set_color);
    lcd_show_text_32x32(btn_start_x + 30, btn_start_y + 20, text[59], font_color);  // 设
    lcd_show_text_32x32(btn_start_x + 62, btn_start_y + 20, text[60], font_color);  // 置
    
    color_t start_color = {80, 200, 80};
    lcd_color_block_drawing(btn_start_x + btn_w + gap, btn_start_y, btn_w, btn_h, start_color);
    lcd_show_text_32x32(btn_start_x + btn_w + gap + 30, btn_start_y + 20, text[79], font_color);  // 开
    lcd_show_text_32x32(btn_start_x + btn_w + gap + 62, btn_start_y + 20, text[80], font_color);  // 始
    
    color_t pause_color = {200, 200, 80};
    lcd_color_block_drawing(btn_start_x + (btn_w + gap) * 2, btn_start_y, btn_w, btn_h, pause_color);
    lcd_show_text_32x32(btn_start_x + (btn_w + gap) * 2 + 30, btn_start_y + 20, text[81], font_color);  // 暂
    lcd_show_text_32x32(btn_start_x + (btn_w + gap) * 2 + 62, btn_start_y + 20, text[82], font_color);  // 停
    
    color_t reset_color = {200, 80, 80};
    int reset_x = (1024 - btn_w) / 2;
    lcd_color_block_drawing(reset_x, btn_start_y + btn_h + gap, btn_w, btn_h, reset_color);
    lcd_show_text_32x32(reset_x + 30, btn_start_y + btn_h + gap + 20, text[83], font_color);  // 重
    lcd_show_text_32x32(reset_x + 62, btn_start_y + btn_h + gap + 20, text[84], font_color);  // 置
    
    int last_sec = -1;
    int last_min = -1;
    int last_hour = -1;
    
    while(1) 
    {
        if(timer_need_refresh || tmr.sec != last_sec || tmr.min != last_min || tmr.hour != last_hour) 
        {
            show_timer_time(color);
            last_sec = tmr.sec;
            last_min = tmr.min;
            last_hour = tmr.hour;
            timer_need_refresh = 0;
            
            if(tmr.finished)             
                printf("倒计时结束！\n");                
        }

        //使用select等待触摸事件，但最多等100ms
        fd_set readfds;
        struct timeval tv;
        int ret;        
        FD_ZERO(&readfds);
        FD_SET(ts_fd, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        ret = select(ts_fd + 1, &readfds, NULL, NULL, &tv);
        //处理触摸事件 -- 若有触摸事件发生，大概率是按下，故需要再判断是否有第二个触摸事件发生，若有则读取ts_fd数据
        if(ret > 0) 
        {
            int x = -1, y = -1;
            struct input_event ev;
            int touch_complete = 0;
            
            struct timeval read_tv;
            read_tv.tv_sec = 0;
            read_tv.tv_usec = 50000;
            
            while(!touch_complete)//套一个循环，首先是为了当第二个触摸事件未发生时，能直接退出触摸事件判断流程
            {
                fd_set touch_fds;
                FD_ZERO(&touch_fds);
                FD_SET(ts_fd, &touch_fds);
                
                int read_ret = select(ts_fd + 1, &touch_fds, NULL, NULL, &read_tv);
                if(read_ret <= 0) 
                    break; 
                
                memset(&ev, 0, sizeof(ev));
                read(ts_fd, &ev, sizeof(ev));
                
                if(ev.type == EV_ABS && ev.code == ABS_X) 
                    x = ev.value;
                else if(ev.type == EV_ABS && ev.code == ABS_Y) 
                    y = ev.value;
                else if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0) 
                {
                    touch_complete = 1;
                    break;
                }
            }

            if(x == -1 || y == -1) 
                continue;
            
            if(ts_click_area(x, y, 50, 50, 80, 80)) 
            {
                timer_running = 0;//退出计时界面
                tmr.running = false;//停止计时
                pthread_join(timer_thread, NULL);
                return 0;
            }
            
            if(ts_click_area(x, y, btn_start_x, btn_start_y, btn_w, btn_h))//按下设置按钮
            {
                tmr.running = false;//若当前有计时正在进行也停止                
                int set_hour = timer_input_number(color, "小时");
                if(set_hour == -1) 
                {   
                    // 如果之前已经设置过时间（不全为0），保持设置但确保停止状态
                    if(tmr.hour != 0 || tmr.min != 0 || tmr.sec != 0) 
                    {
                        tmr.running = false;//停止计时
                        tmr.finished = false;//清除完成标志
                    }
                    show_timer_time(color);
                    continue;
                }
                if(set_hour < 0 || set_hour > 23) 
                {
                    printf("小时无效: %d\n", set_hour);
                    continue;
                }
                
                int set_min = timer_input_number(color, "分钟");
                if(set_min == -1) 
                {
                    show_timer_time(color);
                    continue;
                }
                if(set_min < 0 || set_min > 59) 
                {
                    printf("分钟无效: %d\n", set_min);
                    continue;
                }
                
                int set_sec = timer_input_number(color, "秒钟");
                if(set_sec == -1) 
                {
                    show_timer_time(color);
                    continue;
                }
                if(set_sec < 0 || set_sec > 59) 
                {
                    printf("秒钟无效: %d\n", set_sec);
                    continue;
                }
                
                tmr.hour = set_hour;
                tmr.min = set_min;
                tmr.sec = set_sec;
                tmr.running = false;//计时未开始
                tmr.finished = false;//计时未完成
                tmr.paused = false;//计时未暂停
                printf("倒计时已设置: %02d:%02d:%02d\n", tmr.hour, tmr.min, tmr.sec);

                lcd_color_block_drawing(0, 0, 1024, 600, color);
                show_timer_time(color);
                lcd_draw_pic("./PIC/back.bmp", 50, 50);
                lcd_show_text_32x32(450, 20, text[57], font_color);//计
                lcd_show_text_32x32(492, 20, text[19], font_color);//时
                lcd_color_block_drawing(btn_start_x, btn_start_y, btn_w, btn_h, set_color);
                lcd_show_text_32x32(btn_start_x + 30, btn_start_y + 20, text[59], font_color);//设
                lcd_show_text_32x32(btn_start_x + 62, btn_start_y + 20, text[60], font_color);//置
                lcd_color_block_drawing(btn_start_x + btn_w + gap, btn_start_y, btn_w, btn_h, start_color);
                lcd_show_text_32x32(btn_start_x + btn_w + gap + 30, btn_start_y + 20, text[79], font_color);//开
                lcd_show_text_32x32(btn_start_x + btn_w + gap + 62, btn_start_y + 20, text[80], font_color);//始
                lcd_color_block_drawing(btn_start_x + (btn_w + gap) * 2, btn_start_y, btn_w, btn_h, pause_color);
                lcd_show_text_32x32(btn_start_x + (btn_w + gap) * 2 + 30, btn_start_y + 20, text[81], font_color);//暂
                lcd_show_text_32x32(btn_start_x + (btn_w + gap) * 2 + 62, btn_start_y + 20, text[82], font_color);//停
                lcd_color_block_drawing(reset_x, btn_start_y + btn_h + gap, btn_w, btn_h, reset_color);
                lcd_show_text_32x32(reset_x + 30, btn_start_y + btn_h + gap + 20, text[83], font_color);//重
                lcd_show_text_32x32(reset_x + 62, btn_start_y + btn_h + gap + 20, text[84], font_color);//置
            }
            
            if(ts_click_area(x, y, btn_start_x + btn_w + gap, btn_start_y, btn_w, btn_h))//按下开始按钮
            {
                if(tmr.hour == 0 && tmr.min == 0 && tmr.sec == 0) 
                {
                    printf("请先设置倒计时时间\n");
                    continue;
                }
                
                if(tmr.finished)
                {
                    tmr.finished = false;//确保你按下开始按钮时，计时是未完成的
                }
                
                if(!tmr.running) 
                {
                    tmr.running = true;//开始计时
                    tmr.paused = false;//计时未暂停
                    printf("倒计时开始\n");
                    show_timer_time(color);
                }
            }
            
            if(ts_click_area(x, y, btn_start_x + (btn_w + gap) * 2, btn_start_y, btn_w, btn_h))//按下暂停按钮
            {
                if(tmr.running)//若正在计时
                {
                    tmr.running = false;//停止计时
                    tmr.paused = true;//计时暂停
                    printf("倒计时暂停\n");
                    show_timer_time(color);
                } 
                else if(tmr.paused && !tmr.finished)//再次按下暂停且计时未完成 == 开始计时
                {
                    tmr.running = true;//开始计时
                    tmr.paused = false;//暂停计时
                    printf("倒计时继续\n");
                    show_timer_time(color);
                } 
                else 
                {
                    printf("没有正在运行的倒计时\n");
                }
            }
            
            if(ts_click_area(x, y, reset_x, btn_start_y + btn_h + gap, btn_w, btn_h))//按下重置按钮
            {
                tmr.hour = 0;
                tmr.min = 0;
                tmr.sec = 0;
                tmr.running = false;//停止计时
                tmr.finished = false;//计时未完成
                tmr.paused = false;//未暂停计时
                printf("倒计时重置\n");
                show_timer_time(color);
            }
        }
    }
}
