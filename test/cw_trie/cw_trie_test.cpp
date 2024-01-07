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

/**
 * basic test to check letters_at_indicies
*/
TEST_CASE("cw_trie letters_at_indicies-same_len", "[cw_trie],[quick]") {
    shared_ptr<cw_trie_test_driver> driver = make_shared<cw_trie_test_driver>("cw_trie_test_driver-letters_at_indicies-same_len");
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> init_words;
    //               a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
    init_words.fill({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> init_nodes = init_words;

    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words = init_words;
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_nodes = init_nodes;

    vector<word_t> words;
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_truth;
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_truth;

    uint num_duplicates = 0;

    words.push_back(word_t("aaaaa"));
    num_words[0][0]++; num_words[1][0]++; num_words[2][0]++; num_words[3][0]++; num_words[4][0]++; 
    num_nodes[0][0]++; num_nodes[1][0]++; num_nodes[2][0]++; num_nodes[3][0]++; num_nodes[4][0]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("aaaaa")); // duplicates should do nothing
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);
    num_duplicates++;

    words.push_back(word_t("aabaa"));
    num_words[0][0]++; num_words[1][0]++; num_words[2][1]++; num_words[3][0]++; num_words[4][0]++; 
                                          num_nodes[2][1]++; num_nodes[3][0]++; num_nodes[4][0]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("abcda"));
    num_words[0][0]++; num_words[1][1]++; num_words[2][2]++; num_words[3][3]++; num_words[4][0]++; 
                       num_nodes[1][1]++; num_nodes[2][2]++; num_nodes[3][3]++; num_nodes[4][0]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("bbcda"));
    num_words[0][1]++; num_words[1][1]++; num_words[2][2]++; num_words[3][3]++; num_words[4][0]++; 
    num_nodes[0][1]++; num_nodes[1][1]++; num_nodes[2][2]++; num_nodes[3][3]++; num_nodes[4][0]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("abbaa"));
    num_words[0][0]++; num_words[1][1]++; num_words[2][1]++; num_words[3][0]++; num_words[4][0]++; 
                                          num_nodes[2][1]++; num_nodes[3][0]++; num_nodes[4][0]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("abbac"));
    num_words[0][0]++; num_words[1][1]++; num_words[2][1]++; num_words[3][0]++; num_words[4][2]++; 
                                                                                num_nodes[4][2]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("cbbac"));
    num_words[0][2]++; num_words[1][1]++; num_words[2][1]++; num_words[3][0]++; num_words[4][2]++; 
    num_nodes[0][2]++; num_nodes[1][1]++; num_nodes[2][1]++; num_nodes[3][0]++; num_nodes[4][2]++; 
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);

    words.push_back(word_t("cbbac")); // duplicates should do nothing
    num_words_truth.push_back(num_words);
    num_nodes_truth.push_back(num_nodes);
    num_duplicates++;

    REQUIRE(driver->test_letters_at_indicies_basic(words, init_words, init_nodes, num_words_truth, num_nodes_truth));

    const uint word_len = 5;
    bool row_sum_equal = true;
    for(uint i = 0; i < word_len; i++) {
        row_sum_equal &= (std::accumulate(std::begin(num_words[i]), std::end(num_words[i]), 0u) == words.size() - num_duplicates);
    }

    REQUIRE(row_sum_equal);
}