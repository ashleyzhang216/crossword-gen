// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: class implementation for finding word domains
// ==================================================================

#include "word_finder.h"

using namespace word_finder_ns;

/**
 * constructor for word_finder block
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
 * destructor for word_finder
*/
word_finder::~word_finder() {
    delete word_tree;
}

/**
 * private helper for word_finder constructor, adds word to word tree
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