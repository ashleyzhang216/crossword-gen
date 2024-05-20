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
            cw_trie_test_driver(string name);

            // constructor with initial contents
            cw_trie_test_driver(string name, string filepath);

            // basic directed test for trie & find_matches()
            bool test_trie_basic(string pattern, unordered_set<word_t>& ground_truth);

            // basic directed test for letters_at_indicies
            bool test_letters_at_indicies_add(
                vector<word_t> words,
                array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_words,
                array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_nodes,
                vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths,
                vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths
            );

            // basic test to check if alls row sums equal to word length for cw_variable use case
            bool test_letters_at_indicies_row_sums(
                array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words,
                uint&& word_len, uint total_words
            );

            // basic directed test for letters_at_indicies state after calling removal func
            bool test_letters_at_indicies_remove(
                vector<word_t> init_words, vector<pair<uint, char> > remove_params,
                array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_words,
                array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_nodes,
                vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths,
                vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths
            );
            
            // basic check after remove tests that word_map is empty
            bool test_word_map_empty() { return dut->get_word_map().size() == 0; }

            // basic directed test for num_letters_at_index() after assigning domain
            bool test_num_letters_at_indicies_assign(word_t value);

            // expose basic functionalities for dut 
            void add_words(vector<word_t> words) { for(word_t w : words) dut->add_word(w); }
            void remove_words(shared_ptr<unordered_set<word_t> > pruned_words, vector<pair<uint, char> > remove_params) { 
                for(const auto& pair : remove_params) dut->remove_matching_words(pruned_words, pair.first, pair.second);
            }
            void assign_domain(word_t word) { dut->assign_domain(word); }
            void unassign_domain() { dut->unassign_domain(); }

            // destructor, all objects are raii and should delete automatically
            ~cw_trie_test_driver() = default;

        private:
            // design under test
            unique_ptr<cw_trie> dut;

            // helper function for test_letters_at_indicies_add()
            bool letters_at_indicies_entries_equal(
                array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> expected,
                array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> ground_truth,
                bool test_num_words
            );
    }; // cw_trie_test_driver
} // cw_trie_test_driver_ns