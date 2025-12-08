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

    void push(T value);

    template<typename... Args>
    void emplace(Args&&... args);

    std::optional<T> wait_and_pop();
    bool             try_pop(T& value);
    size_t           get_size() const;
    bool             empty() const;
    void             shutdown();

private:
    std::queue<T>           m_data;
    std::condition_variable m_cv;
    mutable std::mutex      m_mutex;
    bool                    m_shutdown = false;
};
