#include"CONSTANT.h"
#include"visualFA.hpp"
#include<bits/stdc++.h>
#include"Logger.h"
using namespace std;

template<typename T>
class basicFA{
public:
    Logger logger;
    shared_mutex Wrlock;
    int tail = 0;
    vector<T> pool;
    basicFA(){}
    basicFA(Logger &log):logger(log){}
    T& operator [](int i){
        return pool[i];
    }
    virtual int add();
    virtual int add(T node);
    virtual int head();
    #ifdef VISUAL       
    visualFA<int> vFA;
    #endif
};
