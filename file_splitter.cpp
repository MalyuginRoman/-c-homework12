#include "file_splitter.h"

#include <filesystem>

std::vector<file_block_reader> file_splitter::split(const std::string& path)
{    
    std::vector<file_block_reader> readers;
    readers.reserve(m_blocks_count);
    std::filesystem::directory_entry file_info(path);
    auto block_start = 0;
    auto rest_size = file_info.file_size();
    if (m_blocks_count == 1) {
        readers.emplace_back(path, block_start, rest_size);
        return readers; }
    const char linebreak = '\n';
    char symbol;
    std::ifstream file_content(path, std::fstream::binary);
    for(size_t reader_i = 0; reader_i < m_blocks_count; ++reader_i) {
        auto rest_parts = m_blocks_count - reader_i;
        auto block_length = rest_size / rest_parts + 1;
        file_content.seekg(block_length, file_content.cur);
        do {
            file_content.get(symbol);
            if (!file_content) {
                break; }
            ++block_length; } 
        while (symbol != linebreak);
        readers.emplace_back(path, block_start, block_length);
        rest_size -= block_length;
        block_start += block_length; }
    return readers;
}
