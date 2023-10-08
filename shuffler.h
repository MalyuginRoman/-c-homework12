#pragma once

#include <iterator>
#include <functional>
#include <list>
#include <numeric>
#include <vector>

template<typename Container> 
struct container_trace
{
    container_trace(Container & container)
        :m_container(container), m_pos(container.begin()), m_end(container.end()) {}
    bool is_finished() const {
        return m_pos == m_end; }
    Container & m_container;
    typename Container::iterator m_pos;
    typename Container::iterator m_end;
};

template<typename K, typename V>
class shuffler
{
public:
    void run(std::vector<std::vector<std::pair<K, V>>>& after_map,
             std::vector<std::vector<std::pair<K, V>>>& for_reduce);
};


template<typename K, typename V>
void shuffler<K, V>::run(
        std::vector<std::vector<std::pair<K, V>>>& after_map,
        std::vector<std::vector<std::pair<K, V>>>& for_reduce)
{
    auto count_fold  = [](size_t count, const std::vector<std::pair<K, V>>& container) {
        return count + container.size(); };
    auto mapped_count = std::accumulate(after_map.begin(), after_map.end(), 0, count_fold);
    auto per_reducer_count = mapped_count / for_reduce.size();
    if (mapped_count % for_reduce.size() != 0) {
        per_reducer_count += 1; }
    std::list<container_trace<std::vector<std::pair<K, V>>>> containers;
    for(auto& container: after_map) {
        if (!container.empty()) {
            containers.emplace_back(container);
        }
    }
    auto reducer = for_reduce.begin();
    reducer->reserve(per_reducer_count);
    while(!containers.empty()) {
        auto min_key_container = containers.begin();
        auto min_key = min_key_container->m_pos->first;
        for(auto container_it = next(min_key_container); container_it != containers.end(); ++container_it) {
            if (container_it->m_pos->first < min_key) {
                min_key_container = container_it;
                min_key = min_key_container->m_pos->first;
            }
        }
        for(auto container_it = min_key_container; container_it != containers.end(); ++container_it) {
            if (container_it->m_pos->first != min_key)
                continue;
            auto from_iterator = std::make_move_iterator(container_it->m_pos);
            auto end_iterator = std::make_move_iterator(container_it->m_end);
            auto to_iterator = std::make_move_iterator(container_it->m_pos);
            auto count = 0;
            while((to_iterator != end_iterator) && (to_iterator->first == min_key)) {
                ++to_iterator;
                ++count;
            }
            reducer->insert(reducer->end(), from_iterator, to_iterator);
            std::advance(container_it->m_pos, count);
            if (container_it->is_finished()) {
                auto to_remove = container_it;
                --container_it; 
                containers.erase(to_remove);
            }
        }
        if (reducer->size() >= per_reducer_count) {
            ++reducer;
            reducer->reserve(per_reducer_count);
        }
    }
}

template<typename Accumulate, typename Key, typename Value>
class reducer
{
public:
    reducer(const std::function<void(Accumulate&, const std::pair<Key, Value>&)>& reduce_func) : m_reduce(reduce_func){}
    Accumulate run(const std::vector<std::pair<Key, Value>>& pairs);
private:
    std::function<void(Accumulate&, const std::pair<Key, Value>&)> m_reduce;
};

template<typename Accumulate, typename Key, typename Value>
Accumulate reducer<Accumulate, Key, Value>::run(const std::vector<std::pair<Key, Value>>& pairs)
{
    Accumulate accumulate;
    for(const auto& pair: pairs) {
        m_reduce(accumulate, pair);
    }
    return accumulate;
}
