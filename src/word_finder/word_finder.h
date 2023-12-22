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
    /**
     * @brief utility class to find words that fit within patterns
    */
    class word_finder : public common_parent {
        public:
            // base constructor
            word_finder(string name, string file_addr);

            // word check
            bool is_word(string word);

            // find all words that match a pattern
            void find_matches(unordered_set<word_t>* matches, string& pattern);

            // destructor, automatically destructs raii objects
            ~word_finder() {}

        private:
            // file that this object read from
            string file_addr;

            // for processing input txt file
            ifstream word_file;

            // tree of all words
            shared_ptr<letter_node> word_tree;

            // map of all words to word structs (with heuristics) for O(1) validity checking & struct lookup for find_matches()
            unordered_map<string, word_t> word_map;

            // helper to check if word is legal
            string parse_word(string& word);

            // helper to insert words into word_tree upon initialization
            void add_word_to_tree(shared_ptr<letter_node> node, string& word, uint pos);

            // helper to traverse word_tree for finding all words that fit a pattern
            void traverse_to_find_matches(unordered_set<word_t>* matches, string& pattern, uint pos, shared_ptr<letter_node> node, string fragment);
    };
}

#endif // WORD_FINDER_H