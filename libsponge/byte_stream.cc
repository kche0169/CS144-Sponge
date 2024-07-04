#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : buf_capacity(capacity), buf(), _bytes_read(0), _bytes_written(0), _eof(false), input_ended_signal(false), _error(false) { 
}

size_t ByteStream::write(const string &data) {
    size_t _data_size = size_t(data.size());
    size_t _temp_bytes_written = min(remaining_capacity(), _data_size);
    if (_data_size > remaining_capacity()) set_error();
    for (size_t i = 0; i < _temp_bytes_written; i ++) {
            buf.push_back(data[i]);
    }
    _bytes_written += _temp_bytes_written;
    // end_input();
    return _temp_bytes_written;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string _peek_data;
    // 考虑len合法与否的情况
    // 只访问小于len和buf长度的数据, 超过buffer长度的数据会读不出来
    auto real_len = min(len, buffer_size());
    for (size_t i = 0; i < real_len; i ++) {
        _peek_data += buf[i];
    }
    return _peek_data;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    // 我对这个函数的理解是
    // 如果长度合法，那么pop掉len长度的数据，反之pop掉所有内容
    auto valid_len = min(len, buffer_size());
    for (size_t i = 0; i < valid_len; i ++) {
        buf.pop_front();
    }
    _bytes_read += valid_len;
    
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    std::string read_srting = peek_output(len);
    pop_output(len);
    return read_srting;
}

void ByteStream::end_input() {
    // 有问题
    input_ended_signal = true;
}

bool ByteStream::input_ended() const {
    // 有问题
    return input_ended_signal;
}

size_t ByteStream::buffer_size() const { 
    return size_t(buf.size()); 
    
}

bool ByteStream::buffer_empty() const { 
    // true if the buffer is empty
    return (buffer_size() == 0) ? true : false; 
}

bool ByteStream::eof() const { 
    // 有问题
    return buffer_empty() && input_ended();

}

size_t ByteStream::bytes_written() const { 
    return _bytes_written; 
}

size_t ByteStream::bytes_read() const { 
    return _bytes_read; 
}

size_t ByteStream::remaining_capacity() const { 
    return buf_capacity - buffer_size(); 
}
