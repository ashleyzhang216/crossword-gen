// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: test driver declaration for word_finder
// ==================================================================

#include "../../src/common/common_data_types.h"
#include "../../src/common/common_parent.h"
#include "../../src/word_finder/word_finder.h"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace word_finder_ns;

namespace word_finder_test_driver_ns {
    class word_finder_test_driver : public common_parent {
        public:
            // base constructor
            word_finder_test_driver(string name, string filepath);

            // basic directed test for word_set
            bool test_word_set_basic(unordered_set<string> valid, unordered_set<string> invalid);

            // basic directed test for word_tree
            bool test_word_tree_basic(string pattern, unordered_set<string> ground_truth);

        private:
            word_finder* dut;
    }; // word_finder_test_driver
} // word_finder_test_driver_ns