#include "main_interface.h"

int x=-1, y=-1;
int touch_x = -1, touch_y = -1;

int wait_touch_or_timeout(int timeout_sec)
{
    fd_set set;
    struct timeval tv;
    int ret;
    struct input_event ev;

    // 先清空触摸事件队列（非阻塞读取）
    int flags = fcntl(ts_fd, F_GETFL, 0);
    fcntl(ts_fd, F_SETFL, flags | O_NONBLOCK);
    // 丢弃所有待处理事件
    while(read(ts_fd, &ev, sizeof(ev)) > 0);
    // 恢复阻塞模式
    fcntl(ts_fd, F_SETFL, flags);

    FD_ZERO(&set);
    FD_SET(ts_fd, &set);
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;

    ret = select(ts_fd + 1, &set, NULL, NULL, &tv);
    printf("ret=%d\n");
    if (ret > 0) 
        return 1;  // 有触摸事件
    else if (ret == 0)
        return 0;  // 超时
    else
        return -1; // 错误
}

int main(int argc, char const *argv[])
{
    if(lcd_init()==-1)
        return -1;
    if(ts_init()==-1)
        return -1;

    color_t background={255, 220, 212};//淡蓝色背景
    color_t font_color={255, 255, 255};//白色字体
    int ret;
    enum {
        STATE_BLACK = 0,      // 黑屏
        STATE_LOCKED = 1,     // 锁屏
        STATE_MAIN = 2        // 主界面
    } state = STATE_BLACK;

    lcd_clear();
    while(1)
    {
        ret=wait_touch_or_timeout(5);
        if(ret==1)
        { 
            int gesture=ts_detect_touch_gestures(&x, &y);            
            if(x == -1 || y == -1) 
                continue;
            printf("触摸: (%d,%d), 手势: %d, 状态: %d\n", x, y, gesture, state);
            
            switch(state) 
            {
                case STATE_BLACK:
                    // 黑屏时触摸 → 显示锁屏
                    state=STATE_LOCKED;
                    lcd_clear();
                    lcd_screen_off(font_color);
                    sleep(1);
                    break;

                case STATE_LOCKED:
                    if(gesture==SWIPE_UP)
                    {
                        lcd_clear();
                        lcd_draw_pic("./PIC/number1.bmp", 272, 120);
                        int chance=0;
                        for(chance=0; chance<5; chance++)
                        {   
                            if(pwd(font_color))
                                break;
                            else
                                printf("密码输入错误\n");
                        }
                        if(chance!=5)
                        {
                            state=STATE_MAIN;
                            main_interface(background);
                            break;
                        }
                        lcd_screen_lock(font_color);
                        state=STATE_LOCKED;
                        lcd_clear();
                        lcd_screen_off(font_color);
                        break;
                    }
                    break;

                case STATE_MAIN:                    
                    break;
            }
        }
        else if(ret == 0)
        {            
            // 5秒无触摸 → 黑屏
            if(state != STATE_BLACK)
            {
                state = STATE_BLACK;
                color_t black = {0, 0, 0};
                lcd_color_block_drawing(0, 0, 1024, 600, black);
                printf("清屏幕了\n");
            }
        }
        else
        {
            perror("select error");
            break;
        }
    }

    ts_close();
    lcd_close();

    return 0;
}
