//////////////////////////////////////////////////////////////////////
//该文件包含了整个HTTP服务器的实现
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

#define SIZE (1024 * 4)

typedef struct HttpRequest{
    char first_line[SIZE];
    char* method;
    char* url;
    char* url_path;
    char* query_string;
    int content_length;
}HttpRequest;



//url 形如：/index.html?a=10&b=20
//          http://www.baidu.com/index.html?a=10&b=20(暂时不考虑)
int ParseUrl(char url[], char** p_url_path, char** p_query_string){
    *p_url_path = url;
    //查找？所在的位置
    char* p = url;
    for(; *p != '\0'; ++p){
        if(*p == '?'){
            //找到了？，此时就把？替换成 \0
            *p = '\0';
            *p_query_string = p + 1;
            return 0;
        }
    }
    *p_query_string = NULL;
    return 0;
}

int ParseHeader(int new_sock, int* content_length){
    char buf[SIZE] = 0;
    while(1){
        ssize_t read_size = ReadLine(new_sock, buf, sizeof(buf) - 1);
        if(read_size <= 0){
            return -1;
        }
        if(strncmp(buf, '\n') == 0){
            return 0;
        }
        //Content-Length: 100\n
        const char* key = "Content-Length: ";
        if(strncmp(buf, key, strlen(key) == 0){
            *content_length = atoi(buf + strlen(key));    //atoi发现非数字字符会自动结束
            //此处不能写break，否则就是粘包问题
        } 
    }
    return 0;
}

void Handler404(int new_sock){
    const char* first_line = "HTTP/1.1 404 Not Found\n";
    //此处的代码可以不加 header
    //Content-Type 可以让浏览器自动识别
    //Content-Length 可以通过关闭socket的方式告知浏览器已经读完
    //body 部分其实就是 HTML 页面
    const char* blank_line = "\n";
    const char* body = "<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"></head>"
                "<h1>404!!!您的页面被喵星人吃掉了!!!</h1>";
    send(new_sock, first_line, strlen(first_line), 0);
    send(new_sock, blank_line, strlen(blank_line), 0);
    send(new_sock, body, strlen(body), 0);
}

int IsDir(const char* file_path){
    struct stat st;
    int ret = stat(file_path, &st);
    if(ret < 0){
        //此处不是目录
        return 0;
    }
    if(S_ISDIR(st.st_mode)){
        return 1;
    }
}

void HandlerFilePath(const char* url_path, char file_path){
    //url_path 是以 / 开头的，所以不需要 wwwroot 之后显示指明 /
    sprintf(file_path, "./wwwroot%s", url_path);
    //如果 url_path 指向的是目录，就在目录后面拼接上 index.html 作为默认访问文件
    //如何识别 url_path 指向的文件到底是文件还是目录呢？
    if(file_path[strlen(file_path) - 1] == '/'){
        //  a)url_path 以 / 结尾，如：/image/
        strcat(file_path, "index.html");
    }
    else{
        //  b)url_path 没有以 / 结尾，此时需要根据文件属性来判断是否是目录
        if(IsDir(file_path)){
            strcat(file_path, "/index.html");
        }
    }
}

ssize_t GetFileSize(const char* file_path){
    struct stat st;
    int ret = stat(file_path, &st);
    if(ret < 0){
        return 0;
    }
    return st.st_size;
}

int WriteStaticFile(int new_sock, const char* file_path){
    //1.打开文件，如果打开失败，返回404
    int fd = open(file_path, O_RDONLY);
    if(fd < 0){
        perror("open");
        return 404;
    }
    //2.构造HTTP响应报文
    const char* first_line = "HTTP/1.1 200 OK\n";
    send(new_sock, first_line, strlen(first_line), 0);
    //此处如果从一个更严谨的角度来说，还是应该加上header,此处没有写Content-Type 是因为浏览器可以自动识别
    const char* blank_line = "\n";
    send(new_sock, blank_line, strlen(blank_line), 0);
    //3.读文件内容并且写到 socket 中
    //char c = '\0';
    //while(read(new_sock, &c, 1) > 0){
    //    send(new_sock, &c, 1);
    //}
    //此处使用更高效的sendfile()来完成文件传输操作
    ssize_t file_size = GetFileSize(file_path);
    sendfile(new_sock, fd, NULL, file_size);
    //4.关闭文件
    close(fd);
    return 200;
}

int HandlerStaticFile(int new_sock, const HttpRequest* req){
    //1.根据 url_path 获取文件的真实路径
    //例如：此时的 HTTP 服务器的根目录叫做 ./wwwroot
    //此时有一个文件 ./wwwroot/image/101.jpg
    //在url 中写path就叫做 /image/101.jpg
    char file_path[SIZE] = {0};
    //根据下面的函数把 /image/101.jpg 替换成了
    //磁盘上的 ./wwwroot/image/101.jpg
    HandlerFilePath(req->url_path, file_path);
    //2.打开文件，读取文件内容，把文件内容写到 socket 中
    int err_code = WriteStaticFile(new_sock, file_path);
    return err_code;
}

int HandlerCGI(){
    return 404;
}

//完成具体的请求处理过程
void HandlerRequest(int64_t new_sock){
    int err_code = 200;
    HttpRequest req;
    memset(&req, 0, sizeof(req));
    //1.解析请求
    // a)从socket中读取首行
    if(ReadLine(new_sock, req.first_line, sizeof(req.first_line) - 1) < 0){
        //错误处理，此处一旦触发错误处理逻辑，
        //此处简略    返回一个404数据包
        //正常来说，
        err_code = 404;
        goto END;
    }
    printf("first_line: %s\n", req.first_line);
    // b)解析首行，获取到 url 和 method
    if(ParseFirstLine(req.first_line, &req.method, &req.url) < 0){
        //错误处理
        err_code = 404;
        goto END;
    }
    // c)解析url，获取到 url_path 和 query_string
    if(ParseUrl(req.url, &req.url_path, &req.query_string) < 0){
        //错误处理
        err_code = 404;
        goto END;
    }
    // d)解析header，丢弃大部分 header ，只保留 Content-Length
    if(ParseHeader(new_sock, &req.content_length)){
        //错误处理
        err_code = 404;
        goto END;
    }
    //2.根据请求计算响应并写回到客户端
    //GET, Get, gET 多种形式
    if(strcasecmp(req.method, "GET") == 0 && req.query_string == NULL){
        // a)处理静态页面
        HandlerStaticFile(new_sock, &req);
    }
    else if(strcasecmp(req.method, "GET") == 0 && req.query_string != NULL){
        // b)处理动态页面
        HandlerCGI();
    }
    else if(strcasecmp(req.method, "POST") == 0){
        // b)处理动态页面
        HandlerCGI();
    }
    else{
        //错误处理
    }
END:
    //收尾工作, 主动关闭 socket，会进入
    if(err_code == 404){
        Handler404(new_sock);
    }
    close(new_sock);
}

void* ThreadEntry(void* arg){
    int64_t new_sock = (int64_t)arg;
    HandlerRequest(new_sock);
    return NULL;
}

void HttpServerStart(const char* ip, short port){
    //1.基本的初始化，基于TCP
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0){
        perror("socket");
        return;
    }

    //设置一个选项 REUSEADDR
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    int ret = bind(listen_sock, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return;
    }
    ret = listen(listen_sock, 5);
    if(ret < 0){
        perror("listen");
        return;
    }
    printf("HttpServer start OK\n");
    //2.进入事件循环
    while(1){
        //此处实现一个多线程版本的服务器
        //每个请求都创建一个新的线程
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int new_sock = accept(listen_sock, (sockaddr*)&peer, &len);
        if(new_sock < 0){
            perror("accept");
            continue;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, ThreadEntry, (void*)new_sock);
        pthread_detach(tid);
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage ./http_server [ip] [port]\n");
        return 1;
    }
    HttpServerStart(argv[1], atoi(argv[2]));
    return 0;
}
