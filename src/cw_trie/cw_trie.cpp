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
void cw_trie::add_word(word_t& w) {
    // update word counts in letters_at_indices
    word_map.insert({w.word, w});
    for(uint i = 0; i < w.word.size(); i++) {
        assert('a' <= w.word.at(i) && w.word.at(i) <= 'z');
        letters_at_indices[i][(size_t)(w.word.at(i) - 'a')].num_words++;
    }

    add_word_to_trie(trie, w.word, 0);
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
void cw_trie::find_matches(shared_ptr<unordered_set<word_t> > matches, string& pattern) {
    traverse_to_find_matches(matches, pattern, 0, trie, "");
}

/**
 * @brief helper to find_matches() to recursively traverse tree to find matches
 * 
 * @param matches ptr to set to add matches to
 * @param pattern the pattern to compare against
 * @param pos index of next char in pattern to check
 * @param node current node traversing in word_tree
 * @param fragment part of word matched already
*/
void cw_trie::traverse_to_find_matches(shared_ptr<unordered_set<word_t> > matches, string& pattern, uint pos, shared_ptr<trie_node> node, string fragment) {
    ss << "entering traverse_to_find_matches() w/ pattern " << pattern << " at pos " << pos 
       << " @ node " << node->letter;
    utils->print_msg(&ss, DEBUG);

    // pattern fully matched
    if(pos >= pattern.size()) {
        // AND this is a valid word
        ss << "pattern fully matched, valid check: " << node->valid;
        utils->print_msg(&ss, DEBUG);

        if(node->valid) { 
            matches->insert(word_map.at(fragment));
        }
        return;
    }

    if(pattern.at(pos) == WILDCARD) {
        // wildcard at this index, add all possible matches
        ss << "traversing for wild card";
        utils->print_msg(&ss, DEBUG);

        for(auto& pair : node->children) {
            traverse_to_find_matches(matches, pattern, pos + 1, pair.second, fragment + pair.first);
        }

    } else if (node->children.find(pattern.at(pos)) != node->children.end()) {
        // next letter progresses towards a valid word, continue
        ss << "traversing for letter " << pattern.at(pos);
        utils->print_msg(&ss, DEBUG);
        traverse_to_find_matches(matches, pattern, pos + 1, node->children[pattern.at(pos)], fragment + pattern.at(pos));

    } else {
        // this is a dead end, do nothing
    }
}

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
