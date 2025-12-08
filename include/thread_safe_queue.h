#pragma once
#include <memory>
#include <mutex>
#include <queue>

class thread_safe_queue
{
public:
    thread_safe_queue()  = default;
    ~thread_safe_queue() = default;

    thread_safe_queue(const thread_safe_queue&)            = delete;
    thread_safe_queue& operator=(const thread_safe_queue&) = delete;

    thread_safe_queue(thread_safe_queue&&) noexcept            = default;
    thread_safe_queue& operator=(thread_safe_queue&&) noexcept = default;

private:
};
