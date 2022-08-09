#ifndef SIMPLETHREADPOOL_H
#define SIMPLETHREADPOOL_H
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <functional>
#include <future>
#include <memory>
#include <type_traits>

template<typename Callable>
using return_type_of_t =
    typename decltype(std::function<Callable>{std::declval<Callable>()})::result_type;

class SimpleThreadPool {
    public:
    explicit SimpleThreadPool(std::size_t threadCount = std::thread::hardware_concurrency());
    ~SimpleThreadPool();
    SimpleThreadPool(const SimpleThreadPool & ) = delete;
    SimpleThreadPool & operator = (const SimpleThreadPool & ) = delete;
    template <typename Fnc_T, std::enable_if_t<!std::is_same<return_type_of_t<Fnc_T>, void>::value, bool> = true>
    auto Post(Fnc_T task) -> std::future<decltype(task())>;
    template <typename Fnc_T, std::enable_if_t<std::is_same<return_type_of_t<Fnc_T>, void>::value, bool> = false>
    auto Post(Fnc_T task) -> std::future<decltype(task())>;
    void WorkOn();
    void Destroy();

    private:
        size_t m_threadCount;
        std::vector<std::thread> threads;
        std::queue <std::function <void()>> tasks;
        std::condition_variable condition;
        std::mutex mut;
        bool stop = false;
        void join();
};

template <typename Fnc_T, std::enable_if_t<!std::is_same<return_type_of_t<Fnc_T>, void>::value, bool> = true>
auto SimpleThreadPool::Post(Fnc_T task) -> std::future<decltype(task())>
{
    auto task_promise = std::make_shared<std::promise<decltype(task())>>();
    auto task_future = task_promise->get_future();
    auto thread_task = [t = std::move(task), tp = task_promise]() {
        auto result = t();
        tp->set_value(result);
    };
    {
        std::lock_guard<std::mutex> lk(mut);
        tasks.push(std::move(thread_task));
    }
    condition.notify_one();

    return task_future;
}

template <typename Fnc_T, std::enable_if_t<std::is_same<return_type_of_t<Fnc_T>, void>::value, bool> = false>
auto SimpleThreadPool::Post(Fnc_T task) -> std::future<decltype(task())>
{
    auto task_promise = std::make_shared<std::promise<decltype(task())>>();
    auto task_future = task_promise->get_future();
    auto thread_task = [t = std::move(task), tp = task_promise]() {
        t();
        tp->set_value();
    };
    {
        std::lock_guard<std::mutex> lk(mut);
        tasks.push(std::move(thread_task));
    }
    condition.notify_one();

    return task_future;
}

#endif // SIMPLETHREADPOOL_H
