# ThreadPool

The **ThreadPool** provides a multi-threaded task execution mechanism. It allows you to submit tasks to be executed concurrently by multiple worker threads. The class supports various types of tasks and prioritizes their execution based on their priority levels.

## Features

- Execute tasks concurrently using a pool of worker threads.
- Support for different task priorities: Low, Medium, and High.
- Pause and resume functionality to control the execution of tasks.
- Future tasks that return a value upon completion.

## Usage

### Creating a ThreadPool

To create an instance of the `ThreadPool` class, you need to specify the initial number of worker threads. This sets up the pool of threads that will be used to execute tasks.

```cpp
#include "thread_pool.hpp"

int main()
{
    // Create a ThreadPool with 4 initial worker threads
    ilrd::ThreadPool threadPool(4);

    // ...
}
```

### Adding Tasks to the ThreadPool
You can add tasks to the ThreadPool using the Add method. Tasks can be of various types, such as functions, functors, or lambda expressions. You also need to specify the priority of the task.

```cpp
// Example of adding a function task to the ThreadPool
std::function<void(void)> myFunction = []() { /* Task code here */ };
threadPool.Add(std::make_shared<ilrd::ThreadPool::FunctionTask>(myFunction), ilrd::ThreadPool::Priority::MEDIUM);

// ...
```

### Pausing and Resuming Threads

You can pause and resume the execution of worker threads in the ThreadPool. This can be useful to control the execution of tasks under specific conditions.

```cpp
// Pause the execution of worker threads
threadPool.Pause();

// Resume the execution of worker threads
threadPool.Resume();

// ...

```

### Future Tasks

The ThreadPool also supports future tasks that return a value upon completion. You can use the FutureTask template to create such tasks.

```cpp
// Example of adding a future task to the ThreadPool
std::function<int(void)> myFutureFunction = []() { return 42; };
auto futureTask = std::make_shared<ilrd::ThreadPool::FutureTask<int>>(myFutureFunction);
threadPool.Add(futureTask, ilrd::ThreadPool::Priority::HIGH);

// Wait for the task to complete and get the result
int result = futureTask->Get();

// ...
```
### Requirements
C++20 or later
The priority_queue.hpp, waitable_queue.hpp, and utils.hpp headers provided by ILRD.

















