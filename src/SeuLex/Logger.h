#ifndef LOGGER
#define LOGGER
#include<bits/stdc++.h>
using namespace std;


/* 
    2021/5/9 13:55
    Attemped to use char* instead of string
    In theory it can improve some performance
    However it's too complicated to transform betwenn "char *const" and "const char *"
    also the cmp function should be overwrite.
    If we need the little performance enhancement when we finished the whole programe,
    the code block below is the cmp function we needed.

*/

/*
    struct ptrCmp{
        bool operator()( const char * s1, const char * s2 ) const
        {return strcmp( s1, s2 ) < 0;}
    };
*/

class Logger{
private:
    time_t _beginTime;
    unordered_map<string,clock_t> _timeStamp;
    bool printTime();
    FILE *_log;
    int flag;
public:
    Logger(); //default log: 
    Logger(string);
    ~Logger();
    int init();
    int start(string);
    int end(string);
    int error(string,string,int);
    int customMSG(string);
    int close();
};


#endif