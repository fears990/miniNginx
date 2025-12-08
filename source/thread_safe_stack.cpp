#include "../include/thread_safe_stack.h"

template<typename T>
thread_safe_stack<T>::thread_safe_stack(const thread_safe_stack& other)
{
    std::lock_guard<std::mutex> lock(other.m_mutex);
    m_data = other.m_data;
}

template<typename T>
thread_safe_stack<T>::thread_safe_stack(const thread_safe_stack&& other)
{
    std::lock_guard<std::mutex> lock(other.m_mutex);
    m_data = std::move(other.m_data);
}

template<typename T>
thread_safe_stack<T>& thread_safe_stack<T>::operator=(const thread_safe_stack& other)
{
    if (this == &other)
    {
        return *this;
    }

    std::scoped_lock lock(m_mutex, other.m_mutex);

    m_data = other.m_data;
    return *this;
}

template<typename T>
bool thread_safe_stack<T>::try_pop(T& value) noexcept
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        return false;
    }

    value = std::move(m_data.top);
    m_data.pop();

    return true;
}

template<typename T>
std::shared_ptr<T> thread_safe_stack<T>::try_pop() noexcept
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        return nullptr;
    }

    auto shared_ptr = std::make_shared<T>(std::move(m_data.top()));
    m_data.pop();
    return shared_ptr;
}

template<typename T>
void thread_safe_stack<T>::pop(T& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        throw std::runtime_error("Empty stack");
    }

    value = std::move(m_data.top());
    m_data.pop();
}

template<typename T>
std::shared_ptr<T> thread_safe_stack<T>::pop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        throw std::runtime_error("Empty stack");
    }

    auto shared_ptr = std::make_shared<T>(std::move(m_data.pop()));
    m_data.pop();
    return shared_ptr;
}

template<typename T>
bool thread_safe_stack<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data.empty())
    {
        return false;
    }
    return true;
}

template<typename T>
size_t thread_safe_stack<T>::get_size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.size();
}

template<typename T>
void thread_safe_stack<T>::swap(thread_safe_stack& other) noexcept
{}

template<typename T>
thread_safe_stack<T>& thread_safe_stack<T>::operator=(const thread_safe_stack&& other)
{
    if (this == &other)
    {
        return *this;
    }

    std::unique_lock lock_1(m_mutex, std::defer_lock);
    std::unique_lock lock_2(other.m_mutex, std::defer_lock);
    std::lock(lock_1, lock_2);

    m_data = std::move(other.m_data);
    return *this;
}

template<typename T>
template<typename U>
void thread_safe_stack<T>::push(U&& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data = std::forward<U>(value);
}

template<typename T>
template<typename... Args>
void thread_safe_stack<T>::emplace(Args&&... args)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.emplace(std::forward<Args>(args)...);
}
