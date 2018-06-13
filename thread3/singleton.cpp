#if 1
//饿汉模式
template <typename T>
class Singleton{
public:
    static T* GetInstance(){
        return &inst_;
    }
private:
    static T inst_;
};
Singleton<int>::GetInstance();
#endif


#if 0
//懒汉模式
//类模板
#include <stdlib.h>
template <typename T>
class Singleton{
public:
    static T* GetInstance(){
        //为了线程安全
        if(inst_ == NULL){
            lock();
            if(inst_ == NULL){
                inst_ = new T();
            }
            unlock();
        }
        return inst_;
    }
private:
    static volatile T* inst_;
};
//inst_ 初始化为空
Singleton<int>::GetInstance();
#endif
