#ifndef __THREAD_POOL__
#define __THREAD_POOL__

// Standard library headers
#include <thread>             //std::thread
#include <functional>         // std::function
#include <condition_variable> // std::condition_variable
#include <vector>             // std::vector
#include <memory>             // std::shared_ptr
#include <map>                // std::map
#include <semaphore>

// Additional Headers
#include "priority_queue.hpp" // priority_queue
#include "waitable_queue.hpp" // waitable queue
#include "utils.hpp"     // noncopyable


class ThreadPool : private noncopyable
{
public:
    class Task
    {
    public:
        virtual void Run() = 0;
        virtual ~Task() {}
    };

    class FunctionTask : public Task
    {
    public:
        FunctionTask(std::function<void(void)> &function);
        virtual void Run();

    private:
        std::function<void(void)> m_function;
    };

    template <typename T>
    class FutureTask : public Task
    {
    public:
        FutureTask(std::function<T(void)> &function);
        virtual void Run();
        T Get() const;

    private:
        T m_futureObj;
        mutable std::binary_semaphore m_getIsReady{0};
        std::function<T(void)> m_function;
    };

    typedef struct taskData
    {
        std::shared_ptr<Task> ptr;
        int priority;
        size_t counter;
    } task_t;

    // typedef std::pair<std::shared_ptr<Task>, int> task_t;

    struct Compare
    {
    public:
        bool operator()(const task_t &lhs, const task_t &rhs) const;
    };

    ThreadPool(std::size_t initialNumThreads); // Might Throw... TODO
    ~ThreadPool();
    // Copy and assignment blocked by noncopyable

    enum Priority
    {
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3
    };

    void Add(std::shared_ptr<Task> task, Priority priority);
    void Pause();                                 // Might Throw... TODO
    void Resume();                                // Might Throw... TODO
    void SetNumOfThreads(std::size_t numThreads); // Might Throw... TODO

private:
    WaitablePriorityQueue<PriorityQueue<task_t, std::vector<task_t>, Compare>> m_threadsQueue;
    WaitablePriorityQueue<std::queue<std::thread::id>> m_detachedThreads;
    void ThreadHandler();
    std::map<std::thread::id, std::thread> m_workingThreadsMap;
    std::map<std::thread::id, bool> m_threadShouldLive;
    std::mutex m_mutex;
    std::function<void(void)> m_removeThreadFunc;
    std::shared_ptr<FunctionTask> m_removeFunctionSp;
    std::function<void(void)> m_pauseThreadsFunc;
    std::shared_ptr<FunctionTask> m_pauseThreadsSp;
    std::counting_semaphore<std::__semaphore_impl::_S_max> m_pauseThreads;
    bool m_shouldStop;
    bool m_isPaused;
    enum SpecialPriority
    {
        MAX_PRIORITY = 4,
        MIN_PRIORITY = 0
    };
    size_t m_threadsNum;
    size_t m_counter;
};

template <typename T>
inline ThreadPool::FutureTask<T>::FutureTask(std::function<T(void)> &function) : m_function(function)
{
}

template <typename T>
inline T ThreadPool::FutureTask<T>::Get() const
{
    m_getIsReady.acquire();
    m_getIsReady.release();

    return m_futureObj;
}

template <typename T>
inline void ThreadPool::FutureTask<T>::Run()
{
    m_futureObj = m_function();
    m_getIsReady.release();
}



#endif //__THREAD_POOL__
