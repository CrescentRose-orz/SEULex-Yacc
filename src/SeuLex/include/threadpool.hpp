#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "CONSTANT.h"
namespace std
{
#define  MAX_THREAD_NUM 256
/*
    2021/5/10 12:48 来源网络
    经测试可用
    类成员需要使用std::bind进行commit
    exp. commit(bind(qwq,arg1,arg2));
    新增join函数，用于阻塞确保任务完成
*/
//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
//不支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
class threadpool
{
    using Task = std::function<void()>;
    // 线程池
    std::vector<std::thread> pool;
    // 任务队列
    std::queue<Task> tasks;
    // 同步
    std::mutex m_lock;
    // 条件阻塞
    std::condition_variable cv_task;
    // 是否关闭提交
    std::atomic<bool> closeCommit;
    //是否结束
    std::atomic<bool> stoped;
    //空闲线程数量
    std::atomic<int>  idlThrNum;

    int size;

public:
    inline threadpool(unsigned short size = THREADCNT) :stoped{ false },closeCommit{ false }
    {
        this->size = size < 1 ? 1 : size;
        idlThrNum = this -> size;
        for (size = 0; size < idlThrNum; ++size)
        {   //初始化线程数量
            pool.emplace_back(
                [this]
                { // 工作线程函数
                    while(!this->stoped)
                    {
                        std::function<void()> task;
                        {   // 获取一个待执行的 task
                            std::unique_lock<std::mutex> lock{ this->m_lock };// unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
                            this->cv_task.wait(lock,
                                [this] {
                                    return this->stoped.load() || !this->tasks.empty();
                                }
                            ); // wait 直到有 task
                            if (this->stoped && this->tasks.empty())
                                return;
                            task = std::move(this->tasks.front()); // 取一个 task
                            this->tasks.pop();
                        }
                        idlThrNum--;
                        task();
                        idlThrNum++;
                    }
                }
            );
        }
    }
    inline ~threadpool()
    {
        stoped.store(true);
        cv_task.notify_all(); // 唤醒所有线程执行
        for (std::thread& thread : pool) {
            //thread.detach(); // 让线程“自生自灭”
            if(thread.joinable())
                thread.join(); // 等待任务结束， 前提：线程一定会执行完
        }
    }

public:
    // 提交一个任务
    // 调用.get()获取返回值会等待任务执行完,获取返回值
    // 有两种方法可以实现调用类成员，
    // 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
    // 一种是用 mem_fn： .commit(std::mem_fn(&Dog::sayHello), &dog)
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
    {
        if (stoped.load())    // stop == true ??
            throw std::runtime_error("commit on ThreadPool is stopped.");

        using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
        auto task = std::make_shared<std::packaged_task<RetType()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );    // wtf !
        std::future<RetType> future = task->get_future();
        {    // 添加任务到队列
            std::lock_guard<std::mutex > lock{ m_lock };//对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
            tasks.emplace(
                [task]()
                { // push(Task{...})
                    (*task)();
                }
            );
        }
        cv_task.notify_one(); // 唤醒一个线程执行
        return future;
    }

    //空闲线程数量
    int idlCount() { return idlThrNum; }

    int close(){
        int cnt = 0;
        while(1){
            for (int i = 0 ; i < 10000000;++i){}
            lock_guard<mutex> lock(m_lock);
            if (tasks.empty()&&idlThrNum == size){
                break;
            } else {
                ++cnt;
                if (cnt == 10){
                    cnt = 0;
                    cout<<tasks.size()<<" "<<idlThrNum<<endl;
                }
            }
        }
        cv_task.notify_all(); // 唤醒所有线程执行
        for (std::thread& thread : pool) {
            //thread.detach(); // 让线程“自生自灭”
            if(thread.joinable())
                thread.join(); // 等待任务结束， 前提：线程一定会执行完
        }
        return 0;
    }
    int join(){
        closeCommit.store(true);
        while(!tasks.empty()||idlThrNum != size);
        while(tasks.empty());
        while(idlThrNum != size);
        closeCommit.store(false);
        return 0;
    }
};

}

#endif