#include "SimpleThreadPool.h"

SimpleThreadPool::SimpleThreadPool(std::size_t threadCount) : m_threadCount(threadCount)
{
    for(int i = 0; i < m_threadCount; ++i) {
        threads.push_back(std::thread(&SimpleThreadPool::WorkOn, this));
    }
}

 void SimpleThreadPool::WorkOn()
 {
    std::function<void()> task;
    while(!stop) {
         {
            std::unique_lock<std::mutex> lk(mut);
            condition.wait(lk, [this](){return !tasks.empty() || stop;});
            if(stop)
                return;
            task = tasks.front();
            tasks.pop();
        }
        task();
    }
 }

void SimpleThreadPool::Destroy()
{
    stop = true;
    condition.notify_all();
    join();
}

void SimpleThreadPool::join()
{
    for(auto &t : threads)
        if(t.joinable())
            t.join();
}

SimpleThreadPool::~SimpleThreadPool()
{
    join();
}
