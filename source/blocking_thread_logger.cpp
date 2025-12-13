#include "../include/blocking_thread_logger.h"
#include <fstream>

blocking_thread_logger::blocking_thread_logger(const std::string& file_name)
    : m_file_name(file_name)
{
    m_worker = std::thread(&blocking_thread_logger::worker_loop, this);
}

blocking_thread_logger::~blocking_thread_logger()
{
    m_running = true;
    m_queue_file_log.shutdown();
    stop_worker();
}

void blocking_thread_logger::log(const std::string_view& msg)
{
    m_queue_file_log.push(std::string(msg));
}

void blocking_thread_logger::worker_loop()
{
    std::ofstream            out_file(m_file_name, std::ios::app | std::ios::binary);
    std::vector<std::string> local_buff;
    local_buff.reserve(BATCH_SIZE);

    if (!out_file.is_open())
    {
        throw std::runtime_error("Cannot open file " + m_file_name);
    }

    while (m_running || !m_queue_file_log.empty())
    {
        auto opt = m_queue_file_log.wait_and_pop();
        if (!opt.has_value())
        {
            break;
        }

        local_buff.push_back(std::move(*opt));

        if (local_buff.size() >= BATCH_SIZE)
        {
            for (const auto& log : local_buff)
            {
                out_file << log << '\n';
            }
            local_buff.clear();
            out_file.flush();
        }
    }

    for (const auto& log : local_buff)
    {
        out_file << log << '\n';
    }
    out_file.flush();
}

void blocking_thread_logger::stop_worker()
{
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}
