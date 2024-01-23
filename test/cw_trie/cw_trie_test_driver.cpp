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
 * @param filepath json files only
*/
cw_trie_test_driver::cw_trie_test_driver(string name, string filepath) : common_parent(name) {
    dut = make_unique<cw_trie>(name);

    // open word file, parse data
    ifstream word_file(filepath);
    assert_m(word_file.is_open(), "could not open file " + filepath);
    json j = json::parse(word_file);

    string word;
    for(const auto& [item, data] : j.items()) {
        // no need for parse_word() since incoming json is guarenteed to be clean, besides for word length (all lowercase and alphabetical)
        word = item;

        if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN) {
            dut->add_word(word_t(word, data["Score"], data["Frequency"]));
        } 
    }
    word_file.close();
}

/**
 * @brief basic directed cw_trie test for find_matches
 * 
 * @param pattern pattern to test against
 * @param ground_truth expected output of find_matches() from cw_trie
 * @return true iff successful
*/
bool cw_trie_test_driver::test_trie_basic(string pattern, unordered_set<word_t>& ground_truth) {
    shared_ptr<unordered_set<word_t> > result = make_shared<unordered_set<word_t> >();
    dut->find_matches(result, pattern);

    ss << "actual output: ";
    for(word_t w : *result) ss << w.word << ", ";
    utils->print_msg(&ss, DEBUG);

    return check_condition("find_matches for \"" + pattern + "\"", set_contents_equal(&ground_truth, &(*result), true));
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
bool cw_trie_test_driver::test_letters_at_indicies_basic(
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
 * @param total_words rval ref to total valid (non duplicate) words in num_words
 * @returns true iff all row sums are as expected
*/
bool cw_trie_test_driver::test_letters_at_indicies_row_sums(
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words,
    uint&& word_len, uint&& total_words
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

    shared_ptr<unordered_set<word_t> > pruned_words = make_shared<unordered_set<word_t> >();
    const size_t init_word_map_size = dut->get_word_map().size();
    for(uint i = 0; i < remove_params.size(); i++) {
        dut->remove_matching_words(pruned_words, remove_params[i].first, remove_params[i].second);
        letters_at_indices = dut->get_letters_at_indices();

        result &= check_condition("letters_at_indicies num_words", letters_at_indicies_entries_equal(num_words_ground_truths[i], letters_at_indices, true));
        result &= check_condition("letters_at_indicies num nodes", letters_at_indicies_entries_equal(num_nodes_ground_truths[i], letters_at_indices, false));
        result &= check_condition("word_map size preserved", init_word_map_size == pruned_words->size() + dut->get_word_map().size());
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