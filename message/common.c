#include "common.h"

static int MsgCommon(int flags){
    ket_t key = ftok(PATHNAME, PROJ_ID);
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
    return MsgCommon(IPC_CREAT | IPC_EXCL | 0666);
}

int MsgOpen(){
    return MsgCommon(IPC_CREAT);
}
