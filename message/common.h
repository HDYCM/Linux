#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define PATHNAME "."
#define PROJ_ID 1

#define SERVER_TYPE 1
#define CLIENT_TYPE 2

typedef struct Msgbuf{
    long mtype;
    char mtext[1024];
}Msgbuf;

//如果消息队列不存在，就创建它
//如果消息队列存在，就创建失败
int MsgCreate();

//如果消息队列不存在就打开失败
//如果消息队列存在就打开
int MsgOpen();

//销毁消息队列
int MsgDestory(int msgid);

//往消息队列中发送数据
int MsgSend(int msgid, int type, char* buf, size_t size);

//从消息队列中接收数据
int MsgRecv(int msgid, int type, char* buf, size_t max_size);
