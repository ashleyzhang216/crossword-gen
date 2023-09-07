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
    assert_m(word_file.is_open(), "could not open file " + file_addr);

    // initialize word tree
    word_tree = new letter_node(true, false, '_');

    // parse word file
    string word;
    while(getline(word_file, word))
    {
        // check for validity & convert uppercase, remove dashes, etc.
        word = parse_word(word);

        // add to word set & tree if of valid size
        if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN && word != "") {
            // add to hashset of all words
            word_set.insert(word);

            add_word_to_tree(word_tree, word, 0);
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
 * @brief adds all words that match pattern with WILDCARD ('?') as placeholder
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
    word_tree->~letter_node();
    delete word_tree;
}

/**
 * @brief helper for constructor to test validity for and parse words
 * 
 * @param word the word to test
 * @return parsed word iff word only contains lowercase letters, uppercase letters, dashes, apostrophes, spaces; "" otherwise
*/
string word_finder::parse_word(string word) {
    stringstream word_ss;
    for(char c : word) {
        if(c >= 'a' && c <= 'z') {
            // valid lowercase letters, do nothing
            word_ss << c;
        } else if(c >= 'A' && c <= 'Z') {
            // valid uppercase letters, convert to lowercase
            word_ss << (char)(c + 'a' - 'A');
        } else if(c == '-' || c == '\'' || c == ' ') {
            // remove dashes/apostrophes/spaces, do nothing
        } else {
            // invalid word, contains unknown character
            return "";
        }
    }
    return word_ss.str();
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
    if(pos >= word.size()) {
        node->valid = true;
        return;
    }

    // create child node if it doesn't exist yet
    if(node->next.find(word.at(pos)) == node->next.end()) {
        node->next.insert({word.at(pos), new letter_node(false, false, word.at(pos))});
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

    ss << "entering traverse_to_find_matches() w/ pattern " << pattern << " at pos " << pos 
       << " @ node " << node->letter;
    utils->print_msg(&ss, DEBUG);

    // pattern fully matched
    if(pos >= pattern.size()) {
        // AND this is a valid word
        ss << "pattern fully matched, valid check: " << node->valid;
        utils->print_msg(&ss, DEBUG);

        if(node->valid) matches->insert(fragment);
        return;
    }

    if(pattern.at(pos) == WILDCARD) {
        // wildcard at this index, add all possible matches
        ss << "traversing for wild card";
        utils->print_msg(&ss, DEBUG);

        for(auto& pair : node->next) {
            traverse_to_find_matches(matches, pattern, pos + 1, pair.second, fragment + pair.first);
        }

    } else if (node->next.find(pattern.at(pos)) != node->next.end()) {
        // next letter progresses towards a valid word, continue
        ss << "traversing for letter " << pattern.at(pos);
        utils->print_msg(&ss, DEBUG);
        traverse_to_find_matches(matches, pattern, pos + 1, node->next[pattern.at(pos)], fragment + pattern.at(pos));

    } else {
        // this is a dead end, do nothing
    }
}