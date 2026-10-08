#include "weather_block.h"

static int current_page = 0;
#define     ITEMS_PER_PAGE      7

int timeToch(const char *str, color_t color, int x_start)
{
    int len = strlen(str);
    char result[4]={0};
    if (len < 4) {
        result[0] = result[1] = result[2] = result[3] = 0;
        return 0;
    }
    
    result[0] = str[len - 4] - '0';
    result[1] = str[len - 3] - '0';
    result[2] = str[len - 2] - '0';
    result[3] = str[len - 1] - '0';
    
    lcd_show_text_16x32(x_start+18*0, 434, ch[(result[0])], color);
    lcd_show_text_16x32(x_start+18*1, 434, ch[(result[1])], color);
    lcd_show_text_16x32(x_start+18*2, 434, ch[10], color);
    lcd_show_text_16x32(x_start+18*3, 434, ch[(result[2])], color);
    lcd_show_text_16x32(x_start+18*4, 434, ch[(result[3])], color);
}

int dataToch(const char *str, color_t color, int x_start)
{
    int len = strlen(str);
    char result[4]={0};
    if (len < 4) {
        result[0] = result[1] = result[2] = result[3] = 0;
        return 0;
    }
    
    result[0] = str[len - 4] - '0';
    result[1] = str[len - 3] - '0';
    result[2] = str[len - 2] - '0';
    result[3] = str[len - 1] - '0';
    
    lcd_show_text_16x32(x_start+18*0, 434, ch[(result[0])], color);
    lcd_show_text_16x32(x_start+18*1, 434, ch[(result[1])], color);
    lcd_show_text_16x32(x_start+18*2, 434, ch[13], color);
    lcd_show_text_16x32(x_start+18*3, 434, ch[(result[2])], color);
    lcd_show_text_16x32(x_start+18*4, 434, ch[(result[3])], color);
}

int wind_directionTotext(const char *str, color_t color, int x_start)
{
    int len=strlen(str);
    if(len==2)
    {
        if(strcmp(str, "北风"))
            lcd_show_text_32x32(x_start+38*0, 342, text[42], color);
        else if(strcmp(str, "南风"))
            lcd_show_text_32x32(x_start+38*0, 342, text[43], color);
        else if(strcmp(str, "西风"))
            lcd_show_text_32x32(x_start+38*0, 342, text[44], color);
        else if(strcmp(str, "东风"))
            lcd_show_text_32x32(x_start+38*0, 342, text[45], color);
        lcd_show_text_32x32(x_start+38*1, 342, text[36], color);
    }
    else if(len==3)
    {
        if(strcmp(str, "东北风"))
        {
            lcd_show_text_32x32(x_start-15+34*0, 342, text[45], color);
            lcd_show_text_32x32(x_start-15+34*1, 342, text[42], color);
        }
        else if(strcmp(str, "西北风"))
        {
            lcd_show_text_32x32(x_start-15+34*0, 342, text[44], color);
            lcd_show_text_32x32(x_start-15+34*1, 342, text[42], color);
        }
        else if(strcmp(str, "西南风"))
        {
            lcd_show_text_32x32(x_start-15+34*0, 342, text[44], color);
            lcd_show_text_32x32(x_start-15+34*1, 342, text[43], color);
        }
        else if(strcmp(str, "东南风"))
        {
            lcd_show_text_32x32(x_start-15+34*0, 342, text[45], color);
            lcd_show_text_32x32(x_start-15+34*1, 342, text[43], color);
        }
        lcd_show_text_32x32(x_start-15+34*2, 342, text[36], color);
    }
    else
    {
        lcd_show_text_32x32(x_start-15+34*0, 342, text[50], color);
        lcd_show_text_32x32(x_start-15+34*1, 342, text[36], color);
        lcd_show_text_32x32(x_start-15+34*2, 342, text[37], color);
    }
}

int wind_powerTotext(const char *str, color_t color, int x_start)
{
    int len = strlen(str);
    int count = 0;
    char num[2] = {0};
    
    for (int i = 0; i < len && count < 2; i++) 
    {
        if (str[i] >= '0' && str[i] <= '9') 
            num[count++] = (str[i] - '0');
    }
    
    lcd_show_text_16x32(x_start+19*0, 388, ch[(num[0])], color);
    lcd_show_text_16x32(x_start+19*1, 388, ch[12], color);
    lcd_show_text_16x32(x_start+19*2, 388, ch[(num[1])], color);
    lcd_show_text_32x32(x_start+19*3, 388, text[46], color);
}

