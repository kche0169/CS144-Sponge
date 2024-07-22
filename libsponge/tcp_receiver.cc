#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader &_TCPHeader = seg.header();
    if (!_TCPHeader.syn && _connect_status == ConnectStatus::Listen) {
        return;
    } else if (_TCPHeader.syn && _connect_status == ConnectStatus::Listen) {
        _connect_status = ConnectStatus::Open;
        _isn = _TCPHeader.seqno;
    } else if (_TCPHeader.syn && _connect_status != ConnectStatus::Listen) {
        return;
    } else if (!_TCPHeader.syn && _connect_status != ConnectStatus::Listen && _isn == _TCPHeader.seqno) {
        return;
    }

    _checkpoint = _reassembler.stream_out().bytes_written() + 1;
    uint64_t abs_seq = unwrap(_TCPHeader.seqno, _isn, _checkpoint);
    uint64_t stream_index = abs_seq - 1 + (_TCPHeader.syn ? 1 : 0);


    _reassembler.push_substring(seg.payload().copy(), stream_index, _TCPHeader.fin);

    if (_reassembler.stream_out().input_ended()) {
        _connect_status = ConnectStatus::Closed;
    }
    
    
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    switch (_connect_status)
    {
    case ConnectStatus::Listen:
        return nullopt;
    case ConnectStatus::Open:
        return wrap(_reassembler.stream_out().bytes_written() + 1, _isn);
    case ConnectStatus::Closed:
        return wrap(_reassembler.stream_out().bytes_written() + 2, _isn);
    }
    return std::nullopt; 
}

size_t TCPReceiver::window_size() const {
    return _capacity - _reassembler.stream_out().buffer_size(); 

}
