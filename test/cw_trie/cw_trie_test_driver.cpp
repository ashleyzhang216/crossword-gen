// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: test driver implementation for crossword
// ==================================================================

#include "cw_trie_test_driver.h"

using namespace cw_trie_test_driver_ns;

/**
 * @brief basic constructor for cw_trie test driver
 * 
 * @param name name of driver
*/
cw_trie_test_driver::cw_trie_test_driver(string name) : common_parent(name) {
    dut = make_unique<cw_trie>(name);
}

/**
 * @brief constructor for cw_trie test driver with initial contents
 * 
 * @param name name of driver
 * @param filepath path to .txt or .json file containing word data
*/
cw_trie_test_driver::cw_trie_test_driver(string name, string filepath) : common_parent(name) {
    dut = make_unique<cw_trie>(name, filepath);
}

/**
 * @brief basic directed cw_trie test for find_matches
 * 
 * @param pattern pattern to test against
 * @param ground_truth expected output of find_matches() from cw_trie
 * @return true iff successful
*/
bool cw_trie_test_driver::test_trie_basic(string pattern, unordered_set<word_t>& ground_truth) {
    unordered_set<word_t> result = dut->find_matches(pattern);
    return check_condition("find_matches for \"" + pattern + "\"", set_contents_equal(&ground_truth, &result, true));
}

/**
 * @brief basic directed cw_trie test for letters_at_indicies
 * 
 * @param words words to add to trie, in order. size must be equal as num_words_ground_truths, num_nodes_ground_truths
 * @param initial_num_words initial state of num_words in letters_at_indices
 * @param initial_num_nodes initial state of nodes.size() in letters_at_indices
 * @param num_words_ground_truths expected num_words after each call to add_word(), size must be equal as words, num_nodes_ground_truths
 * @param num_nodes_ground_truths expected size() of children nodes after each call to add_word(), size must be equal as num_words_ground_truths, words
 * @returns true iff letters_at_indices equal to expected at every step 
*/
bool cw_trie_test_driver::test_letters_at_indicies_add(
    vector<word_t> words,
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_words,
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_nodes,
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths,
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths
) {
    
    bool result = true;
    assert(words.size() == num_words_ground_truths.size()); 
    assert(num_nodes_ground_truths.size() == num_words_ground_truths.size()); 
    array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> letters_at_indices = dut->get_letters_at_indices();

    result &= check_condition("letters_at_indicies initial num_words", letters_at_indicies_entries_equal(initial_num_words, letters_at_indices, true));
    result &= check_condition("letters_at_indicies initial num nodes", letters_at_indicies_entries_equal(initial_num_nodes, letters_at_indices, false));

    for(uint i = 0; i < words.size(); i++) {
        dut->add_word(words[i]);
        letters_at_indices = dut->get_letters_at_indices();

        result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths[i], letters_at_indices, true));
        result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths[i], letters_at_indices, false));
    }

    return result;
}

/**
 * @brief check if all rows in num_words sum to total valid words for case where word length is equal
 * 
 * @param num_words state of num_words table
 * @param word_len rval ref length of all words added to num_words
 * @param total_words total valid (non duplicate) words in num_words
 * @returns true iff all row sums are as expected
*/
bool cw_trie_test_driver::test_letters_at_indicies_row_sums(
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words,
    uint&& word_len, uint total_words
) {
    bool result = true;
    for(uint i = 0; i < word_len; i++) {
        result &= check_condition("letters_at_indicies row sum", std::accumulate(std::begin(num_words[i]), std::end(num_words[i]), 0u) == total_words);
    }

    return result;
}

