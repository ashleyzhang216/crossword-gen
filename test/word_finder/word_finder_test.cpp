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

    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_set-basic", "word_finder/data/dict_barebones.txt");
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
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_tree-basic", "word_finder/data/dict_barebones.txt");
    
    unordered_map<string, unordered_set<word_t> > test_cases = {
        {"ab", {word_t("ab")}},
        {"cat", {word_t("cat")}},
        {"hello", {word_t("hello")}},
        {"x", {}},
        {"?", {}},
        {"??", {word_t("ab")}},
        {"?????", {word_t("hello")}},
        {"hell", {}},
        {"ac", {}},
        {"???", {word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}},
        {"c?b", {word_t("cab"), word_t("cub")}},
        {"?u?", {word_t("cup"), word_t("cub")}},
        {"ca?", {word_t("cat"), word_t("cab"), word_t("can")}},
        {"??b", {word_t("cab"), word_t("cub"), word_t("dab")}},
        {"?a?", {word_t("cat"), word_t("cab"), word_t("can"), word_t("dan"), word_t("dab")}},
        {"ace", {word_t("ace")}},
        {"?ace???", {word_t("racecar")}},
    };

    for(auto& pair : test_cases) {
        REQUIRE(driver->test_word_tree_basic(pair.first, pair.second));
    }
}

/**
 * test to prune out words with invalid characters, convert uppercase to lowercase, & remove dashes
*/
TEST_CASE("word_finder parsing-invalid", "[word_finder],[quick]") {
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-parsing-invalid", "word_finder/data/words_top3000_with_invalids.txt");

    unordered_set<string> valid = {
        "african",
        "africanamerican",
        "am",
        "american",
        "email",
        "longterm",
        "mmhmm",
        "socalled",
        "aids",
        "arab",
        "asian",
        "bible",
        "ceo",
        "nt",
        "supervalid"
    };
    unordered_set<string> invalid = {
        "invålid",
        "super valid",
        "notaword!",
        "notaword",
        "dont@ddme",
        "{edgecase}",
        "n't",
        "African",
        "African-American",
        "AIDS",
        "AM",
        "American",
        "Arab",
        "Asian",
        "Bible",
        "CEO",
        "I", // below min length
        "i", // below min length
    };

    REQUIRE(driver->test_word_set_basic(valid, invalid));
}