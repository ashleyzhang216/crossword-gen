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