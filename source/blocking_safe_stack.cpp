#include "../include/blocking_safe_stack.h"

template<typename T>
void blocking_thread_safe_stack<T>::shutdown_stack()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_shutdown = true;
    m_cv.notify_all();
}

template<typename T>
void blocking_thread_safe_stack<T>::push(T value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.push(std::move(value));
    m_cv.notify_one();
}

template<typename T>
std::optional<T> blocking_thread_safe_stack<T>::wait_and_pop()
{
    std::unique_lock u_lock(m_mutex);
    m_cv.wait(u_lock, [this]() { return !m_data.empty() || m_shutdown; });

    if (m_shutdown && m_data.empty())
    {
        return std::nullopt;
    }

    T value = std::move(m_data.top());
    m_data.pop();
    return value;
}

template<typename T>
std::optional<T> blocking_thread_safe_stack<T>::wait_and_pop_for()
{}
