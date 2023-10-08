#include "block_reader.h"

#include <sstream>

bool file_block_reader::get_next_line(std::string& line)
{
    if (!m_filestream.is_open() || m_is_eof || (m_bytes_read >= m_length)) {
        return false; }
    if (!std::getline(m_filestream, line)) {
        m_is_eof = true; }
    else {
        m_bytes_read += line.size() + 1; }
    return !m_is_eof && (m_bytes_read <= m_length);
}

void file_block_reader::reset()
{
    if (!m_filestream.is_open() || m_is_eof) {
        m_filestream.close();
        m_filestream.open(m_path); }
    m_filestream.seekg(m_start, m_filestream.beg);
    m_bytes_read = 0;
    m_is_eof = false;
}
