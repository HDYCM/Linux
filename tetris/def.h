#pragma once

#define FC 5
#define BC 8

#define W 10
#define H 20

struct shape{
    int s[5][5];
};

struct data{
    int x;
    int y;
};
 
extern struct shape shape_arr[7];
extern struct data t;
extern int background[H][W];
extern int cur;
extern int next;
