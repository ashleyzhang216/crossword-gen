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
    trie = make_shared<trie_node>(true, false, '_', nullptr);
}

/**
 * @brief adds word to word tree, updates letters_at_indices
 * 
 * @param word the word to add
*/
void cw_trie::add_word(string& word) {
    // update word counts in letters_at_indices
    for(uint i = 0; i < word.size(); i++) {
        assert('a' <= word.at(i) && word.at(i) <= 'z');
        letters_at_indices[i][(size_t)(word.at(i) - 'a')].num_words++;
    }

    add_word_to_trie(trie, word, 0);
}

/**
 * @brief private helper for add_word(), adds word to trie
 * 
 * @param node ptr to node to branch off of, if applicable
 * @param word the word to add
 * @param pos index of next letter to add to tree
*/
void cw_trie::add_word_to_trie(shared_ptr<trie_node> node, string& word, uint pos) {
    // all letters added to tree
    if(pos >= word.size()) {
        node->valid = true;
        return;
    }

    // create child node if it doesn't exist yet
    if(node->children.find(word.at(pos)) == node->children.end()) {
        node->children.insert({word.at(pos), make_shared<trie_node>(false, false, word.at(pos), node)});

        // update nodes in letters_at_indices
        letters_at_indices[pos][(size_t)(word.at(pos) - 'a')].nodes.insert(node->children.at(word.at(pos)));
    } 

    // recurse to next letter
    add_word_to_trie(node->children.at(word.at(pos)), word, pos + 1);
}

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
bool cw_trie::has_letters_at_index(uint index, char letter) const {
    assert('a' <= letter && letter <= 'z');
    return letters_at_indices[index][(size_t)(letter - 'a')].num_words > 0;
}

// deletion function for words with letters at an index
/**
 * @brief removes words with a specific letter at a specific index from trie
 * @warning behavior undefined if called from word_finder
 * 
 * @param pruned_words ptr to hashset to copy prune words to
 * @param index the index to remove
 * @param letter the letter to remove, 'a' <= letter <= 'z'
*/
// void cw_trie::remove_matching_words(shared_ptr<unordered_set<word_t> > pruned_words, uint index, char letter) {
//     // TODO: implement
// }
