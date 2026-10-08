#include "ts.h"

int ts_fd=-1;
int state_stack[MAX_STACK];
int stack_top = 0;

void push_state(int new_state)
{
    if (stack_top < MAX_STACK) 
        state_stack[stack_top++] = new_state;
}

int pop_state(void)
{
    if (stack_top > 0) 
        return state_stack[--stack_top];
    return STATE_MAIN;  // 默认返回主界面
}

int get_current_state(void)
{
    if (stack_top == 0)
        return STATE_MAIN;
    return state_stack[stack_top - 1];
}

int ts_init(void)
{
    ts_fd=open("/dev/input/event2", O_RDONLY);
    if(ts_fd==-1)
    {
        perror("open touch screen error");
        return -1;
    }
    return 0;
}

int ts_close(void)
{
    close(ts_fd);
}

int ts_get_axes(int *x, int *y)
{
    struct input_event ev;
    while(1)
    {
        memset(&ev, 0, sizeof(ev));
        read(ts_fd, &ev, sizeof(ev));
        if(ev.type==EV_ABS && ev.code==ABS_X)
            *x=ev.value;
        else if(ev.type==EV_ABS && ev.code==ABS_Y)
            *y=ev.value;
        else if(ev.type==EV_KEY && ev.code==BTN_TOUCH && ev.value==0)
            break;
        else
            continue;
    }
    return 0;
}

bool ts_click_area(int x, int y, int x_start, int y_start, int width, int height)
{
    if(x>=x_start &&x<=x_start+width && y>=y_start &&y <=y_start+height)
        return true;
    else 
        return false;
}

int ts_detect_touch_gestures(int *x, int *y)
{
    struct input_event ev;
    int x_start=-1, y_start=-1;
    int x_last=-1, y_last=-1;
    while(1)
    {
        memset(&ev, 0, sizeof(ev));
        read(ts_fd, &ev, sizeof(ev));
        if(ev.type==EV_ABS && ev.code==ABS_X)
        {
            if(x_start==-1)
                x_start=ev.value;
            else
                x_last=ev.value;
        }
        else if(ev.type==EV_ABS && ev.code==ABS_Y)
        {
            if(y_start==-1)
                y_start=ev.value;
            else
                y_last=ev.value;
        }
        else if(ev.type==EV_KEY && ev.code==BTN_TOUCH && ev.value==0)
            break;
        else
            continue;
    }

    if((x_last==-1 && y_last==-1) || (abs(x_last -x_start)<20 && abs(y_last-y_start)<20))
    {
        if(x_start!=-1)
            *x=x_start;
        if(y_start!=-1)
            *y=y_start;
        return TOUCH;
    }
    if(abs(x_last-x_start) > abs(y_last-y_start))
    {
        if(x_last - x_start <0)
            return SWIPE_LEFT;
        else
            return SWIPE_RIGHT;
    }
    else
    {
        if(y_last - y_start <0)
            return SWIPE_UP;
        else
            return SWIPE_DOWN;
    }
}

bool pwd(color_t color)
{
	char keyboard[4][3] = {
		{'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'d', '0', 'o'}
	};
	
	char *password = "123456";
	char input[20] = {0};
	
	int x,y;
	int i,j,n = 0;
	while(1)
	{
		ts_get_axes(&x,&y);
		printf("<%d,%d>\n",x,y);
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
						printf("input = %s\n",input);
					}
					else if(keyboard[j][i] == 'o')
					{
						if(strcmp(input,password) == 0)
							return true;
						else
							return 0;
					}
					else
					{
						if(n == 20)
							printf("密码最多输入20位，已达到上限\n");
						else
							input[n++] = keyboard[j][i];
						printf("input = %s\n",input);
					}

                    color_t balck={0, 0, 0};
                    lcd_color_block_drawing(300, 50, 480, 32, balck);
                    for(int k=0; k<n; k++)
                    {
                        lcd_show_text_16x32(340+16*k+10, 50, ch[11], color);
                    }
				}
			}
		}
	}
}



int lamp(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体

    lcd_draw_pic("./PIC/back.bmp", 50, 50);
}

int air(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体

    lcd_draw_pic("./PIC/back.bmp", 50, 50);
}
