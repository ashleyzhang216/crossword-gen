// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: class declaration for finding word domains
// ==================================================================

#ifndef WORD_FINDER_H
#define WORD_FINDER_H

#include "../common/common_data_types.h"
#include "../common/common_parent.h"
#include "../utils/cw_utils.h"
#include "word_finder_data_types.h"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace word_finder_data_types_ns;

namespace word_finder_ns {
    class word_finder : public common_parent {
        public:
            // base constructor
            word_finder(string name, string file_addr);

            // word check
            bool is_word(string word);

            // find all words that match a pattern
            void find_matches(unordered_set<string>* matches, string pattern);

            // destructor
            ~word_finder();

        private:
            // file that this object read from
            string file_addr;

            // for processing input txt file
            ifstream word_file;

            // tree of all words
            letter_node* word_tree;

            // hashset of all defined words for O(1) validity checking
            unordered_set<string> word_set;

            // helper to insert words into word_tree upon initialization
            void add_word_to_tree(letter_node* node, string word, uint pos);

            // helper to traverse word_tree for finding all words that fit a pattern
            void traverse_to_find_matches(unordered_set<string>* matches, string pattern, uint pos, letter_node* node, string fragment);
    };
}

#endif // WORD_FINDER_H