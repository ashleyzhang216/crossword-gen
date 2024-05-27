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

    REQUIRE(driver->test_letters_at_indicies_add(words, init_words, init_nodes, num_words_truth, num_nodes_truth));
    REQUIRE(driver->test_letters_at_indicies_row_sums(num_words, 5, static_cast<uint>(words.size() - num_duplicates)));
}

/**
 * basic test to check letters_at_indicies after remove_matching_words() calls
*/
TEST_CASE("cw_trie remove_matching_words-letters_at_indicies", "[cw_trie],[quick]") {
    shared_ptr<cw_trie_test_driver> driver = make_shared<cw_trie_test_driver>("cw_trie_test_driver-remove_matching_words-letters_at_indicies");
    vector<word_t> init_words = {
        word_t("aaaaa"), word_t("aaaab"), word_t("aabaa"), 
        word_t("abcda"), word_t("bbcda"), word_t("abbaa"), 
        word_t("abbac"), word_t("cbbac"), word_t("bbcdb")
    };
    vector<pair<uint, char> > remove_params;

    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> init_num_words = {{
        //a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
        { 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 2, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 6, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 5, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }};
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> init_num_nodes = {{
        //a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
        { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 5, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }};
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words = init_num_words;
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_nodes = init_num_nodes;
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths;
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths;

    remove_params.push_back({4u, 'c'});
    num_words[4][2] -= 2; num_words[3][0] -= 2; num_words[2][1] -= 2; num_words[1][1] -= 2; num_words[0][0]--; num_words[0][2]--;
    num_nodes[4][2] -= 2; num_nodes[3][0] -= 1; num_nodes[2][1] -= 1; num_nodes[1][1] -= 1; num_nodes[0][2] -= 1;
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    // double removal should do nothing
    remove_params.push_back({4u, 'c'});
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    remove_params.push_back({3u, 'a'});
    num_words[4][0] -= 3; num_words[4][1] -= 1; num_words[3][0] -= 4; num_words[2][0] -= 2; num_words[2][1] -= 2; num_words[1][0] -= 3; num_words[1][1] -= 1; num_words[0][0] -= 4;
    num_nodes[4][0] -= 3; num_nodes[4][1] -= 1; num_nodes[3][0] -= 3; num_nodes[2][0] -= 1; num_nodes[2][1] -= 2; num_nodes[1][0] -= 1; 
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    remove_params.push_back({0u, 'b'});
    num_words[4][0] -= 1; num_words[4][1] -= 1; num_words[3][3] -= 2; num_words[2][2] -= 2; num_words[1][1] -= 2; num_words[0][1] -= 2;
    num_nodes[4][0] -= 1; num_nodes[4][1] -= 1; num_nodes[3][3] -= 1; num_nodes[2][2] -= 1; num_nodes[1][1] -= 1; num_nodes[0][1] -= 1;
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    remove_params.push_back({2u, 'c'});
    num_words[4][0] -= 1; num_words[3][3] -= 1; num_words[2][2] -= 1; num_words[1][1] -= 1; num_words[0][0] -= 1;
    num_nodes[4][0] -= 1; num_nodes[3][3] -= 1; num_nodes[2][2] -= 1; num_nodes[1][1] -= 1; num_nodes[0][0] -= 1;
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    // double removal should do nothing
    remove_params.push_back({2u, 'c'});
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    // double removal should do nothing
    remove_params.push_back({1u, 'd'});
    num_words_ground_truths.push_back(num_words);
    num_nodes_ground_truths.push_back(num_nodes);

    REQUIRE(driver->test_letters_at_indicies_remove(init_words, remove_params, init_num_words, init_num_nodes, num_words_ground_truths, num_nodes_ground_truths));
    REQUIRE(driver->test_domain_empty());
}

/**
 * more complex removing test using with calculated ground truths
*/
TEST_CASE("cw_trie remove_matching_words-complex-letters_at_indicies", "[cw_trie]") {
    shared_ptr<cw_trie_test_driver> driver = make_shared<cw_trie_test_driver>("cw_trie_test_driver-remove_matching_words-complex-letters_at_indicies");

    // for this test, add every possible 4 letter word composed of only the first 20 letters of alphabet
    vector<word_t> init_words;
    for(char c0 = 'a'; c0 <= 't'; c0++) {
        for(char c1 = 'a'; c1 <= 't'; c1++) {
            for(char c2 = 'a'; c2 <= 't'; c2++) {
                for(char c3 = 'a'; c3 <= 't'; c3++) {
                    string word = {c0, c1, c2, c3};
                    init_words.push_back(word_t(word));
                }
            }
        }
    }
    driver->add_words(init_words);
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> init_num_words = {{
        //   a     b     c     d     e     f     g     h     i     j     k     l     m     n     o     p     q     r     s     t     u     v     w     x     y     z
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
    }};
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> init_num_nodes = {{
        //   a     b     c     d     e     f     g     h     i     j     k     l     m     n     o     p     q     r     s     t     u     v     w     x     y     z
        {    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,    0,    0},
        {   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,    0,    0,    0,    0,    0,    0},
        {  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
    }};
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_words = init_num_words;
    array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> num_nodes = init_num_nodes;
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_words_ground_truths;
    vector<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > num_nodes_ground_truths;
    vector<pair<uint, char> > remove_params;
    uint num_words_remaining = static_cast<uint>(init_words.size());
    uint factor;

    REQUIRE(driver->test_letters_at_indicies_add({}, num_words, num_nodes, {}, {}));
    REQUIRE(driver->test_letters_at_indicies_row_sums(num_words, 4, num_words_remaining));

    init_num_words = num_words;
    init_num_nodes = num_nodes;
    num_words_ground_truths.clear();
    num_nodes_ground_truths.clear();    

    // --------------- remove a @ 0 ---------------

    remove_params.push_back({0, 'a'});
    num_words_remaining -= 8000;

    for(uint i = 1; i < 4; i++) for(uint j = 0; j < 20; j++) num_words[i][j] -= 400;
    num_words[0][0] = 0; 
    num_words_ground_truths.push_back(num_words);

    factor = 1;
    for(uint i = 1; i < 4; i++) { for(uint j = 0; j < 20; j++) { num_nodes[i][j] -= factor; } factor *= 20; }
    num_nodes[0][0] = 0;
    num_nodes_ground_truths.push_back(num_nodes);

    // --------------- remove b @ 0 ---------------

    remove_params.push_back({0, 'b'});
    num_words_remaining -= 8000;

    for(uint i = 1; i < 4; i++) for(uint j = 0; j < 20; j++) num_words[i][j] -= 400;
    num_words[0][1] = 0; 
    num_words_ground_truths.push_back(num_words);

    factor = 1;
    for(uint i = 1; i < 4; i++) { for(uint j = 0; j < 20; j++) { num_nodes[i][j] -= factor; } factor *= 20; }
    num_nodes[0][1] = 0;
    num_nodes_ground_truths.push_back(num_nodes);

    // --------------- remove c @ 1 ---------------

    remove_params.push_back({1, 'c'});
    num_words_remaining -= 400 * 18;

    for(uint j = 2; j < 20; j++) num_words[0][j] -= 20 * 20; // excluding 'a' and 'b' @ index 0 since those are already removed
    for(uint i = 2; i < 4; i++) for(uint j = 0; j < 20; j++) num_words[i][j] -= 18 * 20;
    num_words[1][2] = 0; 
    num_words_ground_truths.push_back(num_words);

    factor = 18;
    for(uint i = 2; i < 4; i++) { for(uint j = 0; j < 20; j++) { num_nodes[i][j] -= factor; } factor *= 20; }
    num_nodes[1][2] = 0;
    num_nodes_ground_truths.push_back(num_nodes);

    // --------------- remove d @ 2 ---------------

    remove_params.push_back({2, 'd'});
    num_words_remaining -= 20 * 19 * 18;

    for(uint j = 2; j < 20; j++) num_words[0][j] -= 20 * 19; // excluding 'a' and 'b' @ index 0 since those are already removed
    for(uint j = 0; j < 20; j++) if(j != 2) num_words[1][j] -= 20 * 18;
    for(uint j = 0; j < 20; j++) num_words[3][j] -= 19 * 18;
    num_words[2][3] = 0; 
    num_words_ground_truths.push_back(num_words);

    factor = 19 * 18;
    for(uint j = 0; j < 20; j++) { num_nodes[3][j] -= factor; }
    num_nodes[2][3] = 0;
    num_nodes_ground_truths.push_back(num_nodes);

    // --------------- remove a @ 3 ---------------

    remove_params.push_back({3, 'a'});
    num_words_remaining -= 19 * 19 * 18;    

    for(uint j = 2; j < 20; j++) num_words[0][j] -= 19 * 19; // excluding 'a' and 'b' @ index 0 since those are already removed
    for(uint j = 0; j < 20; j++) if(j != 2) num_words[1][j] -= 19 * 18;
    for(uint j = 0; j < 20; j++) if(j != 3) num_words[2][j] -= 19 * 18;
    num_words[3][0] = 0;
    num_words_ground_truths.push_back(num_words);

    num_nodes[3][0] = 0;
    num_nodes_ground_truths.push_back(num_nodes);

    REQUIRE(driver->test_letters_at_indicies_remove({}, remove_params,init_num_words, init_num_nodes, num_words_ground_truths, num_nodes_ground_truths));
    REQUIRE(driver->test_letters_at_indicies_row_sums(num_words, 4, num_words_remaining));
}

/**
 * basic test for assigning domain values
*/
TEST_CASE("cw_trie assigning-basic", "[cw_trie],[quick]") {
    shared_ptr<cw_trie_test_driver> driver = make_shared<cw_trie_test_driver>("cw_trie_test_driver-assigning-basic");

    REQUIRE_FALSE(driver->test_num_letters_at_indicies_assign(word_t("hello")));
    driver->add_words({word_t("bread")});
    REQUIRE_FALSE(driver->test_num_letters_at_indicies_assign(word_t("hello")));

    driver->assign_domain(word_t("hello"));
    REQUIRE(driver->test_num_letters_at_indicies_assign(word_t("hello")));
    driver->assign_domain(word_t("jumps"));
    REQUIRE_FALSE(driver->test_num_letters_at_indicies_assign(word_t("hello")));
    REQUIRE(driver->test_num_letters_at_indicies_assign(word_t("jumps")));
    driver->unassign_domain();
    REQUIRE(driver->test_num_letters_at_indicies_assign(word_t("bread")));
    driver->add_words({word_t("jumps")});
    REQUIRE_FALSE(driver->test_num_letters_at_indicies_assign(word_t("jumps")));
    driver->assign_domain(word_t("jumps"));
    REQUIRE(driver->test_num_letters_at_indicies_assign(word_t("jumps")));
    driver->add_words({word_t("skate"), word_t("axels"), word_t("flips")});
    REQUIRE_FALSE(driver->test_num_letters_at_indicies_assign(word_t("hello")));
    REQUIRE(driver->test_num_letters_at_indicies_assign(word_t("flips")));
    driver->unassign_domain();
    REQUIRE_FALSE(driver->test_num_letters_at_indicies_assign(word_t("flips")));
}

/**
 * more complex test mixing adding, removing, and assigning
*/
TEST_CASE("cw_trie adding_removing_assigning", "[cw_trie],[quick]") {
    REQUIRE(true);
}

// TODO: add test case with word of max length

// TODO: add test for letters_at_index