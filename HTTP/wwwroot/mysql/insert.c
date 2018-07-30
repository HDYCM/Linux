/////////////////////////////////////////////////////////////////
// 通过用户提交的数据网数据库中插入一条记录
// id, name
/////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "cgi_base.h"

int main(){
    // 1.获取 query_string 并解析
    char buf[1024 * 4] = {0};
    if(GetQueryString(buf) < 0){
        fprintf(stderr, "GetQueryString failed\n");
        return 1;
    }
    int id = 0;
    char name[100] = {0};
    sscanf(buf, "id=%d&name=%s", &id, &name);
    // 2.初始化 mysql 连接句柄
    MYSQL* connect_fd = mysql_init(NULL);
    // 3.建立连接
    if(mysql_real_connect(connect_fd, "127.0.0.1", "root", "", "TestDB", 3306, NULL, 0) == NULL){
        fprintf(stderr, "mysql_real_connect failed\n");
    }
    fprintf(stderr, "mysql_real_connect\n");
    // 4.构造 sql 语句
    char sql[1024 * 4] = {0};
    sprintf(sql, "insert into TestTable values(%d, %s)", id, name);
    // 5.发送 sql 语句
    int ret = mysql_query(connect_fd, sql);
    if(ret < 0){
        fprintf(stderr, "mysql_query failed\n");
    }
    printf("<html>");
    printf("insert success!\n");
    printf("</html>");
    // 6.断开连接
    mysql_close(connect_fd);
    return 0;
}

