// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: Catch2 tests for word_domain class
// ==================================================================

#include "../catch/catch.hpp"
#include "word_domain_test_driver.h"

using namespace word_domain_test_driver_ns;

/**
 * hello world test for word_domain
*/
TEST_CASE("word_domain hello_world", "[word_domain],[hello_world],[quick]") {
    REQUIRE(true);
}

/**
 * basic test to check trie and find_matches() for barebone dictionary for json files
*/
TEST_CASE("word_domain trie-basic_json", "[word_domain],[quick]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-trie-basic_json", "word_domain/data/data_small.json");
    
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
TEST_CASE("word_domain letters_at_indicies-same_len", "[word_domain],[quick]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-letters_at_indicies-same_len");
    letter_idx_table<uint> init_words;
    //               a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
    init_words.fill({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    letter_idx_table<uint> init_nodes = init_words;

    letter_idx_table<uint> num_words = init_words;
    letter_idx_table<uint> num_nodes = init_nodes;

    vector<word_t> words;
    vector<letter_idx_table<uint> > num_words_truth;
    vector<letter_idx_table<uint> > num_nodes_truth;

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
TEST_CASE("word_domain remove_matching_words-letters_at_indicies", "[word_domain],[quick]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-remove_matching_words-letters_at_indicies");
    vector<word_t> init_words = {
        word_t("aaaaa"), word_t("aaaab"), word_t("aabaa"), 
        word_t("abcda"), word_t("bbcda"), word_t("abbaa"), 
        word_t("abbac"), word_t("cbbac"), word_t("bbcdb")
    };
    vector<pair<uint, char> > remove_params;

    letter_idx_table<uint> init_num_words = {{
        //a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
        { 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 2, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 6, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 5, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }};
    letter_idx_table<uint> init_num_nodes = {{
        //a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
        { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 5, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }};
    letter_idx_table<uint> num_words = init_num_words;
    letter_idx_table<uint> num_nodes = init_num_nodes;
    vector<letter_idx_table<uint> > num_words_ground_truths;
    vector<letter_idx_table<uint> > num_nodes_ground_truths;

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
TEST_CASE("word_domain remove_matching_words-complex-letters_at_indicies", "[word_domain]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-remove_matching_words-complex-letters_at_indicies");

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
    letter_idx_table<uint> init_num_words = {{
        //   a     b     c     d     e     f     g     h     i     j     k     l     m     n     o     p     q     r     s     t     u     v     w     x     y     z
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
    }};
    letter_idx_table<uint> init_num_nodes = {{
        //   a     b     c     d     e     f     g     h     i     j     k     l     m     n     o     p     q     r     s     t     u     v     w     x     y     z
        {    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,    0,    0},
        {   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,    0,    0,    0,    0,    0,    0},
        {  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
    }};
    letter_idx_table<uint> num_words = init_num_words;
    letter_idx_table<uint> num_nodes = init_num_nodes;
    vector<letter_idx_table<uint> > num_words_ground_truths;
    vector<letter_idx_table<uint> > num_nodes_ground_truths;
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
TEST_CASE("word_domain assigning-basic", "[word_domain],[quick]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-assigning-basic");

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
 * more complex test mixing adding, removing, and assigning in an AC-3 step
*/
TEST_CASE("word_domain adding_removing_assigning-letters_at_indicies", "[word_domain]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-adding_removing_assigning-letters_at_indicies");

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
    letter_idx_table<uint> init_num_words = {{
        //   a     b     c     d     e     f     g     h     i     j     k     l     m     n     o     p     q     r     s     t     u     v     w     x     y     z
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
    }};
    letter_idx_table<uint> init_num_nodes = {{
        //   a     b     c     d     e     f     g     h     i     j     k     l     m     n     o     p     q     r     s     t     u     v     w     x     y     z
        {    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,    0,    0},
        {   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,    0,    0,    0,    0,    0,    0},
        {  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,    0,    0,    0,    0,    0,    0},
        { 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,    0,    0,    0,    0,    0,    0},
    }};
    letter_idx_table<uint> num_words = init_num_words;
    letter_idx_table<uint> num_nodes = init_num_nodes;
    vector<letter_idx_table<uint> > num_words_ground_truths;
    vector<letter_idx_table<uint> > num_nodes_ground_truths;
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

    REQUIRE(driver->test_letters_at_indicies_remove_assign({}, remove_params, std::nullopt, init_num_words, init_num_nodes, num_words_ground_truths, num_nodes_ground_truths));
    REQUIRE(driver->test_letters_at_indicies_row_sums(num_words, 4, num_words_remaining));
}

/**
 * removing and assigning test with words of max length
*/
TEST_CASE("word_domain remove_matching_words-max_length-letters_at_indicies", "[word_domain],[slow]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-remove_matching_words-max_length-letters_at_indicies");

    assert_m(MAX_WORD_LEN <= 32, "value of MAX_WORD_LEN exceeds bit width, update test");

    // for this test, add every possible max length word composed only of letters a and b
    vector<string> fragments = {""};
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        vector<string> next_fragments;
        for(string fragment : fragments) {
            next_fragments.push_back(fragment + "a");
            next_fragments.push_back(fragment + "b");
        }
        fragments = next_fragments;
    }
    vector<word_t> init_words;
    for(string word : fragments) init_words.push_back(word_t(word));
    assert_m(init_words.size() == 1 << MAX_WORD_LEN, "incorrect size of init_words");

    driver->add_words(init_words);

    letter_idx_table<uint> init_num_words;
    init_num_words.fill({ 1 << (MAX_WORD_LEN - 1), 1 << (MAX_WORD_LEN - 1), });
    letter_idx_table<uint> init_num_nodes;
    for(uint i = 0; i < MAX_WORD_LEN; i++) init_num_nodes[i] = {1u << i, 1u << i,};
    
    letter_idx_table<uint> num_words = init_num_words;
    letter_idx_table<uint> num_nodes = init_num_nodes;
    vector<letter_idx_table<uint> > num_words_ground_truths;
    vector<letter_idx_table<uint> > num_nodes_ground_truths;
    vector<pair<uint, char> > remove_params;
    uint num_words_remaining = static_cast<uint>(init_words.size());
    stringstream last_word;

    REQUIRE(driver->test_letters_at_indicies_add({}, num_words, num_nodes, {}, {}));
    REQUIRE(driver->test_letters_at_indicies_row_sums(num_words, MAX_WORD_LEN, num_words_remaining));
    
    // --------------- remove a or b, alternating, from every index ---------------
    
    for(uint letter = 0; letter < MAX_WORD_LEN; letter++) {
        remove_params.push_back({letter, (letter % 2 == 0) ? 'a' : 'b'});
        last_word << ((letter % 2 != 0) ? 'a' : 'b');
        num_words_remaining = num_words_remaining >> 1;

        for(uint i = 0; i < MAX_WORD_LEN; i++) {
            if(i != letter) {
                num_words[i][0] = num_words[i][0] >> 1;
                num_words[i][1] = num_words[i][1] >> 1;
            }
        }
        num_words[letter][letter % 2] = 0;
        num_words_ground_truths.push_back(num_words);

        for(uint i = letter + 1; i < MAX_WORD_LEN; i++) {
            num_nodes[i][0] = num_nodes[i][0] >> 1;
            num_nodes[i][1] = num_nodes[i][1] >> 1;
        }
        num_nodes[letter][letter % 2] = 0;
        num_nodes_ground_truths.push_back(num_nodes);
    }

    REQUIRE(driver->test_letters_at_indicies_remove_assign({}, remove_params, optional<word_t>(word_t(last_word.str())), init_num_words, init_num_nodes, num_words_ground_truths, num_nodes_ground_truths));
    REQUIRE(driver->test_letters_at_indicies_row_sums(num_words, MAX_WORD_LEN, num_words_remaining));

    REQUIRE(num_words_remaining == 1);
    REQUIRE(driver->get_cur_domain().size() == 1 << MAX_WORD_LEN);
}

/**
 * basic test for get_all_letters_at_index
*/
TEST_CASE("word_domain get_all_letters_at_index", "[word_domain][quick]") {
    shared_ptr<word_domain_test_driver> driver = make_shared<word_domain_test_driver>("word_domain_test_driver-get_all_letters_at_index");

    const size_t word_len = 9;
    const uint num_word_variations = 3;
    const uint num_removes = 10;
    letter_idx_table<uint> init_num_words;
    //                   a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
    init_num_words.fill({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    // construct many words using changing intervals between letters 
    vector<word_t> words;
    vector<string> fragments = {""};
    for(uint i = 0; i < word_len; i++) {
        vector<string> next_fragments;
        for(string fragment : fragments) {
            for(uint j = 0; j < num_word_variations; j++) {
                char letter = static_cast<char>('a' + (i*j + i) % NUM_ENGLISH_LETTERS);
                next_fragments.push_back(fragment + letter);
            }
        }
        fragments = next_fragments;
    }
    unordered_set<string> fragments_unique(fragments.begin(), fragments.end());
    for(string word : fragments_unique) words.push_back(word_t(word));

    // construct many remove params using changing intervals and letters
    vector<pair<uint, char> > remove_params;
    for(uint i = 0; i < num_removes; i++) {
        uint index = (7 * num_removes + i) % word_len;
        char letter = static_cast<char>((11 * num_removes + i) % NUM_ENGLISH_LETTERS + 'a');
        remove_params.push_back({index, letter});
    }

    REQUIRE(driver->test_get_all_letters_at_index(words, remove_params, init_num_words));
}

/**
 * basic test case for has_letters_at_index_with_letter_assigned()
*/
TEST_CASE("word_domain has_letters_at_index_with_letter_assigned-basic", "[word_domain],[quick]") {
    unique_ptr<word_domain_test_driver> driver = make_unique<word_domain_test_driver>("has_letters_at_index_with_letter_assigned-basic");

    REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(0));
    REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(1));
    REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(5));
    REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(MAX_WORD_LEN));

    vector<word_t> init_words = {
        word_t("aaaaa"), word_t("aaaab"), word_t("aabaa"), 
        word_t("abcda"), word_t("bbcda"), word_t("abbaa"), 
        word_t("abbac"), word_t("cbbac"), word_t("bbcdb")
    };
    driver->add_words(init_words);

    REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(5));

    driver->assign_domain(word_t("abcda"));

    REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(5));
}