/**
 * @brief basic directed cw_trie test for letters_at_indicies after removal calls
 * 
 * @param init_words words to add at once before removal calls
 * @param remove_params words to remove, in order, size must be equal to num_words_ground_truths, num_nodes_ground_truths
 * @param initial_num_words initial state of num_words in letters_at_indices
 * @param initial_num_nodes initial state of nodes.size() in letters_at_indices
 * @param num_words_ground_truths expected num_words after each call to remove_matching_words(), size must be equal as remove_params, num_nodes_ground_truths
 * @param num_nodes_ground_truths expected size() of children nodes after each call to remove_matching_words(), size must be equal as num_words_ground_truths, remove_params
 * @returns true iff letters_at_indices equal to expected at every step 
*/
bool cw_trie_test_driver::test_letters_at_indicies_remove(
    vector<word_t> init_words, vector<pair<uint, char> > remove_params,
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_words,
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_nodes,
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths,
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths
) {
    bool result = true;
    assert(remove_params.size() == num_words_ground_truths.size()); 
    assert(num_nodes_ground_truths.size() == num_words_ground_truths.size()); 

    for(word_t word : init_words) dut->add_word(word);
    array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> letters_at_indices = dut->get_letters_at_indices();

    result &= check_condition("letters_at_indicies initial num_words", letters_at_indicies_entries_equal(initial_num_words, letters_at_indices, true));
    result &= check_condition("letters_at_indicies initial num nodes", letters_at_indicies_entries_equal(initial_num_nodes, letters_at_indices, false));

    const size_t init_domain_size = dut->get_word_map().size();
    size_t num_removed = 0;

    // iterate twice to ensure AC-3 restoration works properly
    for(uint iteration = 0; iteration < 2; iteration++) {
        for(uint i = 0; i < remove_params.size(); i++) {
            dut->start_new_ac3_call(); // simulate single AC-3 layer for each remove
            num_removed += dut->remove_matching_words(remove_params[i].first, remove_params[i].second);
            letters_at_indices = dut->get_letters_at_indices();

            result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths[i], letters_at_indices, true));
            result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths[i], letters_at_indices, false));
            result &= check_condition("domain size preserved during remove", init_domain_size == num_removed + dut->domain_size());
        }

        for(int i = static_cast<int>(remove_params.size()) - 2; i >= 0; i--) {
            num_removed -= dut->undo_prev_ac3_call();
            letters_at_indices = dut->get_letters_at_indices();

            result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths[static_cast<uint>(i)], letters_at_indices, true));
            result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths[static_cast<uint>(i)], letters_at_indices, false));
            result &= check_condition("domain size preserved during restore", init_domain_size == num_removed + dut->domain_size());
        }

        if(remove_params.size() > 0) {
            num_removed -= dut->undo_prev_ac3_call();
            letters_at_indices = dut->get_letters_at_indices();

            result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(initial_num_words, letters_at_indices, true));
            result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(initial_num_nodes, letters_at_indices, false));
            result &= check_condition("num_removed is 0 after undoing all removes", num_removed == 0);
            result &= check_condition("domain size preserved during restore to init", init_domain_size == dut->get_word_map().size());
        }
    }

    // remove again
    for(uint i = 0; i < remove_params.size(); i++) {
        dut->start_new_ac3_call(); // simulate single AC-3 layer for each remove
        num_removed += dut->remove_matching_words(remove_params[i].first, remove_params[i].second);
        letters_at_indices = dut->get_letters_at_indices();

        result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths[i], letters_at_indices, true));
        result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths[i], letters_at_indices, false));
        result &= check_condition("domain size preserved during remove", init_domain_size == num_removed + dut->domain_size());
    }

    return result;
}

