#include "../include/lru_cache.h"
#include <stdexcept>

template<typename Key, typename Value>
lru_cache<Key, Value>::lru_cache(size_t size_cache)
    : m_capacity(size_cache)
{
    if (m_capacity == INVALID_CAPACITY)
    {
        throw std::invalid_argument("capacity ");
    }
}

template<typename Key, typename Value>
bool lru_cache<Key, Value>::get(const Key& key, Value& out)
{
    auto it = m_key_to_value.find(key);
    if (it == m_key_to_value.end())
    {
        return false;
    }

    out          = it->second;
    auto list_it = m_key_to_iter.find(key)->second;
    update_order(list_it);

    return true;
}

template<typename Key, typename Value>
void lru_cache<Key, Value>::put(const Key& key, Value in)
{
    auto it = m_key_to_value.find(key);

    if (it != m_key_to_value.end())
    {
        it->second   = std::move(in);
        auto list_it = m_key_to_iter.find(key)->second;
        update_order(list_it);
        return;
    }
    else
    {
        if (get_size() == m_capacity)
        {
            auto old_key = m_order.back();
            m_order.pop_back();
            m_key_to_iter.erase(old_key);
            m_key_to_value.erase(old_key);
        }

        m_order.push_front(key);
        m_key_to_iter[key]  = m_order.begin();
        m_key_to_value[key] = std::move(in);
    }
}

template<typename Key, typename Value>
size_t lru_cache<Key, Value>::get_size() const noexcept
{
    return m_key_to_value.size();
}

template<typename Key, typename Value>
bool lru_cache<Key, Value>::empty() const noexcept
{
    return m_key_to_value.empty();
}

template<typename Key, typename Value>
void lru_cache<Key, Value>::update_order(typename std::list<Key>::iterator it)
{
    if (it != m_order.begin())
    {
        Key key = *it;
        m_order.erase(it);
        m_order.push_front(key);
        m_key_to_iter[key] = m_order.begin();
    }
}
