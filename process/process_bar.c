#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    char buf[102] = {0};
    int i = 0;
    int j = 0;
    //设置颜色
    int color[] = {1, 2, 3, 4, 5, 6, 7};
    const char* lable = "|/-\\";
    for(; i < 101; ++i){
       buf[i] = '#';
       printf("\033[3%dm[%-101s]\033[0m\033[33m[%d%%]\033[0m[%c]\r", color[j], buf, i, lable[i % 4]);
      //每25%换一次颜色
      if(i % 25 == 0){
          ++j;
      }
      fflush(stdout);
      usleep(50000);
    }
    printf("\n");
    return 0;
}
