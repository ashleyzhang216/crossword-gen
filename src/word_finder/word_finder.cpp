// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: class implementation for finding word domains
// ==================================================================

#include "word_finder.h"

using namespace word_finder_ns;

/**
 * @brief constructor for word_finder block
 * 
 * @param name the name of this block
 * @param file_addr full filepath to word file 
*/
word_finder::word_finder(string name, string file_addr) : common_parent(name) {
    
    // open file
    this->file_addr = file_addr;
    word_file.open(file_addr);
    assert(word_file.is_open());

    // initialize word tree
    word_tree = new letter_node(true, '_');

    // parse word file
    string word;
    while(getline(word_file, word))
    {
        // add to hashset of all words
        word_set.insert(word);

        // add to word tree
        if(word.size() <= MAX_WORD_LEN) {
            add_word_to_tree(word_tree, word, 0);
        } else {
            ss << "parsed word (" << word << ") with size over " << MAX_WORD_LEN;
            utils->print_msg(&ss, WARNING);
        }
    }
    word_file.close();
}

/**
 * @brief check if a string is a valid word
 * 
 * @param word string to check
 * @return true iff word is a valid word
*/
bool word_finder::is_word(string word) {
    return word_set.count(word) > 0;
}

/**
 * @brief adds all words that match pattern with PATTERN_PLACEHOLDER ('?') as placeholder
 * 
 * @param matches ptr to set to add matches to
 * @param pattern the pattern to compare against
*/
void word_finder::find_matches(unordered_set<string>* matches, string pattern) {
    traverse_to_find_matches(matches, pattern, 0, word_tree, "");
}

/**
 * @brief destructor for word_finder
*/
word_finder::~word_finder() {
    delete word_tree;
}

/**
 * @brief private helper for word_finder constructor, adds word to word tree
 * 
 * @param node ptr to node to branch off of, if applicable
 * @param word the word to add
 * @param pos index of next letter to add to tree
*/
void word_finder::add_word_to_tree(letter_node* node, string word, uint pos) {
    
    // all letters added to tree
    if(pos >= word.size()) return;

    // create child node if it doesn't exist yet
    if(node->next.find(word.at(pos)) == node->next.end()) {
        node->next.insert({word.at(pos), new letter_node(false, word.at(pos))});
    } 

    // recurse to next letter
    add_word_to_tree(node->next[word.at(pos)], word, pos + 1);

    return;
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
void word_finder::traverse_to_find_matches(unordered_set<string>* matches, string pattern, uint pos, letter_node* node, string fragment) {

    // pattern fully matched, this is a valid word
    if(pos >= pattern.size()) {
        matches->insert(fragment);
        return;
    }

    if(pattern.at(pos) == PATTERN_PLACEHOLDER) {
        // wildcard at this index, add all possible matches
        for(auto& pair : node->next) {
            traverse_to_find_matches(matches, pattern, pos + 1, node->next[pattern.at(pos)], fragment + pair.first);
        }

    } else if (node->next.find(pattern.at(pos)) != node->next.end()) {
        // next letter progresses towards a valid word, continue
        traverse_to_find_matches(matches, pattern, pos + 1, node->next[pattern.at(pos)], fragment + pattern.at(pos));

    } else {
        // this is a dead end, do nothing
    }
}