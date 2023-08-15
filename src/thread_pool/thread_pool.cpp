#include "thread_pool.hpp"


ThreadPool::FunctionTask::FunctionTask(std::function<void(void)> &function) : 
m_function(function) {}

void ThreadPool::FunctionTask::Run()
{
    m_function();
}

ThreadPool::ThreadPool(std::size_t initialNumThreads):
    m_workingThreadsMap(),
    m_removeThreadFunc([&]()
    {
        m_threadShouldLive[std::this_thread::get_id()] = false;
    }),
    m_removeFunctionSp(new FunctionTask(m_removeThreadFunc)),
    m_pauseThreadsFunc([&]()
    {
        m_pauseThreads.acquire();
    }),
    m_pauseThreadsSp(new FunctionTask(m_pauseThreadsFunc)),
    m_pauseThreads(0),
    m_shouldStop(false),
    m_isPaused(false),
    m_threadsNum(0),
    m_counter(0)
{
    for (std::size_t i = 0; i < initialNumThreads; i++)
    {
        std::thread thread(&ThreadPool::ThreadHandler, std::ref(*this));
        m_workingThreadsMap[thread.get_id()] =  std::move(thread);
    }
}

void ThreadPool::ThreadHandler()
{
    m_mutex.lock();
    m_threadShouldLive.insert(std::make_pair(std::this_thread::get_id(), true));
    m_mutex.unlock();

    while (m_threadShouldLive[std::this_thread::get_id()])
    {
        task_t task = m_threadsQueue.Dequeue();
        task.ptr->Run();
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_detachedThreads.Enqueue(std::this_thread::get_id());
}

ThreadPool::~ThreadPool()
{
    SetNumOfThreads(0);
 }

void ThreadPool::SetNumOfThreads(std::size_t numThreads)
{
    int threadsDiff = numThreads - m_workingThreadsMap.size();

    if (threadsDiff > 0)
    {
        for (int i = 0; i < threadsDiff; i++)
        {
            std::thread thread(&ThreadPool::ThreadHandler, std::ref(*this));
            m_workingThreadsMap[thread.get_id()] =  std::move(thread);
        }
    }

    else if (threadsDiff < 0)
    {
        for (int i = 0; i > threadsDiff; i--)
        {
            Add(m_removeFunctionSp, static_cast<Priority>(MIN_PRIORITY));
        }

        for (int i = 0; i > threadsDiff; i--)
        {
            std::thread thread = std::move(m_workingThreadsMap[m_detachedThreads.Dequeue()]);
            m_workingThreadsMap.erase(thread.get_id());
            thread.join();
        }
    }
}

bool ThreadPool::Compare::operator()(const task_t &lhs, const task_t &rhs) const
{
    if(lhs.priority == rhs.priority)
    {
        return lhs.counter > rhs.counter;
    }
    
    return lhs.priority < rhs.priority;
}

void ThreadPool::Add(std::shared_ptr<Task> task, Priority priority)
{
    size_t counter = m_counter;
    m_counter++;
    m_threadsQueue.Enqueue({task, priority, counter});
}

void ThreadPool::Pause()
{
    if(!m_isPaused)
    {
        m_isPaused = true;

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_threadsNum = m_workingThreadsMap.size();
        }

        for(size_t i = 0; i < m_threadsNum;  i++)
        {
            Add(m_pauseThreadsSp,  static_cast<Priority>(SpecialPriority::MAX_PRIORITY));
        }
    }
}

void ThreadPool::Resume()
{
    if(m_isPaused)
    {
        m_isPaused = false;

        for(size_t i = 0; i < m_threadsNum;  i++)
        {
            m_pauseThreads.release();
        }
    }
}
