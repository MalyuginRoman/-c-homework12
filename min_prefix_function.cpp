#include "min_prefix_function.h"

std::pair<std::string, int> get_prefix_pair(int prefix_len, const std::string& str) {
    return std::make_pair<std::string, int>(str.substr(0, prefix_len), 1); }
std::function<std::pair<std::string, int>(const std::string&)> get_prefix_pair_function(int prefix_len) {
    return [prefix_len](const std::string& str){ return get_prefix_pair(prefix_len, str); }; }
void accumulate_key_sum(max_summator<std::string, int> & accum, const std::pair<std::string, int>& pair) {
    accum.add(pair); }
