#include "../include/thread_pool.h"

thread_pool::thread_pool(size_t workers)
{
    if (workers == 0)
    {
        throw std::invalid_argument("Workers are absent");
    }

    try
    {
        start_pool(workers);
    }
    catch (const std::system_error& e)
    {
        stop_pool();
        throw std::runtime_error(std::string("Failed to create thread: ") + e.what());
    }
}

thread_pool::~thread_pool()
{
    stop_pool();
}

void thread_pool::stop_pool()
{
    {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_stop_work = true;
    }

    m_cv.notify_all();

    for (auto& worker : m_workers)
    {
        worker.join();
    }
}

void thread_pool::start_pool(size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        m_workers.emplace_back(
            [this]()
            {
                while (true)
                {
                    task curr_task;
                    {
                        std::unique_lock<std::mutex> u_lock(m_mutex);
                        m_cv.wait(u_lock, [this] { return !m_queue_tasks.empty() || m_stop_work; });

                        if (m_stop_work && m_queue_tasks.empty())
                        {
                            return;
                        }

                        curr_task = std::move(m_queue_tasks.front());
                        m_queue_tasks.pop();
                    }

                    curr_task();
                }
            });
    }
}
