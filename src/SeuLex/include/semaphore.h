/*
* author: http://p9as.blogspot.com/2012/06/c11-semaphores.html
* modified by: ilovers
*/
#include <mutex>
#include <condition_variable>
namespace my{
    class semaphore {
    public:
        semaphore(int value=1): count{value}, wakeups{0} {}
        
        void wait(){
            std::unique_lock<std::mutex> lock{mutex};
            if (--count<0) { // count is not enough ?
                condition.wait(lock, [&]()->bool{ return wakeups>0;}); // suspend and wait ...
                --wakeups;  // ok, me wakeup !
            }
        }
        void signal(){
            std::lock_guard<std::mutex> lock{mutex};
            if(++count<=0) { // have some thread suspended ?
                ++wakeups;
                condition.notify_one(); // notify one !
            }
        }
        void signalAll(){
            condition.notify_all();
        }
    private:
        int count;
        int wakeups;
        std::mutex mutex;
        std::condition_variable condition;
    };
};
