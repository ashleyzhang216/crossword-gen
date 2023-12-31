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
 * @param file_addr full filepath to word file, either .txt for unscored, or .json for scored
*/
word_finder::word_finder(string name, string file_addr) : common_parent(name) {
    // save file, initialize word tree
    this->file_addr = file_addr;
    word_tree = make_shared<letter_node>(true, false, '_');

    // TODO: should .txt file support be removed?
    if(has_suffix(file_addr, ".txt")) {
        // open file
        ifstream word_file;
        word_file.open(file_addr);
        assert_m(word_file.is_open(), "could not open txt file " + file_addr);

        // parse word file
        string word;
        while(getline(word_file, word)) {
            // check for validity & convert uppercase, remove dashes, etc.
            word = parse_word(word);

            // add to word set & tree if of valid size
            if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN && word != "") {
                // add to hashset of all words
                word_map.insert({word, word_t(word)}); // no heuristics added due to being unscored

                add_word_to_tree(word_tree, word, 0);
            } 
        }
        word_file.close();

    } else if(has_suffix(file_addr, ".json")) {
        // open word file, parse data
        ifstream word_file(file_addr);
        assert_m(word_file.is_open(), "could not open json file " + file_addr);
        json j = json::parse(word_file);

        string word;
        for(const auto& [item, data] : j.items()) {
            // no need for parse_word() since incoming json is guarenteed to be clean, besides for word length (all lowercase and alphabetical)
            word = item;

            if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN) {
                // add to hashset of all words
                word_map.insert({word, word_t(word, data["Score"], data["Frequency"])}); // TODO: add other heuristics as needed

                add_word_to_tree(word_tree, word, 0);
            } 
        }
        word_file.close();

    } else {
        ss << "word_finder got file of invalid type: " << file_addr;
        utils->print_msg(&ss, FATAL);
        return;
    }
}

/**
 * @brief check if a string is a valid word
 * 
 * @param word string to check
 * @return true iff word is a valid word
*/
bool word_finder::is_word(string word) {
    return word_map.count(word) > 0;
}

/**
 * @brief adds all words that match pattern with WILDCARD ('?') as placeholder
 * 
 * @param matches ptr to set to add matches to
 * @param pattern the pattern to compare against
*/
void word_finder::find_matches(unordered_set<word_t>* matches, string& pattern) {
    traverse_to_find_matches(matches, pattern, 0, word_tree, "");
}

/**
 * @brief helper for constructor to test validity for and parse words
 * 
 * @param word the word to test
 * @return parsed word iff word only contains lowercase letters, uppercase letters, dashes, apostrophes, semicolons, numbers, spaces; "" otherwise
*/
string word_finder::parse_word(const string& word) {
    stringstream word_ss;
    for(char c : word) {
        if(c >= 'a' && c <= 'z') {
            // valid lowercase letters, do nothing
            word_ss << c;
        } else if(c >= 'A' && c <= 'Z') {
            // valid uppercase letters, convert to lowercase
            word_ss << (char)(c + 'a' - 'A'); // TODO: should this be static_cast<char> ?
        } else if(c == '-' || c == '\'' || c == ' ' || c == ';' || (c >= '0' && c <= '9')) {
            // remove dashes/apostrophes/semicolons/numbers/spaces, do nothing
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
void word_finder::add_word_to_tree(shared_ptr<letter_node> node, string& word, uint pos) {
    
    // all letters added to tree
    if(pos >= word.size()) {
        node->valid = true;
        return;
    }

    // create child node if it doesn't exist yet
    if(node->next.find(word.at(pos)) == node->next.end()) {
        node->next.insert({word.at(pos), make_shared<letter_node>(false, false, word.at(pos))});
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
void word_finder::traverse_to_find_matches(unordered_set<word_t>* matches, string& pattern, uint pos, shared_ptr<letter_node> node, string fragment) {

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

/**
 * @brief helper for word_finder constructor to detect file type
 * 
 * @param str string to check the suffix of
 * @param suffix the suffix to check for
 * @return true iff str ends with suffix
*/
bool word_finder::has_suffix(const string& str, const string& suffix) {
    if(str.size() < suffix.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}