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
#include "cgi_base.h"

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