/**
 * @brief directed test for letters_at_indicies state after remove calls and during assignment in an AC-3 step
 * 
 * @param init_words words to add at once before removal calls
 * @param remove_params words to remove, in order, size must be equal to num_words_ground_truths, num_nodes_ground_truths
 * @param last_remaining if has value, expected singular domain value remaining after removes
 * @param initial_num_words initial state of num_words in letters_at_indices
 * @param initial_num_nodes initial state of nodes.size() in letters_at_indices
 * @param num_words_ground_truths expected num_words after each call to remove_matching_words(), size must be equal as remove_params, num_nodes_ground_truths
 * @param num_nodes_ground_truths expected size() of children nodes after each call to remove_matching_words(), size must be equal as num_words_ground_truths, remove_params
 * @returns true iff letters_at_indices equal to expected at every step 
*/
bool cw_trie_test_driver::test_letters_at_indicies_remove_assign(
    vector<word_t> init_words, vector<pair<uint, char> > remove_params, optional<word_t> last_remaining, 
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_words,
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_nodes,
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths,
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths
) {
    bool result = true;
    assert(remove_params.size() == num_words_ground_truths.size()); 
    assert(num_nodes_ground_truths.size() == num_words_ground_truths.size()); 

    for(word_t word : init_words) dut->add_word(word);
    array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> letters_at_indices = dut->get_letters_at_indices();

    result &= check_condition("letters_at_indicies initial num_words", letters_at_indicies_entries_equal(initial_num_words, letters_at_indices, true));
    result &= check_condition("letters_at_indicies initial num nodes", letters_at_indicies_entries_equal(initial_num_nodes, letters_at_indices, false));

    const size_t init_domain_size = dut->get_word_map().size();
    size_t num_removed = 0;

    dut->start_new_ac3_call(); // simulate single AC-3 layer for all removes

    // execute removes
    for(uint i = 0; i < remove_params.size(); i++) {
        num_removed += dut->remove_matching_words(remove_params[i].first, remove_params[i].second);
        letters_at_indices = dut->get_letters_at_indices();

        result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths[i], letters_at_indices, true));
        result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths[i], letters_at_indices, false));
        result &= check_condition("domain size preserved during remove", init_domain_size == num_removed + dut->domain_size());
    }

    if(last_remaining.has_value()) {
        vector<word_t> remaining_domain = dut->get_cur_domain();
        result &= remaining_domain.size() == 1;
        result &= remaining_domain[0] == last_remaining.value();
    }
    
    dut->start_new_ac3_call(); // dummy blank layer
    dut->start_new_ac3_call(); // simulate single AC-3 layer for all assignments

    // try assigning candidates
    vector<word_t> candidates = dut->get_cur_domain();
    result &= candidates.size() == dut->domain_size();
    for(word_t candidate : candidates) {
        dut->assign_domain(candidate);
        result &= test_num_letters_at_indicies_assign(candidate);
        dut->remove_matching_words(0, candidate.word.at(0));
    }

    result &= dut->undo_prev_ac3_call() == 1; // real AC-3 layer
    result &= dut->undo_prev_ac3_call() == 0; // dummy AC-3 layer
    dut->unassign_domain();

    // pre-undo
    if(remove_params.size() > 0) {
        letters_at_indices = dut->get_letters_at_indices();

        result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths.back(), letters_at_indices, true));
        result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths.back(), letters_at_indices, false));
        result &= check_condition("domain size preserved during restore", init_domain_size == num_removed + dut->domain_size());
    }

    // final undo
    num_removed -= dut->undo_prev_ac3_call();
    letters_at_indices = dut->get_letters_at_indices();

    result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(initial_num_words, letters_at_indices, true));
    result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(initial_num_nodes, letters_at_indices, false));
    result &= check_condition("num_removed is 0 after undoing all removes", num_removed == 0);
    result &= check_condition("domain size preserved during restore to init", init_domain_size == dut->get_word_map().size());

    return result;
}

/**
 * @brief directed test for get_all_letters_at_index() return vals after add/remove calls and during assignment in multiple AC-3 steps
 * 
 * @param words words to add first
 * @param remove_params words to remove, in order
 * @param initial_num_words initial state of num_words in letters_at_indices
 * @returns true iff get_all_letters_at_index() returns expected value at every step
*/
bool cw_trie_test_driver::test_get_all_letters_at_index(
    vector<word_t> words, vector<pair<uint, char> > remove_params, 
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> initial_num_words
) {
    bool result = true;

    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words = initial_num_words;
    array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> letters_at_indices = dut->get_letters_at_indices();
    unordered_set<char> expected, actual;

    result &= check_condition("letters_at_indicies initial num_words", letters_at_indicies_entries_equal(initial_num_words, letters_at_indices, true));

    // check initial return result
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        expected = get_all_letters_at_index(i, num_words);
        actual = dut->get_all_letters_at_index(i);
        result &= set_contents_equal(&expected, &actual, false);
    }

    // add words
    for(word_t word : words) {
        
        dut->add_word(word);
        for(size_t i = 0; i < word.word.size(); i++) {
            num_words[i][static_cast<size_t>(word.word.at(i) - 'a')] += 1;
        }

        letters_at_indices = dut->get_letters_at_indices();
        result &= check_condition("letters_at_indicies num_words add", letters_at_indicies_entries_equal(num_words, letters_at_indices, true));

        // check return result
        for(uint i = 0; i < MAX_WORD_LEN; i++) {
            expected = get_all_letters_at_index(i, num_words);
            actual = dut->get_all_letters_at_index(i);
            result &= set_contents_equal(&expected, &actual, false);
        }
    }

    dut->start_new_ac3_call(); // simulate single AC-3 layer for all assignments

    // try assigning candidates
    vector<word_t> candidates = dut->get_cur_domain();
    result &= candidates.size() == dut->domain_size();
    for(word_t candidate : candidates) {
        dut->assign_domain(candidate);
        
        // check return result
        for(uint i = 0; i < candidate.word.size(); i++) {
            expected = { candidate.word.at(i) };
            actual = dut->get_all_letters_at_index(i);
            result &= set_contents_equal(&expected, &actual, false);
        }

        dut->remove_matching_words(0, candidate.word.at(0));
    }

    result &= dut->undo_prev_ac3_call() == 1; 
    dut->unassign_domain();

    unordered_set<word_t> words_remaining(words.begin(), words.end());

    dut->start_new_ac3_call(); // simulate single AC-3 layer for all removes

    // execute removes
    for(uint i = 0; i < remove_params.size(); i++) {
        
        unordered_set<word_t> removed;
        for(word_t word : words_remaining) {
            if(word.word.at(remove_params[i].first) == remove_params[i].second) {
                removed.insert(word);
                for(uint j = 0; j < word.word.size(); j++) {
                    num_words[j][static_cast<size_t>(word.word.at(j) - 'a')] -= 1;
                }
            }
        }
        for(word_t word : removed) words_remaining.erase(word);

        dut->remove_matching_words(remove_params[i].first, remove_params[i].second);
        letters_at_indices = dut->get_letters_at_indices();
        result &= check_condition("letters_at_indicies num_words remove", letters_at_indicies_entries_equal(num_words, letters_at_indices, true));

        // check return result
        for(uint j = 0; j < MAX_WORD_LEN; j++) {
            expected = get_all_letters_at_index(j, num_words);
            actual = dut->get_all_letters_at_index(j);
            result &= set_contents_equal(&expected, &actual, false);
        }
    }

    // try assigning candidates, again
    candidates = dut->get_cur_domain();
    result &= candidates.size() == dut->domain_size();
    for(word_t candidate : candidates) {
        dut->assign_domain(candidate);
        
        // check return result
        for(uint i = 0; i < candidate.word.size(); i++) {
            expected = { candidate.word.at(i) };
            actual = dut->get_all_letters_at_index(i);
            result &= set_contents_equal(&expected, &actual, false);
        }

        dut->remove_matching_words(0, candidate.word.at(0));
    }

    return result;
}

