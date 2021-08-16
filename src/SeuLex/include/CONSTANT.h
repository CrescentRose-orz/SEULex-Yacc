#include<mutex>
#include<shared_mutex>
#define fin lfile 
//#define USE_MULTITHREAD
//#define THREADCNT 8
//#define DEBUG
#define DEBUG2



#define eps 128
#define charSetMAX 128
#define charSetMin 32
#define controlMin 9
#define controlMax 12
#define VISUAL


#define RdLock(x) shared_lock<shared_timed_mutex> lock(x)
#define WrLock(x) lock_guard<shared_timed_mutex> lock(x)

//typedef  std::mutex _mutex_type;
typedef  std::shared_timed_mutex _mutex_type;
//#define shared_lock lock_guard //only for mac
//#define shared_mutex mutex //only for mac
