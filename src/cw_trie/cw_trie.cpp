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
    trie = make_shared<trie_node>(false, '_', nullptr);
}

/**
 * @brief adds word to word tree, updates letters_at_indices
 * 
 * @param word the word to add, duplicates do nothing
*/
void cw_trie::add_word(word_t w) {
    if(word_map.find(w.word) == word_map.end()) {

        word_map.insert({w.word, w});

        // update word counts in letters_at_indices
        for(uint i = 0; i < w.word.size(); i++) {
            assert('a' <= w.word.at(i) && w.word.at(i) <= 'z');
            letters_at_indices[i][static_cast<size_t>(w.word.at(i) - 'a')].num_words++;
        }

        add_word_to_trie(trie, w.word, 0);

        ss << "num_words: " << endl;
        for(uint i = 0; i < MAX_WORD_LEN; i++) {
            for(char c = 'a'; c <= 'z'; c++) {
                ss << letters_at_indices[i][static_cast<size_t>(c - 'a')].num_words << " ";
            }
            ss << endl;
        }
        utils->print_msg(&ss, DEBUG);

        ss << "num nodes: " << endl;
        for(uint i = 0; i < MAX_WORD_LEN; i++) {
            for(char c = 'a'; c <= 'z'; c++) {
                ss << letters_at_indices[i][static_cast<size_t>(c - 'a')].nodes.size() << " ";
            }
            ss << endl;
        }
        utils->print_msg(&ss, DEBUG);
    }
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
        node->children.insert({word.at(pos), make_shared<trie_node>(false, word.at(pos), node)});

        // update nodes in letters_at_indices
        letters_at_indices[pos][static_cast<size_t>(word.at(pos) - 'a')].nodes.insert(node->children.at(word.at(pos)));
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
 * @brief checks number of words in trie with a specific letter at a specific index
 * 
 * @param index the index to check
 * @param letter the letter to check, 'a' <= letter <= 'z'
 * @returns letters_at_indices[index][letter].num_words
*/
uint cw_trie::num_letters_at_index(uint index, char letter) const {
    assert('a' <= letter && letter <= 'z');
    return letters_at_indices[index][static_cast<size_t>(letter - 'a')].num_words;
}

/**
 * @brief removes words with a specific letter at a specific index from trie
 * @warning behavior undefined if called from word_finder, only supports calls from cw_variable
 * 
 * @param pruned_words ptr to hashset to copy prune words to
 * @param index the index to remove
 * @param letter the letter to remove, 'a' <= letter <= 'z'
*/
void cw_trie::remove_matching_words(shared_ptr<unordered_set<word_t> > pruned_words, uint index, char letter) {
    // downwards removal
    uint num_leafs;
    for(shared_ptr<trie_node> node : letters_at_indices[index][static_cast<size_t>(letter - 'a')].nodes) {
        num_leafs = remove_children(node, pruned_words, index, get_fragment(node));

        if(shared_ptr<trie_node> parent = node->parent.lock()) {
            remove_from_parents(parent, num_leafs, index - 1, letter);
        } else {
            ss << "parent of node index " << index << ", letter " << letter << " deleted early";
            utils->print_msg(&ss, ERROR);
        }
    }
}

/**
 * @brief upwards helper for remove_matching_words(), updates letters_at_indices and removes nodes without remaining valid leafs
 * 
 * @param node this parent node of whom a child may be removed
 * @param num_leafs number of valid words/leafs removed from the original call to remove_matching_words()
 * @param depth depth of this parent node in trie or letter index in the word
 * @param child char of the child node of this parent that may be removed if it contains no more valid leafs
*/
void cw_trie::remove_from_parents(shared_ptr<trie_node> node, uint& num_leafs, uint index, char child) {
    // check for base case of reached root node of trie
    if(shared_ptr<trie_node> parent = node->parent.lock()) {
        // check if child node has no valid leafs of its own and thus should be removed
        if(node->children.at(child)->children.size() == 0) {
            node->children.erase(child);
        }

        letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].num_words -= num_leafs;
        remove_from_parents(parent, num_leafs, index - 1, node->letter);
    }
}

/**
 * @brief downwards helper for remove_matching_words(), records and removes all children of this node recursively and updates letters_at_indices
 * @warning behavior undefined if called from word_finder, only supports calls from cw_variable
 * 
 * @param node current node whose children (not itself) will be removed
 * @param pruned_words ptr to hashset to write removed words to
 * @param index depth of this node in trie or letter index in the word
 * @param fragment fragment of word up to but not including this node
 * @returns number of words/leaf nodes removed
*/
uint cw_trie::remove_children(shared_ptr<trie_node> node, shared_ptr<unordered_set<word_t> > pruned_words, uint index, string fragment) {
    // TODO: change this when undos to calls to remove_matching_words() are implemented
    assert(letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].nodes.count(node) > 0);
    letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].nodes.erase(node);

    // base case for leaf nodes
    if(node->valid) {
        // terminates valid word, assumed to be a leaf node since all domain values in cw_variable are equal length
        assert(node->children.size() == 0);

        pruned_words->insert(word_map.at(fragment + node->letter));
        letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].num_words--;
        return 1;
    }

    // recursive calls to children
    uint num_leafs = 0;
    for(const auto& pair : node->children) {
        num_leafs += remove_children(pair.second, pruned_words, index + 1, fragment + node->letter);
    }

    letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].num_words -= num_leafs;
    node->children.clear(); // remove children
    return num_leafs;
}

/**
 * @brief remove_matching_words() helper to get the string fragment terminating at this node
 * 
 * @param node the current node being read and has not been added to result yet
 * @return string fragment of word before and including the provided node
*/
string cw_trie::get_fragment(shared_ptr<trie_node> node) {
    if(auto p = node->parent.lock()) {
        return get_fragment(p) + node->letter;
    }

    return "";
}
