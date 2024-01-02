// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: test driver declaration for cw_trie
// ==================================================================

#include "../../src/common/common_data_types.h"
#include "../../src/common/common_parent.h"
#include "../../src/cw_trie/cw_trie_data_types.h"
#include "../../src/cw_trie/cw_trie.h"
#include "../../src/lib/src/json.hpp"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace cw_trie_data_types_ns;
using namespace cw_trie_ns;
using json = nlohmann::json;

namespace cw_trie_test_driver_ns {
    class cw_trie_test_driver : public common_parent {
        public:
            // base constructor
            cw_trie_test_driver(string name, string filepath);

            // basic directed test for trie & find_matches()
            bool test_trie_basic(string pattern, unordered_set<word_t>& ground_truth);

            // TODO: add basic directed test for letters_at_indicies

            // destructor, all objects are raii and should delete automatically
            ~cw_trie_test_driver() = default;

        private:
            // design under test
            unique_ptr<cw_trie> dut;
    }; // cw_trie_test_driver
} // cw_trie_test_driver_ns