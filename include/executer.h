#pragma once

#include "../include/lru_cache.h"
#include "../include/thread_pool.h"
#include <future>
#include <type_traits>

template<typename Key, typename Value>
class executer
{
public:
    template<typename F>
    using result_type = std::invoke_result<F>;

    template<typename F>
    std::future<result_type<F>> submit(F&& f)
    {
        if (m_shutdown)
        {
            throw std::runtime_error("Executor is off");
        }

        using result_type = std::invoke_result<F>;
        std::promise<result_type> promis;
        auto                      future = promis.get_future();

        auto task = [f = std::forward<F>(f), local_pomis = std::move(promis)]() mutable{
            try
            {
                auto result = f();
                local_pomis.set_value(result);
            }
            catch (...)
            {
                local_pomis.set_exception(std::current_exception());
            }
        };

        return future;
    }

private:
    std::unique_ptr<lru_cache<Key, Value>> m_cache;
    std::unique_ptr<thread_pool>           m_pool;
    std::atomic<bool>                      m_shutdown = false;
};
