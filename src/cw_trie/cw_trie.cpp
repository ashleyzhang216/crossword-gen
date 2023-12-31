// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structure implementations for cw_csp and word_finder
// ==================================================================

#include "cw_trie.h"

using namespace cw_trie_ns;

/**
 * @brief constructor for cw_trie block
 * 
 * @param name the name of this block
*/
cw_trie::cw_trie(string name) : common_parent(name) {
    trie = make_shared<trie_node>(true, false, '_');
}

/**
 * @brief adds word to word tree
 * 
 * @param word the word to add
*/
// void cw_trie::add_word(string& word) {
//     // TODO: implement
// }

/**
 * @brief adds all words that match pattern with WILDCARD ('?') as placeholder
 * 
 * @param matches ptr to set to add matches to
 * @param pattern the pattern to compare against
*/
// void cw_trie::find_matches(shared_ptr<unordered_set<word_t> > matches, string& pattern) {
//     // TODO: implement
// }

/**
 * @brief checks if trie contains words with a specific letter at a specific index
 * 
 * @param index the index to check
 * @param letter the letter to check, 'a' <= letter <= 'z'
 * @returns true iff letters_at_indices[index][letter].num_words > 0
*/
bool cw_trie::has_letters_at_index(uint index, char letter) {
    assert('a' <= letter && letter <= 'z');
    return letters_at_indices[index][(size_t)(letter - 'a')].num_words > 0;
}

// deletion function for words with letters at an index
/**
 * @brief removes words with a specific letter at a specific index from trie
 * @warning behavior undefined if called from word_finder
 * 
 * @param index the index to remove
 * @param letter the letter to remove, 'a' <= letter <= 'z'
*/
// void cw_trie::remove_matching_words(uint index, char letter) {
//     // TODO: implement
// }
