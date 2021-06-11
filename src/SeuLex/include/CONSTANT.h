#include<mutex>
#include<shared_mutex>
#define fin lfile 
//#define USE_MULTITHREAD
#define DEBUG
// transform NFA DFA miniDFA Logger visualGraph



#define eps 128
#define charSetMAX 128
#define VISUAL
//#define MULTI
#define THREADCNT 8

#define RdLock(x) shared_lock<shared_timed_mutex> lock(x)
#define WrLock(x) lock_guard<shared_timed_mutex> lock(x)
//#define shared_mutex shared_timed_mutex
//#define RdLock(x) x = 1;//lock_guard<mutex> lock(x)
//#define WrLock(x) x = 1;//lock_guard<mutex> lock(x)

//typedef  std::mutex _mutex_type;
typedef  std::shared_timed_mutex _mutex_type;
//#define shared_lock lock_guard //only for mac
//#define shared_mutex mutex //only for mac
