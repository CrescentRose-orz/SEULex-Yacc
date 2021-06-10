#define eps 256
#define charSetMAX 127
#define VISUAL
#define THREADCNT 8

#define RdLock(x) shared_lock<shared_mutex> lock(x)
#define WrLock(x) lock_guard<shared_mutex> lock(x)
#define shared_mutex shared_timed_mutex
//#define shared_lock lock_guard //only for mac
//#define shared_mutex mutex //only for mac