int weatherTopic(const char *str, int x_start, int y_start)
{
    if (strcmp(str, "晴")==0) 
        lcd_draw_pic("./PIC/晴.bmp", x_start, y_start);
    else if(strcmp(str, "多云")==0)
        lcd_draw_pic("./PIC/多云.bmp", x_start, y_start);
    else if(strcmp(str, "阴")==0)
        lcd_draw_pic("./PIC/阴.bmp", x_start, y_start);
    else if (strstr(str, "雨") != NULL) 
        lcd_draw_pic("./PIC/雨.bmp", x_start, y_start);
    else if (strstr(str, "雪") != NULL) 
        lcd_draw_pic("./PIC/雪.bmp", x_start, y_start);
    else if (strstr(str, "雾") != NULL || strstr(str, "霾") != NULL) 
        lcd_draw_pic("./PIC/雾.bmp", x_start, y_start);
}

int temperatureToch(const char *str, color_t color, int x_start)
{
    int len = strlen(str);
    char result[len];
    int i=0;
    for(i=0; i<len; i++)
    {
        result[i] = str[i] - '0';
        lcd_show_text_16x32(x_start+18*i, 206, ch[(result[i])], color);
    }
    lcd_show_text_32x32(x_start+18*i, 206, text[40], color);
}

int weather_24hours(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体
    lcd_draw_pic("./PIC/back.bmp", 50, 50);

    static cJSON *cached_arr = NULL;
    static cJSON *cached_root = NULL;
    static int total_len = 0;
    if(cached_arr == NULL) 
    {
        int sockfd=socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd==-1)
        {
            perror("get sockfd error");
            return -1;
        }
        struct hostent *hostIP=gethostbyname("ali-weather.showapi.com");
        if(hostIP==NULL)
            perror("get host name error");
        struct sockaddr_in hostaddr={.sin_family=AF_INET, .sin_addr.s_addr=((struct in_addr *)(hostIP->h_addr_list[0]))->s_addr, .sin_port=htons(80)};
        connect(sockfd, (struct sockaddr *)&hostaddr, sizeof(hostaddr));

        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        char *str="GET /hour24?areaCode=420100 HTTP/1.1\r\n"
                    "Host: ali-weather.showapi.com\r\n"
                    "Authorization:APPCODE ee99aab01c7146faac383bd0ec246b00\r\n"
                    "\r\n";
        send(sockfd, str, strlen(str), 0);
        char buf[1024*10]={0};
        int total = 0;
        int ret;
        //buf+total就指向了buf[total]
        while((ret = recv(sockfd, buf+total, sizeof(buf)-total-1, 0)) > 0) 
        {
            total += ret;
            printf("已接收 %d 字节 (累计 %d 字节)\n", ret, total);
        }
        buf[total] = '\0';
        // recv(sockfd, buf, sizeof(buf), 0);
        close(sockfd); 
        printf("%s\n", buf); 

        int i=0;
        for(i=0; i<strlen(buf) && buf[i]!='{'; i++);

        cJSON *root=cJSON_Parse(buf+i);
        cJSON *info=cJSON_GetObjectItem(root, "showapi_res_body");
        cached_arr=cJSON_GetObjectItem(info, "hourList");
        total_len=cJSON_GetArraySize(cached_arr);

        cached_root = root;
    }

    int total_pages = (total_len + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;//向上取整算页数
    int start_idx = current_page * ITEMS_PER_PAGE;//该页的起始idx
    int end_idx = (start_idx + ITEMS_PER_PAGE < total_len) ? start_idx + ITEMS_PER_PAGE : total_len;//下页的起始idx
    int display_count = end_idx - start_idx;//一页有几个

    for(int i=0; i<3; i++)
        lcd_show_text_32x32(323+(32+10)*i, 100, text[32+i], font_color);//武汉市
    lcd_show_text_32x32(449, 100, text[31], font_color);//近
    lcd_show_text_16x32(491, 100, ch[2], font_color);//2
    lcd_show_text_16x32(517, 100, ch[4], font_color);//4
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(543+(32+10)*i, 100, text[18+i], font_color);//小时
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(627+(32+10)*i, 100, text[9+i], font_color);//天气

    lcd_show_text_32x32(32, 206, text[39], font_color);//温
    lcd_show_text_32x32(74, 206, text[40], font_color);//度
    lcd_show_text_32x32(32, 274, text[9], font_color);//天
    lcd_show_text_32x32(74, 274, text[10], font_color);//气
    lcd_show_text_32x32(32, 342, text[36], font_color);//风
    lcd_show_text_32x32(74, 342, text[37], font_color);//向
    lcd_show_text_32x32(32, 388, text[36], font_color);//风
    lcd_show_text_32x32(74, 388, text[38], font_color);//力
    lcd_show_text_32x32(32, 434, text[19], font_color);//时
    lcd_show_text_32x32(74, 434, text[35], font_color);//间
    printf("时间\t\t风向\t\t风力\t\t天气\t温度\n");

    cJSON *p=NULL;
    char *s=NULL;
    for(int i=0; i<display_count; i++)
    {
        int idx = start_idx + i;
        p=cJSON_GetArrayItem(cached_arr, idx);
        if(p == NULL) 
        {
            printf("第 %d 项为空，跳过\n", idx);
            continue;
        }

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "time"));
        timeToch(s, font_color, 128+(80+50)*i);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "wind_direction"));
        wind_directionTotext(s, font_color, 137+(80+50)*i);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "wind_power"));
        wind_powerTotext(s, font_color, 127+(80+50)*i);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "weather"));
        weatherTopic(s, 132+(80+50)*i, 252);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "temperature"));
        temperatureToch(s, font_color, 144+(80+50)*i);
        printf("%s°\n", s);
    }

    color_t gray={204, 204, 204}; 
    if(current_page > 0) 
    {
        lcd_color_block_drawing(32, 497, 150, 68, gray);
        lcd_show_text_32x32(48, 513, text[47], font_color);//上
        lcd_show_text_32x32(89, 513, text[1], font_color);//一
        lcd_show_text_32x32(130, 513, text[49], font_color);//页
    }
    else
        lcd_color_block_drawing(32, 497, 150, 68, color);

    if(current_page < total_pages - 1)
    {
        lcd_color_block_drawing(842, 497, 150, 68, gray);
        lcd_show_text_32x32(858, 513, text[48], font_color);//下
        lcd_show_text_32x32(899, 513, text[1], font_color);//一
        lcd_show_text_32x32(940, 513, text[49], font_color);//页
    }
    else
        lcd_color_block_drawing(842, 497, 150, 68, color);

    while(1)
    {
        int x = -1, y = -1;
        ts_get_axes(&x, &y);
        
        if(ts_click_area(x, y, 50, 50, 80, 80)) 
        {
            current_page = 0;
            pop_state();
            return 0;
        }
        else if(ts_click_area(x, y, 32, 497, 150, 68) && current_page > 0) 
        {
            current_page--;
            weather_24hours(color);
            return 0;
        }
        else if(ts_click_area(x, y, 842, 497, 150, 68) && current_page < total_pages - 1) 
        {
            current_page++;
            weather_24hours(color);
            return 0;
        }
    }

    return 0;
}

