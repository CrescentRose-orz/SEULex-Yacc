#define fin lfile 
//#define USE_MULTITHREAD
#define DEBUG
// transform NFA DFA miniDFA Logger visualGraph



#define eps 128
#define charSetMAX 128
#define VISUAL
//#define MULTI
#define THREADCNT 8

#define RdLock(x) shared_lock<shared_mutex> lock(x)
#define WrLock(x) lock_guard<shared_mutex> lock(x)
#define shared_mutex shared_timed_mutex
#define shared_lock lock_guard //only for mac
#define shared_mutex mutex //only for mac