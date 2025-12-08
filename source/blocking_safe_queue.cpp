#include "../include/blocking_safe_queue.h"

template<typename T>
void blocking_thread_safe_queue<T>::push(T value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.push(std::move(value));
    m_cv.notify_one();
}

template<typename T>
std::optional<T> blocking_thread_safe_queue<T>::wait_and_pop()
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

template<typename T>
bool blocking_thread_safe_queue<T>::try_pop(T& value)
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

template<typename T>
size_t blocking_thread_safe_queue<T>::get_size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.size();
}

template<typename T>
bool blocking_thread_safe_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_data.empty();
}

template<typename T>
void blocking_thread_safe_queue<T>::shutdown()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_shutdown = true;
    m_cv.notify_all();
}

template<typename T>
template<typename... Args>
void blocking_thread_safe_queue<T>::emplace(Args&&... args)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.emplace(std::forward<Args>(args)...);
    m_cv.notify_one();
}
