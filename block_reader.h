#pragma once
#include <fstream>
#include <string>

class block_reader
{
public:
    virtual bool get_next_line(std::string& line) = 0;
    virtual void reset() {}
    virtual ~block_reader() = default;
};


class file_block_reader : public block_reader
{
public:
    file_block_reader(const std::string& path, uintmax_t start, uintmax_t length)
        :m_path(path), m_start(start), m_length(length) {
        m_filestream.open(m_path);
        m_filestream.seekg(m_start, m_filestream.beg); }
    file_block_reader(const file_block_reader& other)
        :m_path(other.m_path), m_start(other.m_start), m_length(other.m_length) {
        m_filestream.open(m_path);
        m_filestream.seekg(m_start, m_filestream.beg); }
    bool get_next_line(std::string& line) override;
    void reset() override;
    virtual ~file_block_reader() = default;
private:
    std::string m_path;
    std::ifstream m_filestream;
    unsigned long m_start;
    unsigned long m_length;
    unsigned long m_bytes_read = 0;
    bool m_is_eof = false;
};
