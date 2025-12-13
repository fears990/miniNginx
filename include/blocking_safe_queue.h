#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template<typename T>

class blocking_thread_safe_queue
{
public:
    blocking_thread_safe_queue()                                             = default;
    blocking_thread_safe_queue(const blocking_thread_safe_queue&)            = delete;
    blocking_thread_safe_queue& operator=(const blocking_thread_safe_queue&) = delete;

    blocking_thread_safe_queue(blocking_thread_safe_queue&&) noexcept            = default;
    blocking_thread_safe_queue& operator=(blocking_thread_safe_queue&&) noexcept = default;

    void push(T value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.push(std::move(value));
        m_cv.notify_one();
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.emplace(std::forward<Args>(args)...);
        m_cv.notify_one();
    }

    std::optional<T> wait_and_pop()
    {
        std::unique_lock u_lock(m_mutex);
        m_cv.wait(u_lock, [this]() { return !m_data.empty() || m_shutdown; });
        if (m_data.empty() && m_shutdown)
        {
            return std::nullopt;
        }

        T tmp = std::move(m_data.front());
        m_data.pop();
        return tmp;
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_data.empty())
        {
            return false;
        }

        value = std::move(m_data.front());
        m_data.pop();
        return true;
    }

    size_t get_size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data.size();
    };
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        return m_data.empty();
    };
    void shutdown()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_shutdown = true;
        m_cv.notify_all();
    };

private:
    std::queue<T>           m_data;
    std::condition_variable m_cv;
    mutable std::mutex      m_mutex;
    bool                    m_shutdown = false;
};

