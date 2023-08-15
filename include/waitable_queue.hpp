#ifndef __WAITABLE_QUEUE__
#define __WAITABLE_QUEUE__

#include <boost/chrono/duration.hpp>
#include <queue>                                              // std::queue
#include <mutex>  
#include <boost/noncopyable.hpp>                              // noncopyable
#include <boost/interprocess/sync/interprocess_semaphore.hpp> //boost::semaphore
#include <boost/chrono.hpp>                                   // boost::chrono::miliseconds
#include <boost/date_time/posix_time/posix_time.hpp>

#include "utils.hpp"


template <class Container>
class WaitablePriorityQueue : private noncopyable
{
public:
    WaitablePriorityQueue();
    void Enqueue(typename Container::value_type value);
    bool Dequeue(boost::posix_time::milliseconds timeout, typename Container::reference value);
    typename Container::value_type Dequeue();
    bool IsEmpty() const;

private:
    Container m_queue;
    mutable std::mutex m_mutex;
   // std::counting_semaphore m_sem;
     boost::interprocess::interprocess_semaphore m_sem;
};

template <class Container>
inline bool WaitablePriorityQueue<Container>::IsEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_queue.empty();
}

template <class Container>
inline void WaitablePriorityQueue<Container>::Enqueue(typename Container::value_type value)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_queue.push(value);

    m_sem.post();
}

template <class Container>
inline typename Container::value_type WaitablePriorityQueue<Container>::Dequeue()
{
    m_sem.wait();

    std::lock_guard<std::mutex> lock(m_mutex);

    typename Container::value_type temp;

    try
    {
       temp = m_queue.front();
    }

    catch (const std::exception &e)
    {
        m_sem.post();
        throw;
    }

    m_queue.pop();

    return temp;
}

template <class Container>
inline bool WaitablePriorityQueue<Container>::Dequeue(boost::posix_time::milliseconds timeout,
                                                            typename Container::reference value)
{

/*         if(!m_sem.try_aquire_for(timeout));
    {
        return false;
    } */

    if (!m_sem.timed_wait(boost::posix_time::microsec_clock::universal_time() + timeout))
    {
        return false;
    } 

    std::lock_guard<std::mutex> lock(m_mutex);

    try
    {
        value = m_queue.front();
    }

    catch (const std::exception &e)
    {
        m_sem.post();
        throw;
    }

    m_queue.pop();

    return true;
}

template <class Container>
inline WaitablePriorityQueue<Container>::WaitablePriorityQueue() : m_sem(0)
{
}



#endif // __WAITABLE_QUEUE__
