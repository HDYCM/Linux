#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <sys/select.h>

#include "keyboard.h"
#include "def.h"

//绘制颜色
void draw_element(int x, int y, int c){
    x *= 2;
    x++;
    y++;
    printf("\033[%d;%dH\033[?25l", y, x);   //确定坐标
    printf("\033[3%dm\033[4%dm", c, c);     //设置[]颜色及填充的颜色一致
    printf("[]");
    fflush(stdout);
    printf("\033[0m");
}

//绘制形状
void draw_shape(int x, int y, struct shape p, int c){
    int i = 0;
    for(; i < 5; ++i){
        int j = 0;
        for(; j < 5; ++j){
            if(p.s[i][j] != 0){
                draw_element(x + j, y + i, c);
            }
        }
    }
}

//绘制游戏区域
void draw_back(void){
    int i = 0;
    for(; i < H; ++i){
        int j = 0;
        for(; j < W; ++j){
            if(background[i][j] == 0){
                draw_element(j, i, BC);
            }else{
                draw_element(j, i, FC);
            }
        }
    }
}

//边界不能移动
int can_move(int x, int y, struct shape p){
    int i = 0;
    for(; i < 5; ++i){
        int j = 0;
        for(; j < 5; ++j){
            if(p.s[i][j] == 0){
                continue;
            }
            if(x + j >= W){
                return 0;
            }
            if(x + j < 0){
                return 0;
            }
            if(y + i >= H){
                return 0;
            }
            if(background[y + i][x + j] != 0){
                return 0;
            }
        }
    }
    return 1;
}

void set_back(struct data* p, struct shape shp){
    int i = 0;
    for(; i < 5; ++i){
        int j = 0;
        for(; j < 5; ++j){
            if(shp.s[i][j] != 0){
                background[p->y + i][p->x + j] = FC;
            }
        }
    }
}

//消行
void clean_line(){
    int i = 0;
    for(; i < 5; ++i){
        int t = 0;
        int j = 0;
        for(j = 0; j < 5; ++j){
            if(background[i][j] != 0){
                ++t;
            }
        }
        if(t == W){
            int k = i;
            for(; k > 0; k--){
                memcpy(background[k], background[k - 1], sizeof background[k]);
            }
        }
    }
}

//显示下一个图形
void show_next(){
    int i = 0;
    for(; i < 5; ++i){
        int j = 0;
        for(; j < 5; ++j){
            if(shape_arr[next].s[i][j] == 0){
                draw_element(W + 1 + j, 1 + i, BC);
            }else{
                draw_element(W + 1 + j, 1 + i, FC);
            }
        }
    }
}

//游戏结束
int is_over(){
    int i = 0;
    for(; i < W; ++i){
        if(background[1][i] != 0){
            return 1;
        }
    }
    return 0;
}

//定时下落
void tetris_timer(struct data* p){
    if(can_move(p->x, p->y + 1, shape_arr[cur])){
        draw_shape(p->x, p->y, shape_arr[cur], BC);
        p->y++;
    }else{
        set_back(p, shape_arr[cur]);
        clean_line();
        if(is_over() == 1){
            printf("\033[?25h");
            recover_keyboard();
            exit(0);
        }
        draw_back();
        cur = next;
        next = rand() % 7;
        show_next();
        p->x = 2;
        p->y = 0;
    }
    draw_shape(p->x, p->y, shape_arr[cur], FC);
}

//旋转90°
struct shape turn_90(struct shape p){
    struct shape t;
    int i = 0;
    for(; i < 5; ++i){
        int j = 0;
        for(; j < 5; ++j){
            t.s[i][j] = p.s[4 - j][i];
        }
    }
    return t;
}

int tetris(struct data* p){
    int ret = 0;
    int c = get_key();
    if(is_up(c)){
        draw_shape(p->x, p->y, shape_arr[cur], BC);
        shape_arr[0] = turn_90(shape_arr[cur]);
        if(can_move(p->x, p->y, shape_arr[cur])){
            shape_arr[cur] = turn_90(shape_arr[cur]);
            shape_arr[cur] = turn_90(shape_arr[cur]);
            shape_arr[cur] = turn_90(shape_arr[cur]);
        }
        draw_shape(p->x, p->y, shape_arr[cur], FC);
    }
    if(is_down(c)){
        draw_shape(p->x, p->y, shape_arr[cur], BC);
        if(can_move(p->x, p->y + 1, shape_arr[cur])){
            p->y++;
        }
        draw_shape(p->x, p->y, shape_arr[cur], FC);
    }
    if(is_left(c)){
        draw_shape(p->x, p->y, shape_arr[cur], BC);
        if(can_move(p->x - 1, p->y, shape_arr[cur])){
            p->x--;
        }
        draw_shape(p->x, p->y, shape_arr[cur], FC);
    }
    if(is_right(c)){
        draw_shape(p->x, p->y, shape_arr[cur], BC);
        if(can_move(p->x + 1, p->y, shape_arr[cur])){
            p->x++;
        }
        draw_shape(p->x, p->y, shape_arr[cur], FC);

    }
    if(is_esc(c)){
        ret = 1;
    }
    return ret;
}

int main(){
    init_keyboard();
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    struct itimerspec is;
    memset(&is, 0x00, sizeof is);
    is.it_value.tv_nsec = 1;
    is.it_interval.tv_sec = 1;
    timerfd_settime(tfd, 0, &is, NULL);
    
    srand(getpid());
    draw_back();
    next = rand() % 7;
    show_next();

    fd_set rset;
    int kfd = fileno(stdin);
    int maxfd = tfd > kfd ? tfd : kfd;
    while(1){
        FD_ZERO(&rset);
        FD_SET(tfd, &rset);
        FD_SET(kfd, &rset);
        select(maxfd + 1, &rset, NULL, NULL, NULL);
        if(FD_ISSET(tfd, &rset)){
            uint64_t clk;
            read(tfd, &clk, sizeof clk);
            tetris_timer(&t);
        }
        if(FD_ISSET(kfd, &rset)){
            if(tetris(&t) == 1){
                break;                                
            }
        }
    }
    printf("\033[?25h");
    recover_keyboard();
    return 0;
}
