#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;
// initialize
ByteStream::ByteStream(size_t capacity): buffer_capacity(capacity),_buffer(),written_size(0),read_size(0),_end_input(false){}

size_t ByteStream::write(const string &data) {
    if(eof()){
        return 0;
    }
//    得到需要寫入的長度
    size_t write_len=min(data.length(), buffer_capacity-_buffer.size());
//    寫入數據
    for(size_t i=0;i<write_len;i++){
        _buffer.push_back(data[i]);
    }
//    注意還需要更新一下寫入的長度
    written_size+=write_len;
//    最後返回
    return write_len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len)  {
//    首先得到真正需要顯示的長度
    size_t dataLen=min(len,_buffer.size());
    deque<char> tmp;
    string ans;
    for(size_t i=0;i<dataLen;i++){
        auto t=_buffer.front();
        ans.push_back(t);
        tmp.push_front(t);
        _buffer.pop_front();
    }
    while (!tmp.empty()){
        auto t=tmp.front();
        _buffer.push_front(t);
        tmp.pop_front();
    }
    return ans;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
//    buffer的前面的部分會被pop掉
//    獲取真正的len
    size_t popLen=min(len,_buffer.size());
    read_size+=popLen;
    for(size_t i=0;i<popLen;i++){
        _buffer.pop_front();
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string data= peek_output(len);
    pop_output(len);
    return data;
}
// 設置整個字符流已經到結尾了
void ByteStream::end_input() {_end_input= true;}

bool ByteStream::input_ended() const { return _end_input; }

size_t ByteStream::buffer_size() const { return _buffer.size(); }

bool ByteStream::buffer_empty() const { return _buffer.empty(); }

bool ByteStream::eof() const { return _end_input && buffer_empty(); }

size_t ByteStream::bytes_written() const { return written_size; }

size_t ByteStream::bytes_read() const { return read_size; }

size_t ByteStream::remaining_capacity() const { return buffer_capacity -_buffer.size(); }