/**
 * more complex test case for has_letters_at_index_with_letter_assigned()
*/
TEST_CASE("word_domain has_letters_at_index_with_letter_assigned-complex", "[word_domain],[quick]") {
    word_domain_test_driver parser("word_domain_test_driver-parser", "word_domain/data/data_small.json");
    vector<word_t> domain;
    for(const auto& pair : parser.get_word_map()) {
        domain.push_back(pair.second);
    }

    for(uint i = MIN_WORD_LEN; i < MAX_WORD_LEN; ++i) {
        vector<word_t> domain_copy = domain;
        domain_copy.erase(
            std::remove_if(domain_copy.begin(), domain_copy.end(),
                [&i](const word_t& w) {
                    return w.word.size() != i;
                }
            ),
            domain_copy.end()
        );

        unique_ptr<word_domain_test_driver> driver = make_unique<word_domain_test_driver>("has_letters_at_index_with_letter_assigned-complex");
        driver->add_words(domain_copy);
        
        REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(i));

        if(domain_copy.size()) {
            driver->assign_domain(domain_copy.at(domain_copy.size() / 2));
            REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(i));
            driver->unassign_domain();
            REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(i));
        }

        vector<pair<uint, char> > remove_params = {
            {static_cast<uint>((97 * domain_copy.size() + 89) % i), static_cast<char>(( 3 * domain_copy.size() +  5) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((79 * domain_copy.size() + 73) % i), static_cast<char>(( 7 * domain_copy.size() + 11) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((71 * domain_copy.size() + 67) % i), static_cast<char>((13 * domain_copy.size() + 17) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((61 * domain_copy.size() + 59) % i), static_cast<char>((19 * domain_copy.size() + 23) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((53 * domain_copy.size() + 47) % i), static_cast<char>((29 * domain_copy.size() + 31) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((41 * domain_copy.size() + 37) % i), static_cast<char>((37 * domain_copy.size() + 41) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((31 * domain_copy.size() + 29) % i), static_cast<char>((43 * domain_copy.size() + 47) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((23 * domain_copy.size() + 19) % i), static_cast<char>((53 * domain_copy.size() + 59) % NUM_ENGLISH_LETTERS + 'a')},
            {static_cast<uint>((17 * domain_copy.size() + 13) % i), static_cast<char>((61 * domain_copy.size() + 67) % NUM_ENGLISH_LETTERS + 'a')},
        };
        
        std::function<void(const uint)> run_test_at_depth;
        run_test_at_depth = [&run_test_at_depth, &remove_params, &driver, &i](const uint depth) {
            if(depth < remove_params.size()) {
                REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(i));
                
                driver->start_new_ac3_call();
                const size_t num_removed = driver->remove_matching_words(remove_params[depth].first, remove_params[depth].second);
                run_test_at_depth(depth + 1);
                REQUIRE(num_removed == driver->undo_prev_ac3_call());

                REQUIRE(driver->test_has_letters_at_index_with_letter_assigned(i));
            }
        };

        run_test_at_depth(0);
    }
}