/**
 * @brief checks that num_letters_at_index() is consistent with assigning a specific word as the domain
 * @note does not actually assign the domain 
 * 
 * @param value word to assign as the new domain
 * @returns true iff num_letters_at_index() works correctly
*/
bool cw_trie_test_driver::test_num_letters_at_indicies_assign(word_t value) {
    bool result = true;

    uint expected_val;
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(char c = 'a'; c <= 'z'; c++) {
            expected_val = (i < value.word.size() && value.word.at(i) == c) ? 1l : 0l; 
            result &= dut->num_letters_at_index(i, c) == expected_val;
        }
    }

    return result;
}

/**
 * @brief checks if each entry's num_words or children.size() are equal
 * 
 * @param expected expected array
 * @param ground_truth ground truth letters_at_indices
 * @param test_num_words true -> test num_words, false -> test nodes.size() in ground_truth
 * @returns true iff arrays are equal
*/
bool cw_trie_test_driver::letters_at_indicies_entries_equal(
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> expected,
    array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> ground_truth,
    bool test_num_words
) {
    bool result = true;

    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            if(
                (test_num_words  && expected[i][j] != ground_truth[i][j].num_words) || 
                (!test_num_words && expected[i][j] != ground_truth[i][j].nodes.size())
            ) {
                ss << "letters_at_indicies_entries_equal() inequal at index: " << i << ", letter: " << j << ", test_num_words: " << test_num_words 
                   << ", expected: " << expected[i][j] << ", actual nodes: " << ground_truth[i][j].nodes.size() << ", actual words: " << ground_truth[i][j].num_words;
                utils->print_msg(&ss, WARNING);
                result = false;
            }
        }
    }
    return result;
}

/**
 * @brief helper for test_get_all_letters_at_index() to generate expected result
 * 
 * @param index the index of the table to consider
 * @param num_words ref to num word table to generate result from
 * @returns set of all letters with at least 1 appearance at specified index
 * 
*/
unordered_set<char> cw_trie_test_driver::get_all_letters_at_index(uint index, array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN>& num_words) {
    assert_m(index < MAX_WORD_LEN, "index exceeds table dimension");

    unordered_set<char> expected;
    for(uint i = 0; i < NUM_ENGLISH_LETTERS; i++) {
        if(num_words[index][i] > 0) {
            expected.insert(static_cast<char>('a' + i));
        }
    }
    return expected;
}