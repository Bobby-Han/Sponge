#include "byte_stream.hh"
#include "fsm_stream_reassembler_harness.hh"
#include "stream_reassembler.hh"
#include "util.hh"

#include <exception>
#include <iostream>

using namespace std;

int main() {
    try {
        {
            ReassemblerTestHarness test{2};

            test.execute(SubmitSegment{"ab", 0});
            test.execute(BytesAssembled(2));
//            注意這個方法　首先是判斷緩存裏的字符串長度是不是參數字符串的長度
//            接着再判斷從緩存裏讀出來的字符串是不是參數字符串
            test.execute(BytesAvailable("ab"));

            test.execute(SubmitSegment{"cd", 2});
            test.execute(BytesAssembled(4));
            test.execute(BytesAvailable("cd"));

            test.execute(SubmitSegment{"ef", 4});
            test.execute(BytesAssembled(6));
            test.execute(BytesAvailable("ef"));
        }

        {
            ReassemblerTestHarness test{2};

            test.execute(SubmitSegment{"ab", 0});
            test.execute(BytesAssembled(2));

            test.execute(SubmitSegment{"cd", 2});
            test.execute(BytesAssembled(2));

            test.execute(BytesAvailable("ab"));
            test.execute(BytesAssembled(2));

            test.execute(SubmitSegment{"cd", 2});
            test.execute(BytesAssembled(4));

            test.execute(BytesAvailable("cd"));
        }

        {
            ReassemblerTestHarness test{2};

            test.execute(SubmitSegment{"bX", 1});
            test.execute(BytesAssembled(0));

            test.execute(SubmitSegment{"a", 0});
            test.execute(BytesAssembled(2));

            test.execute(BytesAvailable("ab"));
        }

        {
            ReassemblerTestHarness test{1};

            test.execute(SubmitSegment{"ab", 0});
            test.execute(BytesAssembled(1));

            test.execute(SubmitSegment{"ab", 0});
            test.execute(BytesAssembled(1));

            test.execute(BytesAvailable("a"));
            test.execute(BytesAssembled(1));

            test.execute(SubmitSegment{"abc", 0});
            test.execute(BytesAssembled(2));

            test.execute(BytesAvailable("b"));
            test.execute(BytesAssembled(2));
        }

        {
            ReassemblerTestHarness test{8};

            test.execute(SubmitSegment{"a", 0});
            test.execute(BytesAssembled(1));
            test.execute(BytesAvailable("a"));
            test.execute(NotAtEof{});

            test.execute(SubmitSegment{"bc", 1});
            test.execute(BytesAssembled(3));
            test.execute(NotAtEof{});

            test.execute(SubmitSegment{"ghi", 6}.with_eof(true));
            test.execute(BytesAssembled(3));
            test.execute(NotAtEof{});

            test.execute(SubmitSegment{"cdefg", 2});
            test.execute(BytesAssembled(9));
            test.execute(BytesAvailable{"bcdefghi"});
        }

        {
            ReassemblerTestHarness test{3};
            for (unsigned int i = 0; i < 99997; i += 3) {
                const string segment = {char(i), char(i + 1), char(i + 2), char(i + 13), char(i + 47), char(i + 9)};
                test.execute(SubmitSegment{segment, i});
                test.execute(BytesAssembled(i + 3));
                test.execute(BytesAvailable(segment.substr(0, 3)));
            }
        }

    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
