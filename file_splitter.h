#pragma once
#include "block_reader.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <utility>

class file_splitter
{
public:
    file_splitter(size_t blocks)
        :m_blocks_count(blocks) {}
    std::vector<file_block_reader> split(const std::string& path);
private:
    size_t m_blocks_count;
};

template<typename K, typename V>
class mapper
{
public:
    mapper(
        const std::function<std::pair<K, V>(const std::string&)>& map_func,
        block_reader * reader)
        : m_map(map_func), m_block_reader(reader) {}
    void run(std::vector<std::pair<K, V>>& container);
private:
    std::function<std::pair<K, V>(const std::string&)> m_map;
    block_reader * m_block_reader;
};

template<typename K, typename V>
void mapper<K, V>::run(std::vector<std::pair<K, V>>& container)
{    
    std::string line;
    while(m_block_reader->get_next_line(line)) {
        container.push_back(m_map(line)); }
    std::sort(container.begin(), container.end());
}
