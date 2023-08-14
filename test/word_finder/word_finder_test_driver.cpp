// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: test driver implementation for word_finder
// ==================================================================

#include "word_finder_test_driver.h"

using namespace word_finder_test_driver_ns;

/**
 * @brief basic constructor for word_finder test driver
*/
word_finder_test_driver::word_finder_test_driver(string name, string filepath) : common_parent(name) {
    dut = new word_finder(name, filepath);
}

/**
 * @brief basic directed word_finder test for word_set for a barebones dictionary
 * 
 * @param valid set of words included in file, i.e. valid
 * @param invalid set of some words not included in file, i.e. invalid
 * @return true iff successful
*/
bool word_finder_test_driver::test_word_set_basic(unordered_set<string> valid, unordered_set<string> invalid) {
    bool result = true;

    // check that word_set populated correctly
    for(string s : valid) {
        result &= check_condition(s + " valid", dut->is_word(s));
    }
    for(string s : invalid) {
        result &= check_condition(s + " invalid", !dut->is_word(s));
    }

    return result;
}

/**
 * @brief basic directed word_finder test for word_tree for a barebones dictionary
 * 
 * @param pattern pattern to test against
 * @param ground_truth expected output of find_matches() from word_finder
 * @return true iff successful
*/
bool word_finder_test_driver::test_word_tree_basic(string pattern, unordered_set<string> ground_truth) {
    unordered_set<string> result;
    dut->find_matches(&result, pattern);
    
    ss << "actual output: ";
    for(string s : result) ss << s << ", ";
    utils->print_msg(&ss, DEBUG);

    return check_condition("find_matches for \"" + pattern + "\"", string_sets_equal(&ground_truth, &result));
}