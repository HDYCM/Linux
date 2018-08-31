//单例模式是设计模式中的一种
//最常用，也是最常被考的
//某个类，在整个程序中只能有一个实例（对象）
#if 1
//饿汉模式
//inst_对象在程序启动就构造了
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
//第一次使用 Instance 的时候才去构造对象
//类模板
#include <stdlib.h>
template <typename T>
class Singleton{
public:
    static T* GetInstance(){
        //为了线程安全,只有第一次调用时考虑线程安全问题
        if(inst_ == NULL){           // 避免造成死锁
            lock();
            if(inst_ == NULL){
                inst_ = new T();
            }
            unlock();
        }
        return inst_;
    }
private:
    static volatile T* inst_;         //避免编译器优化带来问题
};
//inst_ 初始化为空
Singleton<int>::GetInstance();
#endif
