#include "../include/thread_safe_queue.h"

template<typename T>
template<typename U>
void thread_safe_queue<T>::push(U&& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.push(std::forward<U>(value));
}

template<typename T>
template<typename... Args>
void thread_safe_queue<T>::emplace(Args&&... args)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.emplace(std::forward<Args>(args)...);
}

template<typename T>
bool thread_safe_queue<T>::try_pop(T& value) noexcept
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
std::shared_ptr<T> thread_safe_queue<T>::try_pop() noexcept
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        return nullptr;
    }

    auto ptr = std::make_shared<T>(std::move(m_data.front()));
    m_data.pop();
    return ptr;
}

template<typename T>
void thread_safe_queue<T>::pop(T& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        throw std::runtime_error("queue is empty");
    }

    value = std::move(m_data.front());
    m_data.pop();
}

template<typename T>
std::shared_ptr<T> thread_safe_queue<T>::pop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        throw std::runtime_error("queue is empty");
    }

    auto ptr = std::make_shared<T>(std::move(m_data.front()));
    m_data.pop();
    return ptr;
}

template<typename T>
bool thread_safe_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.empty();
}

template<typename T>
size_t thread_safe_queue<T>::get_size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.size();
}

template<typename T>
void thread_safe_queue<T>::swap(thread_safe_queue& other) noexcept
{}
