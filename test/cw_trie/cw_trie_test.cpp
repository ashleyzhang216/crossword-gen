// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: Catch2 tests for cw_trie class
// ==================================================================

#include "../catch/catch.hpp"
#include "cw_trie_test_driver.h"

using namespace cw_trie_test_driver_ns;

/**
 * hello world test for cw_trie
*/
TEST_CASE("cw_trie hello_world", "[cw_trie],[hello_world],[quick]") {
    REQUIRE(true);
}

/**
 * basic test to check trie and find_matches() for barebone dictionary for json files
*/
TEST_CASE("cw_trie trie-basic_json", "[cw_trie],[quick]") {
    shared_ptr<cw_trie_test_driver> driver = make_shared<cw_trie_test_driver>("cw_trie_test_driver-trie-basic_json", "cw_trie/data/data_small.json");
    
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
        REQUIRE(driver->test_trie_basic(pair.first, pair.second));
    }
}