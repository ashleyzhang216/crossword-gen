// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: data types for word_finder class
// ==================================================================

#ifndef WORD_FINDER_DATA_TYPES_H
#define WORD_FINDER_DATA_TYPES_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

namespace word_finder_data_types_ns {
    
    // node for word tree 
    // TODO: convert this to use std::unique_ptr
    typedef struct letter_node {
        bool is_head; 
        char letter; // valid iff !is_head
        bool valid; // if this terminates a valid word

        // ptr to child words
        unordered_map<char, letter_node* > next;

        // constructor to initialize new words/head
        letter_node(bool is_head, bool valid, char letter);
    } letter_node;
}

#endif // WORD_FINDER_DATA_TYPES_H