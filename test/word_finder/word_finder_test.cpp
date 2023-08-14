// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: Catch2 tests for word_finder class
// ==================================================================

#include "../catch/catch.hpp"
#include "word_finder_test_driver.h"

using namespace word_finder_test_driver_ns;

/**
 * hello world test for word_finder
*/
TEST_CASE("word_finder hello_world", "[word_finder],[hello_world],[quick]") {
    REQUIRE(true);
}

/**
 * basic test to check word_set for barebone dictionary
*/
TEST_CASE("word_finder word_set-basic", "[word_finder],[quick]") {

    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_set-basic", "data/dict_barebones.txt");
    unordered_set<string> valid = {
        "ab",
        "ace",
        "and",
        "cat",
        "cab",
        "can",
        "cup",
        "cub",
        "dan",
        "dab",
        "hello"
    };
    unordered_set<string> invalid = {
        "french",
        "cap",
        "hellooo"
        "an",
        "a"
    };

    REQUIRE(driver->test_word_set_basic(valid, invalid));
}

/**
 * basic test to check word_set for barebone dictionary
*/
TEST_CASE("word_finder word_tree-basic", "[word_finder],[quick]") {
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_set-basic", "data/dict_barebones.txt");
    
    unordered_map<string, unordered_set<string> > test_cases = {
        {"ab", {"ab"}},
        {"cat", {"cat"}},
        {"hello", {"hello"}},
        {"x", {}},
        {"?", {}},
        {"??", {"ab"}},
        {"?????", {"hello"}},
        {"hell", {}},
        {"ac", {}},
        {"???", {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}},
        {"c?b", {"cab", "cub"}},
        {"?u?", {"cup", "cub"}},
        {"ca?", {"cat", "cab", "can"}},
        {"??b", {"cab", "cub", "dab"}},
        {"?a?", {"cat", "cab", "can", "dan", "dab"}},
        {"ace", {"ace"}},
        {"?ace???", {"racecar"}},
    };

    for(auto& pair : test_cases) {
        REQUIRE(driver->test_word_tree_basic(pair.first, pair.second));
    }
}