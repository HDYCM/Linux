#include "common.h"

static int MsgCommon(int flags){
    //若两个文件的ftok参数相同，则key一定相同
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0){
        perror("ftok error");
        return -1;
    }
    int msgid = msgget(key, flags);
    if(msgid < 0){
        perror("msgget error");
        return -1;
    }
    return msgid;
}

int MsgCreate(){
    //IP_CREAT:不存在就创建，存在就打开
    //IPC_EXCL:搭配IP_CREAT使用，若消息队列存在，就打开失败
    //0666:指定权限
    return MsgCommon(IPC_CREAT | IPC_EXCL | 0666);
}

int MsgOpen(){
    return MsgCommon(IPC_CREAT);
}

int MsgDestory(int msgid){
   int ret = msgctl(msgid, IPC_RMID, NULL);
   if(ret < 0){
       perror("msgctl error");
       return -1;
   }
   return 0;
}

int MsgSend(int msgid, int type, char* buf, size_t size){
    Msgbuf msgbuf;
    msgbuf.mtype = type;
    //按照这种方式，mtext一定留有空间保存\0
    if(size >= sizeof(msgbuf.mtext)/sizeof(msgbuf.mtext[0])){
        printf("input buf size is too long\n");
        return -1;
    }
    strcpy(msgbuf.mtext, buf);
    int ret = msgsnd(msgid, &msgbuf, sizeof(msgbuf.mtext), 0);
    if(ret < 0){
        perror("msgsnd error");
        return -1;
    }
    return 0;
}

int MsgRecv(int msgid, int type, char* buf, size_t max_size){
    Msgbuf msgbuf;
    int ret = msgrcv(msgid, &msgbuf, sizeof(msgbuf.mtext), type, 0);
    if(ret < 0){
        printf("msgrcv error");
        return -1;
    }
    //此处我们的判定依据是根据 MsgSend 函数类的
    //由于 MsgSend 函数能够保证一定在 mtext 中存在 \0
    //所以此处我们不必给输出缓冲区再预留一个 \0 的空间
    if(max_size < sizeof(msgbuf.mtext)){
        printf("output buf size too small");
        return -1;
    } 
    strcpy(buf, msgbuf.mtext);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
//以下是测试代码
/////////////////////////////////////////////////////////////////////////////
#if 0
void TestCreate(){
    int msgid = MsgCreate();
    printf("msgid = %d\n", msgid);
}

void TestOpen(){
    int msgid = MsgOpen();
    printf("msgid = %d\n", msgid);
}

void TestDestory(){
    int msgid = MsgOpen();
    int ret = MsgDestory(msgid);
    printf("ret = %d\n", ret);
}

void TestSend(){
    int msgid = MsgCreate();
    char* msg = "hehe\n";
    int ret = MsgSend(msgid, 1, msg, strlen(msg));     //此处用strlen的好处是只取内容，而不用将整个数组取出
    printf("ret = %d\n", ret);
}

void TestRecv(){
    int msgid = MsgOpen();
    char buf[1024] = {0};
    int ret = MsgRecv(msgid, 1, buf, sizeof(buf));
    printf("ret = %d\n", ret);
    printf("buf = %s\n", buf);
}

int main(){
    //TestCreate();
    //TestOpen();
    //TestDestory();
    //TestSend();
    TestRecv();
    return 0;
}
#endif
