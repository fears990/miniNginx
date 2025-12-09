#pragma once

#include <memory>
#include <mutex>
#include <stack>

template<typename T>
class thread_safe_stack
{
public:
    ~thread_safe_stack() = default;
    thread_safe_stack(const thread_safe_stack& other);
    thread_safe_stack(const thread_safe_stack&& other);
    thread_safe_stack& operator=(thread_safe_stack& other);
    thread_safe_stack& operator=(thread_safe_stack&& other);

    template<typename U>
    void push(U&& value);

    template<typename... Args>
    void emplace(Args&&... args);

    bool               try_pop(T& value) noexcept;
    std::shared_ptr<T> try_pop() noexcept;

    void               pop(T& value);
    std::shared_ptr<T> pop();

    bool               empty() const;
    size_t             get_size() const;
    void               swap(thread_safe_stack& other) noexcept;
    // friend void        swap(thread_safe_stack& obj_1, thread_safe_stack& obj_2);

private:
    std::stack<T>      m_data;
    mutable std::mutex m_mutex;
};
