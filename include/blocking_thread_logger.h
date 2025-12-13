#pragma once
#include "../include/blocking_safe_queue.h"
#include "../include/i_logger.h"
#include <atomic>
#include <memory>
#include <thread>

static constexpr size_t BATCH_SIZE = 1000;

class blocking_thread_logger : public i_logger
{
public:
    explicit blocking_thread_logger(const std::string& file_name);
    ~blocking_thread_logger();

    void log(const std::string_view& msg) override;

private:
    std::thread                             m_worker;
    std::string                             m_file_name;
    blocking_thread_safe_queue<std::string> m_queue_file_log;
    std::atomic<bool>                       m_running = false;

    void worker_loop();
    void stop_worker();
};

i_logger* create_raw_logger(const std::string& file_name)
{
    return new blocking_thread_logger(file_name);
}

std::unique_ptr<i_logger> create_logger(const std::string& file_name)
{
    return std::make_unique<blocking_thread_logger>(file_name);
}

// FileLogger in work
// NetworkLogger
// MultiLogger
// AsyncLogger
// ConsoleLogger
