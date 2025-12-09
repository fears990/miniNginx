#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>

static constexpr size_t INVALID_CAPACITY = 0;

template<typename Key, typename Value>
class lru_cache
{
public:
    explicit lru_cache(size_t size_cache);

    bool   get(const Key& key, Value& out);
    void   put(const Key& key, Value in);
    size_t get_size() const noexcept;
    bool   empty() const noexcept;

private:
    size_t                                                     m_capacity;
    std::list<Key>                                             m_order;
    std::unordered_map<Key, Value>                             m_key_to_value;
    std::unordered_map<Key, typename std::list<Key>::iterator> m_key_to_iter;

    void update_order(typename std::list<Key>::iterator it);
};