int weather_7days(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体
    lcd_draw_pic("./PIC/back.bmp", 50, 50);

    int sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd==-1)
    {
        perror("get sockfd error");
        return -1;
    }
    struct hostent *hostIP=gethostbyname("ali-weather.showapi.com");
    if(hostIP==NULL)
        perror("get host name error");
    struct sockaddr_in hostaddr={.sin_family=AF_INET, .sin_addr.s_addr=((struct in_addr *)(hostIP->h_addr_list[0]))->s_addr, .sin_port=htons(80)};
    connect(sockfd, (struct sockaddr *)&hostaddr, sizeof(hostaddr));

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    char *str="GET /day15?areaCode=420100 HTTP/1.1\r\n"
                "Host: ali-weather.showapi.com\r\n"
                "Authorization:APPCODE ee99aab01c7146faac383bd0ec246b00\r\n"
                "\r\n";
    send(sockfd, str, strlen(str), 0);
    char buf[1024*10]={0};
    int total = 0;
    int ret;
    while((ret = recv(sockfd, buf + total, sizeof(buf) - total - 1, 0)) > 0) {
        total += ret;
        printf("已接收 %d 字节 (累计 %d 字节)\n", ret, total);
    }
    buf[total] = '\0';
    // recv(sockfd, buf, sizeof(buf), 0);
    close(sockfd); 
    printf("%s\n", buf); 

    int i=0;
    for(i=0; i<strlen(buf) && buf[i]!='{'; i++);

    cJSON *root=cJSON_Parse(buf+i);
    cJSON *info=cJSON_GetObjectItem(root, "showapi_res_body");
    cJSON *arr=cJSON_GetObjectItem(info, "dayList");
    int len=cJSON_GetArraySize(arr);

    for(int i=0; i<3; i++)
        lcd_show_text_32x32(357+(32+10)*i, 100, text[32+i], font_color);//武汉市
    lcd_show_text_32x32(483, 100, text[31], font_color);//近
    lcd_show_text_16x32(525, 100, ch[7], font_color);//7
    lcd_show_text_32x32(551, 100, text[9], font_color);//天
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(593+(32+10)*i, 100, text[9+i], font_color);//天气

    lcd_show_text_32x32(32, 206, text[39], font_color);//温
    lcd_show_text_32x32(74, 206, text[40], font_color);//度
    lcd_show_text_32x32(32, 274, text[9], font_color);//天
    lcd_show_text_32x32(74, 274, text[10], font_color);//气
    lcd_show_text_32x32(32, 342, text[36], font_color);//风
    lcd_show_text_32x32(74, 342, text[37], font_color);//向
    lcd_show_text_32x32(32, 388, text[36], font_color);//风
    lcd_show_text_32x32(74, 388, text[38], font_color);//力
    lcd_show_text_32x32(32, 434, text[7], font_color);//日
    lcd_show_text_32x32(74, 434, text[51], font_color);//期
    printf("日期\t\t风向\t\t风力\t\t天气\t温度\n");

    int display_count = (len < 7) ? len : 7;
    cJSON *p=NULL;
    char *s=NULL;
    for(int i=0; i<display_count; i++)
    {
        p=cJSON_GetArrayItem(arr, i);
        if(p == NULL) 
        {
            printf("第 %d 项为空，跳过\n", i);
            continue;
        }

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "daytime"));
        dataToch(s, font_color, 128+(80+50)*i);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "day_wind_direction"));
        wind_directionTotext(s, font_color, 137+(80+50)*i);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "day_wind_power"));
        wind_powerTotext(s, font_color, 127+(80+50)*i);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "day_weather"));
        weatherTopic(s, 132+(80+50)*i, 252);
        printf("%s\t", s);

        s=cJSON_GetStringValue(cJSON_GetObjectItem(p, "day_air_temperature"));
        temperatureToch(s, font_color, 145+(80+50)*i);
        printf("%s°\n", s);
    }

    while(1)
    {
        int x = -1, y = -1;
        ts_get_axes(&x, &y);        
        if(ts_click_area(x, y, 50, 50, 80, 80)) 
        {
            current_page = 0;
            pop_state();
            return 0;
        }
    }

    return 0;
}

