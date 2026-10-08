#include "stopwatch_block.h"

typedef struct {
    int hour;
    int min;
    int sec;
    bool running;
    bool paused;
} Stopwatch;
static Stopwatch sw = {0, 0, 0, false, false};
static int stopwatch_running = 1;//进入到秒表界面
static volatile int need_refresh = 0;

void *stopwatch_timer_thread(void *arg)
{
    while(stopwatch_running)//进入到秒表界面时stopwatch_running==true
    {
        if(sw.running)//点击开始按钮后sw.running==true
        {
            sw.sec++;
            if(sw.sec >= 60) 
            {
                sw.sec = 0;
                sw.min++;
                if(sw.min >= 60) 
                {
                    sw.min = 0;
                    sw.hour++;
                    if(sw.hour >= 24) 
                        sw.hour = 0;
                }
            }
            need_refresh = 1;//代表你要刷新一次lcd屏了
        }
        sleep(1);
    }
    return NULL;
}

void show_stopwatch_digit(int num, int x, int y, color_t color)
{
    int tens = num / 10;
    int ones = num % 10;
    lcd_show_text_16x32(x, y, ch[tens], color);
    lcd_show_text_16x32(x + 18, y, ch[ones], color);
}

void show_stopwatch_time(color_t color)
{
    color_t white = {255, 255, 255};
    color_t black = {0, 0, 0};
    color_t font_color = {0, 0, 0};
    
    int total_width=150;
    int x_start=(1024-total_width)/2;
    int y_start=220;
    lcd_color_block_drawing(x_start-14, y_start-14, total_width+28, 32+28, white);
    
    show_stopwatch_digit(sw.hour, x_start, y_start, font_color);//"时"
    lcd_show_text_16x32(x_start+38, y_start, ch[10], black);//:    
    show_stopwatch_digit(sw.min, x_start+58, y_start, font_color);//“分”
    lcd_show_text_16x32(x_start+96, y_start, ch[10], black);//:    
    show_stopwatch_digit(sw.sec, x_start+116, y_start, font_color);//"秒"
}

int stopwatch(color_t color)
{    
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color = {255, 255, 255};
    color_t gray = {204, 204, 204};    
    lcd_draw_pic("./PIC/back.bmp", 50, 50);
    lcd_show_text_32x32(475, 20, text[17], font_color);//秒
    lcd_show_text_32x32(517, 20, text[56], font_color);//表

    sw.hour = 0;
    sw.min = 0;
    sw.sec = 0;
    sw.running = false;//没按下开始按钮
    sw.paused = false;//秒表未暂停
    stopwatch_running = 1; //进入到秒表界面
    pthread_t stopwatch_thread;    
    pthread_create(&stopwatch_thread, NULL, stopwatch_timer_thread, NULL);

    show_stopwatch_time(color);
    
    int btn_w = 120;
    int btn_h = 70;
    int gap = 30;
    int total_btn_w = btn_w * 3 + gap * 2;
    int btn_start_x = (1024 - total_btn_w) / 2;
    int btn_start_y = 400;
    
    color_t start_color = {80, 200, 80};
    lcd_color_block_drawing(btn_start_x, btn_start_y, btn_w, btn_h, start_color);
    lcd_show_text_32x32(btn_start_x + 27, btn_start_y + 19, text[79], font_color);//开
    lcd_show_text_32x32(btn_start_x + 61, btn_start_y + 19, text[80], font_color);//始
    
    color_t pause_color = {200, 200, 80};
    lcd_color_block_drawing(btn_start_x + btn_w + gap, btn_start_y, btn_w, btn_h, pause_color);
    lcd_show_text_32x32(btn_start_x + btn_w + gap + 27, btn_start_y + 19, text[81], font_color);//暂
    lcd_show_text_32x32(btn_start_x + btn_w + gap + 61, btn_start_y + 19, text[82], font_color);//停
    
    color_t reset_color = {200, 80, 80};
    lcd_color_block_drawing(btn_start_x + (btn_w + gap) * 2, btn_start_y, btn_w, btn_h, reset_color);
    lcd_show_text_32x32(btn_start_x + (btn_w + gap) * 2 + 27, btn_start_y + 19, text[83], font_color);//重
    lcd_show_text_32x32(btn_start_x + (btn_w + gap) * 2 + 61, btn_start_y + 19, text[84], font_color);//置
    
    while(1) 
    {
        if(need_refresh && sw.running) 
        {
            show_stopwatch_time(color);
            need_refresh = 0;//若是刷新一次lcd屏后，不将need_refresh置0,那么每次循环（时间未过1秒）就需要你不停的刷新
            printf("刷新显示: %02d:%02d:%02d\n", sw.hour, sw.min, sw.sec);
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
            int x_start = -1, y_start = -1;
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
                //不使用ts_get_axes函数，是由于在该函数内，无法直接通过触摸结束这个条件来退出循环while(!touch_complete)
                memset(&ev, 0, sizeof(ev));
                read(ts_fd, &ev, sizeof(ev));
                
                if(ev.type == EV_ABS && ev.code == ABS_X) 
                {
                    if(x_start == -1) x_start = ev.value;
                    x = ev.value;
                }
                else if(ev.type == EV_ABS && ev.code == ABS_Y) 
                {
                    if(y_start == -1) y_start = ev.value;
                    y = ev.value;
                }
                else if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0) 
                {
                    touch_complete = 1;//触摸结束，退出循环while(!touch_complete)
                    break;
                }
            }
            
            if(x == -1 && y == -1) 
                continue;
            
            if(ts_click_area(x, y, 50, 50, 80, 80)) 
            {
                stopwatch_running = 0;//退出秒表界面
                sw.running = false;//停止计秒
                sw.paused = false;//计时未暂停
                pthread_join(stopwatch_thread, NULL);
                return 0;
            }
            
            if(ts_click_area(x, y, btn_start_x, btn_start_y, btn_w, btn_h))//按下开始按钮
            {
                if(!sw.running) 
                {
                    sw.running = true;//计时开始
                    sw.paused = false;//计时未暂停
                    printf("秒表开始\n");
                    show_stopwatch_time(color);
                }
            }
            
            if(ts_click_area(x, y, btn_start_x + btn_w + gap, btn_start_y, btn_w, btn_h))//按下暂停按钮
            {
                if(sw.running) 
                {
                    sw.running = false;//停止计时
                    sw.paused = true;//暂停计时
                    printf("秒表暂停\n");
                    show_stopwatch_time(color);
                }
                else if(sw.running==false && sw.paused==true)
                {
                    sw.running = true;//开始计时
                    sw.paused = false;//计时未暂停
                    printf("秒表继续计时\n");
                    show_stopwatch_time(color);
                }
                else 
                {
                    printf("没有正在运行的秒表\n");
                }
            }
            
            if(ts_click_area(x, y, btn_start_x + (btn_w + gap) * 2, btn_start_y, btn_w, btn_h))//按下重置按钮
            {
                sw.hour = 0;
                sw.min = 0;
                sw.sec = 0;
                sw.running = false;
                sw.paused = false;
                printf("秒表重置\n");
                show_stopwatch_time(color);
            }            
        }
        //如果select超时（ret==0），继续循环检查need_refresh，刷新lcd界面
    }
}
