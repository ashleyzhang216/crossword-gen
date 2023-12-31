// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: data types, functions for trie and related data structures
// ==================================================================

#ifndef CW_TRIE_DATA_TYPES_H
#define CW_TRIE_DATA_TYPES_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw_trie_data_types_ns {
    /**
     * @brief node for cw_trie
    */
    struct trie_node {
        bool is_root; // true only for top root node
        bool valid; // true iff this node terminates a valid word
        char letter;

        // ptr to parent
        shared_ptr<trie_node> parent;

        // ptr to child words
        unordered_map<char, shared_ptr<trie_node> > children;

        // constructor to initialize new words/head
        trie_node(bool r, bool v, char l, shared_ptr<trie_node> p) : is_root(r), valid(v), letter(l), parent(p) {}
    };

    /**
     * @brief entry in letters_at_indices
    */
    struct letters_table_entry {
        uint num_words;

        // all nodes that 
        unordered_set<shared_ptr<trie_node> > nodes;

        // base constructor
        letters_table_entry() : num_words(0) {}
    };
}; // cw_trie_data_types_ns

#endif // CW_TRIE_DATA_TYPES_H