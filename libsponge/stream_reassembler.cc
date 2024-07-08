#include "stream_reassembler.hh"
#include <optional>
#include <iostream>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), 
                                                            _capacity(capacity), _bytes_container({}), 
                                                            _current_index(0), _eof_index(0), _expected_string_index(numeric_limits<uint64_t>::max()), 
                                                            _eof(false), _unassembled_bytes(0) {
    
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    
    // 最简单的做法
    // 用一个map一个序号存一个字符
    
    // const size_t container_size = _bytes_container.size();
    const size_t data_size = data.size();

    if (data_size == 0 && !eof) return;

    // 计算预期字符串的大小，仅在 eof 为 true 时更新
    if (eof) {
        _eof = true;
        _expected_string_index = index + data_size;
    }

    // 扩展 _bytes_container 的大小以容纳新数据
    size_t str_end = max(index + data_size, _bytes_container.size());
    if (str_end > _bytes_container.size()) {
        _bytes_container.resize(str_end, make_pair('\0', Status::PENDING));
    }
  
    // 将 data 中的数据复制到 _bytes_container 中相应位置
    auto start = max(index, _current_index);
    auto end = min(index + data_size, min( _current_index + _output.remaining_capacity(), _expected_string_index));
    for (size_t i = start, j = start - index; i < end; ++i, ++j) {
        if (_bytes_container[i].second == Status::PENDING) {
            _bytes_container[i].first = data[j];
            _bytes_container[i].second = Status::UNASSEMBLED;
            _unassembled_bytes++;
        }
    }

    // 从 _current_index 开始构建输入字符串 _input
    std::string input;
    // while (_current_index < _expected_string_index && _bytes_container[_current_index].second != true) 
    
    while (_current_index < _expected_string_index) {
        if (_bytes_container[_current_index].second == Status::UNASSEMBLED) {
            input.push_back(_bytes_container[_current_index].first);
            _bytes_container[_current_index] = {0, Status::ASSEMBLED};
        } else {
            break;
        }
        _unassembled_bytes --;
        _current_index ++;
    }
    _output.write(input);
    


    // 如果是 EOF 并且当前索引达到预期字符串大小，结束输入流
    if (_expected_string_index == _current_index) {
        _output.end_input();
        // _current_index = 0;  // 重置当前索引，准备下一轮处理

    }
}

size_t StreamReassembler::unassembled_bytes() const {

    return _unassembled_bytes; 
}

bool StreamReassembler::empty() const { 
    return unassembled_bytes() == 0; 
}
