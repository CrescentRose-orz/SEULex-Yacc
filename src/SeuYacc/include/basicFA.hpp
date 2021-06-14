#ifndef BASICFA
#define BASICFA
#include"CONSTANT.h"
#include"visualFA.hpp"
#include<bits/stdc++.h>
#include<shared_mutex>
#include"Logger.h"
using namespace std;

template<typename T>
class basicFA{
public:
    Logger logger;
    _mutex_type Wrlock;
    int tail = 0;
    vector<T> pool;
    basicFA(){}
    basicFA(Logger &log):logger(log){}
    T& operator [](int i){
        return pool[i];
    }
    virtual int add()=0;
    virtual int add(T)=0;
    virtual int head()=0;
    #ifdef VISUAL       
    visualLR vLR;
    #endif
};
#endif