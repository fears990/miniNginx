#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class thread_pool
{
public:
    using task = std::function<void()>;

    explicit thread_pool(size_t workers);
    ~thread_pool();

    template<typename T>
    void add_task(T&& task)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (m_stop_work)
            {
                throw std::runtime_error("All threads is dead");
            }

            m_queue_tasks.emplace(std::forward<T>(task));
        }

        m_cv.notify_one();
    }

private:
    std::mutex                        m_mutex;
    std::condition_variable           m_cv;
    bool                              m_stop_work = false;
    std::queue<std::function<void()>> m_queue_tasks;
    std::vector<std::thread>          m_workers;

    void stop_pool();
    void start_pool(size_t count);
};
