#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>
#include <queue>
using namespace std;

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.
//    用這個來表示沒有整理好的字符串　一定要注意這些字符串中間會有斷層　這些字符串的順序是由index完全確定的
//    搞一個小根堆存放斷層
    priority_queue<pair<size_t ,string>,vector<pair<size_t ,string>>,greater<>> unassembled;
//    這個表示已經整理好的字符串
//    由於中間_output可能被讀，所以用這個記錄所有的被整理過得字符串
    string assembled;
    ByteStream _output;  //!< The reassembled in-order byte stream
//    注意這個capacity是指所有被整理過和沒有被整理過的字節字節數的上界
    size_t _capacity;    //!< The maximum number of bytes
//    這個表示已經整理過的字符串中的最大idx
    size_t assembled_maxIdx;
    bool hasEof;
    size_t eofIdx;
    string eofStr;

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded.
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
