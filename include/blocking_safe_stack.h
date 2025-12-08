#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <stack>

template<typename T>
class blocking_thread_safe_stack
{
public:
    blocking_thread_safe_stack() = default;
    void             shutdown_stack();
    void             push(T value);
    std::optional<T> wait_and_pop();
    std::optional<T> wait_and_pop_for();

private:
    std::stack<T> m_data;

    // mutable для const методов
    mutable std::mutex      m_mutex;
    std::condition_variable m_cv;
    bool                    m_shutdown = false;
};