int weather(color_t color)
{
    lcd_color_block_drawing(0, 0, 1024, 600, color);
    color_t font_color={255, 255, 255};//白色字体

    lcd_draw_pic("./PIC/back.bmp", 50, 50);

    lcd_draw_pic("./PIC/24hours.bmp", 212, 180);
    for(int i=0; i<3; i++)
        lcd_show_text_32x32(-62+212+(36+5)*i, 390, text[29+i], font_color);//查看近
    lcd_show_text_16x32(273, 390, ch[2], font_color);//2
    lcd_show_text_16x32(294, 390, ch[4], font_color);//4
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(315+(36+5)*i, 390, text[18+i], font_color);//小时
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(397+(36+5)*i, 390, text[9+i], font_color);//天气

    
    lcd_draw_pic("./PIC/7days.bmp", 612, 180);
    for(int i=0; i<3; i++)
        lcd_show_text_32x32(-31+612+(36+5)*i, 390, text[29+i], font_color);//查看近
    lcd_show_text_16x32(704, 390, ch[7], font_color);//7
    lcd_show_text_32x32(725, 390, text[9], font_color);//天
    for(int i=0; i<2; i++)
        lcd_show_text_32x32(766+(36+5)*i, 390, text[9+i], font_color);//天气

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
            if(ts_click_area(x, y, 212, 180, 200, 200))
            {
                printf("点击24小时预报\n");
                weather_24hours(color);
            }
            else if(ts_click_area(x, y, 612, 150, 200, 200))
            {
                printf("点击7天预报\n");
                weather_7days(color);
            }
            else
                continue;

            lcd_color_block_drawing(0, 0, 1024, 600, color);
            lcd_draw_pic("./PIC/back.bmp", 50, 50);

            lcd_draw_pic("./PIC/24hours.bmp", 212, 180);
            for(int i=0; i<3; i++)
                lcd_show_text_32x32(-62+212+(36+5)*i, 390, text[29+i], font_color);//查看近
            lcd_show_text_16x32(273, 390, ch[2], font_color);//2
            lcd_show_text_16x32(294, 390, ch[4], font_color);//4
            for(int i=0; i<2; i++)
                lcd_show_text_32x32(315+(36+5)*i, 390, text[18+i], font_color);//小时
            for(int i=0; i<2; i++)
                lcd_show_text_32x32(397+(36+5)*i, 390, text[9+i], font_color);//天气

            
            lcd_draw_pic("./PIC/7days.bmp", 612, 180);
            for(int i=0; i<3; i++)
                lcd_show_text_32x32(-31+612+(36+5)*i, 390, text[29+i], font_color);//查看近
            lcd_show_text_16x32(704, 390, ch[7], font_color);//7
            lcd_show_text_32x32(725, 390, text[9], font_color);//天
            for(int i=0; i<2; i++)
                lcd_show_text_32x32(766+(36+5)*i, 390, text[9+i], font_color);//天气
        }
    }
}
