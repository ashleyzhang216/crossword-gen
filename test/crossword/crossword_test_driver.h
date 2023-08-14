// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: test driver declaration for crossword
// ==================================================================

#include "../../src/common/common_data_types.h"
#include "../../src/common/common_parent.h"
#include "../../src/crossword/crossword_data_types.h"
#include "../../src/crossword/crossword.h"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace crossword_data_types_ns;
using namespace crossword_ns;

namespace crossword_test_driver_ns {
    class crossword_test_driver : public common_parent {
        public:
            // base constructor
            crossword_test_driver(string name);

            // cw constructor test w/o contents
            bool test_constructor_empty(uint length, uint height, string ground_truth);

            // cw constructor test with contents in vector & string forms
            bool test_constructor_contents_vector(uint length, uint height, vector<vector<char> > contents, string ground_truth);
            bool test_constructor_contents_string(uint length, uint height, string contents,                string ground_truth);

            // destructor
            ~crossword_test_driver() { delete dut; }

        private:
            // TODO: change to use unique_ptr
            crossword* dut;
    }; // crossword_test_driver
} // crossword_test_driver_ns