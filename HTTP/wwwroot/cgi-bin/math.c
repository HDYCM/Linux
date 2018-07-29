////////////////////////////////////////////////////////////////////////////
//此处实现一个简易的计算加法的 CGI 程序
//与HTTP服务器项目无关
//仅仅用于辅助测试
///////////////////////////////////////////////////////////////////////////
//GET:
//   query_string:a=10&b=20
//   服务器返回30
//POST:
//   body:a=10&b=20
//   服务器返回30
//http://192.168.xx.xx:9090/cgi-bin/math?a=10&b=20
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int GetQueryString(char output[]){
    //1.先从环境变量中获取到方法
    char* method = getenv("REQUEST_METHOD");
    if(method == NULL){
        fprintf(stderr, "REQUEST_METHOD failed\n");
        return -1;
    }
    //2.如果是 GET 方法，就直接从环境变量中获取 QUERY_STRING
    if(strcasecmp(method, "GET") == 0){
        char* query_string = getenv("QUERY_STRING");
        if(query_string == NULL){
            fprintf(stderr, "QUERY_STRING failed\n");
            return -1;
        }
        strcpy(output, query_string);
    }
    else{
        //3.如果是 POST 方法，先通过环境变量获取到 CONTENT_LENGTH，再从标准输入中读取 body
        char* content_length_str = getenv("CONTENT_LENGTH");
        if(content_length_str == NULL){
            fprintf(stderr, "CONTENT_LENGTH failed\n");
            return -1;
        }
        int content_length = atoi(content_length_str);
        int i = 0;
        //表示当前已经在 output 中写了多少个字符了
        for(; i < content_length; ++i){
            read(0, output + i, 1);
        }
        output[content_length] = '\0';
    }
    return 0;
}

int main(){
    //1.先获取到参数(方法, query_string, body)
    char buf[1024 * 4] = {0};
    int ret = GetQueryString(buf);
    if(ret < 0){
        fprintf(stderr, "GetQueryString failed\n");
        return 1;
    }
    //2.解析 buf 中的参数，具体的解析规则，就和业务相关了
    //  解析的时候需要按照客户端构造请求的 key 来进行解析
    //  此处 key 叫做 a,b
    int a, b;
    sscanf(buf, "a=%d&b=%d", &a, &b);        //sscanf 的容错能力较差
    //3.根据业务的具体要求，完成计算
    int c = a + b;
    //4.把结果构造成页面返回给浏览器
    printf("<h1>ret=%d</h1>", c);
    return 0;
}
