#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main(){
    // 使用 mysql api 完成查找动作
    // 查找 TestDB.TestTable 里面的所有数据
    // 正常情况下，第一步应该先根据 CGI 的规则获取到用户定义的相关参数，
    // 但是由于此处只是把表中的所有数据都查出来，暂时就先不管用户输入
    
    // 1.初始化 mysql 的连接句柄
    MYSQL* connect_fd = mysql_init(NULL);
    // 2.建立连接
    //   参数含义：
    //    a) mysql 连接句柄
    //    b) mysql 服务器的 ip 地址
    //    c) 用户名
    //    d) 密码
    //    e) 要访问的数据库名
    //    f) 端口号
    //    g) unix_socket(此处不管，直接填成NULL)
    //    h) 标志位(当前也不管，直接填成0)
    if(mysql_real_connect(connect_fd, "127.0.0.1", "root", "", "TestDB", 3306, NULL, 0) == NULL){
        fprintf(stderr, "mysql_real_connect failed\n");
        return 1;
    }
    fprintf(stderr, "mysql_real_connect OK\n");
    // 3.构造 sql 语句(字符串拼接)
    const char* sql = "select * from TestDB";
    // 4.把 sql 语句发送到服务器上
    int ret = mysql_query(connect_fd, sql);
    if(ret < 0){
        fprintf(stderr, "mysql_query failed\n");
        return 1;
    }
    fprintf(stderr, "mysql_query OK\n");
    // 5.遍历结果集合(表结构)
    //  先按行遍历，每次取一行，再依次的把所有的列也取出来
    MYSQL_RES* result = mysql_store_result(connect_fd);
    if(result == NULL){
        fprintf(stderr, "mysql_store_result failed\n");
        return 1;
    }
    printf("<html>");
    //  a) 先获取到有几行，以及几列
    int rows = mysql_num_rows(result);
    int fields = mysql_num_fields(result);
    //  b)把表头中的这几个字段都获取到
    //    调用一次 mysql_fetch_field 就获取到一个表头
    MYSQL_FIELD* f = mysql_fetch_field(result);
    while(f != NULL){
        //此处的name就表示这列的具体的列名
        printf("%s\t", f->name);
        f = mysql_fetch_field(result);
    }
    printf("<br>");
    //  c)把表内容获取出来
    int i = 0;
    for(; i < rows; ++i){
        MYSQL_ROW row = mysql_fetch_row(result);
        int j = 0;
        for(; j < fields; ++j){
            printf("%s\t", row[j]);
        }
        printf("<br>");
    }
    printf("</html>");
    // 6.断开连接(此处一定不要忘记)
    mysql_close(connect_fd);
    return 0;
}
