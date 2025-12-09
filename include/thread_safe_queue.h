#pragma once
#include <memory>
#include <mutex>
#include <queue>

template<typename T>
class thread_safe_queue
{
public:
    thread_safe_queue()  = default;
    ~thread_safe_queue() = default;

    thread_safe_queue(const thread_safe_queue& other)            = delete;
    thread_safe_queue& operator=(const thread_safe_queue& other) = delete;

    thread_safe_queue(thread_safe_queue&& other) noexcept            = default;
    thread_safe_queue& operator=(thread_safe_queue&& other) noexcept = default;

    template<typename U>
    void push(U&& value);

    template<typename... Args>
    void emplace(Args&&... args);

    bool               try_pop(T& value) noexcept;
    std::shared_ptr<T> try_pop() noexcept;

    void               pop(T& value);
    std::shared_ptr<T> pop();

    bool   empty() const;
    size_t get_size() const;

    void swap(thread_safe_queue& other) noexcept;

private:
    mutable std::mutex m_mutex;
    std::queue<T>      m_data;
};
