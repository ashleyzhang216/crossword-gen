// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: test driver declaration for word_domain
// ==================================================================

#ifndef WORD_DOMAIN_TEST_DRIVER_H
#define WORD_DOMAIN_TEST_DRIVER_H

#include "../../src/common/common_data_types.h"
#include "../../src/common/common_parent.h"
#include "../../src/word_domain/word_domain_data_types.h"
#include "../../src/word_domain/word_domain.h"
#include "../../src/lib/src/json.hpp"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace word_domain_data_types_ns;
using namespace word_domain_ns;
using json = nlohmann::json;

namespace word_domain_test_driver_ns {
    class word_domain_test_driver : public common_parent {
        public:
            // base constructor
            word_domain_test_driver(string name);

            // constructor with filepath contents
            word_domain_test_driver(string name, string filepath);

            // basic directed test for trie & find_matches()
            bool test_trie_basic(string pattern, unordered_set<word_t>& ground_truth);

            // basic directed test for letters_at_indicies
            bool test_letters_at_indicies_add(
                vector<word_t> words,
                letter_idx_table<uint> initial_num_words,
                letter_idx_table<uint> initial_num_nodes,
                vector<letter_idx_table<uint> > num_words_ground_truths,
                vector<letter_idx_table<uint> > num_nodes_ground_truths
            );

            // basic test to check if alls row sums equal to word length for cw_variable use case
            bool test_letters_at_indicies_row_sums(
                letter_idx_table<uint> num_words,
                uint&& word_len, uint total_words
            );

            // basic directed test for letters_at_indicies state after calling removal func
            bool test_letters_at_indicies_remove(
                vector<word_t> init_words, vector<pair<uint, char> > remove_params,
                letter_idx_table<uint> initial_num_words,
                letter_idx_table<uint> initial_num_nodes,
                vector<letter_idx_table<uint> > num_words_ground_truths,
                vector<letter_idx_table<uint> > num_nodes_ground_truths
            );

            // directed test for letters_at_indicies state after remove calls and during assignment in an AC-3 step
            bool test_letters_at_indicies_remove_assign(
                vector<word_t> init_words, vector<pair<uint, char> > remove_params, optional<word_t> last_remaining, 
                letter_idx_table<uint> initial_num_words,
                letter_idx_table<uint> initial_num_nodes,
                vector<letter_idx_table<uint> > num_words_ground_truths,
                vector<letter_idx_table<uint> > num_nodes_ground_truths
            );

            // directed test for get_all_letters_at_index state after add, remove, and assign calls
            bool test_get_all_letters_at_index(
                vector<word_t> words, vector<pair<uint, char> > remove_params, 
                letter_idx_table<uint> initial_num_words
            );
            
            // basic check after remove tests that the domain is empty
            bool test_domain_empty() { return dut->size() == 0; }

            // basic directed test for num_letters_at_index() after assigning domain
            bool test_num_letters_at_indicies_assign(word_t value);

            // basic directed test for has_letters_at_index_with_letter_assigned()
            bool test_has_letters_at_index_with_letter_assigned(uint len);

            // expose basic functionalities for dut 
            void add_words(vector<word_t> words) { for(word_t w : words) dut->add_word(w); }
            void remove_words(vector<pair<uint, char> > remove_params) { 
                for(const auto& pair : remove_params) dut->remove_matching_words(pair.first, pair.second);
            }
            void assign_domain(word_t word) { dut->assign_domain(word); }
            void unassign_domain() { dut->unassign_domain(); }
            vector<word_t> get_cur_domain() { return dut->get_cur_domain(); }
            unordered_map<string, word_t>& get_word_map() { return dut->get_word_map(); }
            size_t remove_matching_words(uint index, char letter) { return dut->remove_matching_words(index, letter); }
            void start_new_ac3_call() { dut->start_new_ac3_call(); }
            size_t undo_prev_ac3_call() { return dut->undo_prev_ac3_call(); }

            // destructor, all objects are raii and should delete automatically
            ~word_domain_test_driver() = default;

        private:
            // design under test
            unique_ptr<word_domain> dut;

            // helper function for test_letters_at_indicies_add()
            bool letters_at_indicies_entries_equal(
                letter_idx_table<uint> expected,
                letter_idx_table<letters_table_entry> ground_truth,
                bool test_num_words
            );

            // helper function for test_get_all_letters_at_index()
            letter_bitset_t get_all_letters_at_index(
                uint index, letter_idx_table<uint>& num_words
            );
    }; // word_domain_test_driver
} // word_domain_test_driver_ns

#endif // WORD_DOMAIN_TEST_DRIVER_H