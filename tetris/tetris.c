#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <sys/select.h>

#include "def.h"

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

void tetris_timer(struct data* p){
    draw_shape(p->x, p->y, shape_arr[0], BC);
    p->y++;
    draw_shape(p->x, p->y, shape_arr[0], FC);
}

int main(){
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    struct itimerspec is;
    memset(&is, 0x00, sizeof is);
    is.it_value.tv_nsec = 1;
    is.it_interval.tv_sec = 1;
    timerfd_settime(tfd, 0, &is, NULL);

    fd_set rset;
    int maxfd = tfd + 1;
    while(1){
        FD_ZERO(&rset);
        FD_SET(tfd, &rset);
        select(maxfd, &rset, NULL, NULL, NULL);
        if(FD_ISSET(tfd, &rset)){
            uint64_t clk;
            read(tfd, &clk, sizeof clk);
            tetris_timer(&t);
        }
    }
    printf("\033[?25h");
    return 0;
}
