#include "stream_reassembler.hh"
// Dummy implementation of a stream reassembler.
#include "algorithm"
// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;
// 初始化
StreamReassembler::StreamReassembler(const size_t capacity) : unassembled(),assembled(),_output(capacity), _capacity(capacity), assembled_maxIdx(0),hasEof(false),eofStr(){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
//    怎樣判斷eof是一個問題
    if(eof){
        hasEof= true;
        eofStr=data;
    }
    if(index<=assembled_maxIdx){
//        正好接在後面
        string assemble_data;
        if(index==assembled_maxIdx){
//            第一步．寫進去不能超過buffer的大小
            assemble_data=data.substr(0, min(_capacity-_output.buffer_size(),data.size()));
        } else{
//            idx有重復的情況
            size_t duplicate_num=assembled_maxIdx-index;
            if(data.size()<=duplicate_num){
                assemble_data="";
            } else{
                assemble_data=data.substr(duplicate_num,min(data.size()-duplicate_num,_capacity-_output.buffer_size()));
            }
        }
//        更新一下assembled_maxIdx
        assembled_maxIdx+=assemble_data.size();
        assembled+=assemble_data;
        _output.write(assemble_data);
//        當斷層裏面有字符串時
        if (!unassembled.empty()){
            auto t=unassembled.top();
            while (t.first<=assembled_maxIdx){
                if(t.first==assembled_maxIdx){
                    assemble_data=t.second.substr(0,min(_capacity-_output.buffer_size(),t.second.size()));
                } else{
                    size_t duplicate_num=assembled_maxIdx-t.first;
                    if(t.second.size()<=duplicate_num){
                        assemble_data="";
                    } else{
                        assemble_data=t.second.substr(duplicate_num,min(t.second.size()-duplicate_num,_capacity-_output.buffer_size()));
                    }
                }
                assembled_maxIdx+=assemble_data.size();
                assembled+=assemble_data;
                _output.write(assemble_data);
                unassembled.pop();
                if(!unassembled.empty()){
                    t=unassembled.top();
                } else{
                    break;
                }
            }
        }
        if(hasEof && (eofStr.empty() || eofStr[eofStr.size()-1]==assembled[assembled_maxIdx-1])){
            _output.end_input();
        }
    } else{
//        如果是一個斷層data
//        直接放到小根堆裏
        unassembled.push({index,data.substr(0, min(_capacity,data.size()))});
    }

}
size_t StreamReassembler::unassembled_bytes() const {
    if(unassembled.empty()){
        return 0;
    } else{
        priority_queue<pair<size_t ,string>,vector<pair<size_t ,string>>,greater<>> unassembled_cpy=unassembled;
        vector<char> unassemble_char;
        while (!unassembled_cpy.empty()){
            auto t=unassembled_cpy.top();
            for(auto ele:t.second){
                unassemble_char.push_back(ele);
            }
            unassembled_cpy.pop();
        }
        sort(unassemble_char.begin(),unassemble_char.end());
        size_t m= unique(unassemble_char.begin(),unassemble_char.end())-unassemble_char.begin();
        return m;
    }
}
bool StreamReassembler::empty() const { return unassembled.empty(); }
