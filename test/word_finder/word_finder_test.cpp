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
 * basic test to check word_set for barebone dictionary for txt files
*/
TEST_CASE("word_finder word_set-basic_txt", "[word_finder],[quick]") {

    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_set-basic_txt", "word_finder/data/dict_barebones.txt");
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
 * basic test to check word_set for barebone dictionary for json files
*/
TEST_CASE("word_finder word_set-basic_json", "[word_finder],[quick]") {

    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_set-basic_json", "word_finder/data/data_small.json");
    unordered_set<string> valid = {
        "can",
        "cup",
        "but",
        "food",
        "search",
        "fursona",
        "jackandthebeanstalk"
    };
    unordered_set<string> invalid = {
        "butt",
        "french",
        "cap",
        "hello",
        "hellooo"
        "a",
        "fitnessgrampacertest",
        "denverinternationalairport" // valid but above max word len
    };

    REQUIRE(driver->test_word_set_basic(valid, invalid));
}

/**
 * basic test to check word_set for barebone dictionary for txt files
*/
TEST_CASE("word_finder word_tree-basic_txt", "[word_finder],[quick]") {
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_tree-basic_txt", "word_finder/data/dict_barebones.txt");
    
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
 * basic test to check word_set for barebone dictionary for json files
*/
TEST_CASE("word_finder word_tree-basic_json", "[word_finder],[quick]") {
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-word_tree-basic_json", "word_finder/data/data_small.json");
    
    unordered_map<string, unordered_set<word_t> > test_cases = {
        {"ab", {}},
        {"cat", {}},
        {"hello", {}},
        {"x", {}},
        {"?", {}},
        {"??", {word_t("in"), word_t("is"), word_t("on"), word_t("by"), word_t("it"), word_t("or"), word_t("be"), word_t("at"), word_t("as"), word_t("an"), word_t("us"), word_t("do"), word_t("no")}},
        {"h????", {word_t("house"), word_t("hours"), word_t("human")}},
        {"hell", {}},
        {"ac", {}},
        {"?h?", {word_t("the"), word_t("who"), word_t("she"), word_t("why")}},
        {"c?b", {}},
        {"?u?", {word_t("but"), word_t("our"), word_t("out"), word_t("buy"), word_t("sun"), word_t("run"), word_t("fun"), word_t("due"), word_t("cut"), word_t("guy"), word_t("cup"), word_t("bus"), word_t("bug")}},
        {"ca?", {word_t("can")}},
        {"??b", {word_t("web"), word_t("job"), word_t("lab")}},
        {"ac?", {word_t("act")}},
        {"?????y????", {word_t("goodbyehug"), word_t("gentlyused"), word_t("icephysics")}},
    };

    for(auto& pair : test_cases) {
        REQUIRE(driver->test_word_tree_basic(pair.first, pair.second));
    }
}

/**
 * test to prune out words with invalid characters, convert uppercase to lowercase, & remove dashes for txt files
*/
TEST_CASE("word_finder parsing-invalid_txt", "[word_finder],[quick]") {
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-parsing-invalid_txt", "word_finder/data/words_top3000_with_invalids.txt");

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

/**
 * test to prune out words with invalid characters, convert uppercase to lowercase, & remove dashes for json files
*/
TEST_CASE("word_finder parsing-invalid_json", "[word_finder],[quick]") {
    word_finder_test_driver *driver = new word_finder_test_driver("word_finder_test_driver-parsing-invalid_json", "word_finder/data/data_small.json");

    unordered_set<string> valid = {
        // no strict test cases here since incoming json is guarenteed to be clean (all lowercase and alphabetical)
        "human",
        "email",
        "in",
        "is",
        "selfdestructsequence"
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
        "slay",
        "I", // below min length
        "i", // below min length
        "lovecraftiannightmare", // above max len
        "londonbridgeisfallingdown", // above max len
        "anamericantailfievelgoeswest" // above max len
    };

    REQUIRE(driver->test_word_set_basic(valid, invalid));
}