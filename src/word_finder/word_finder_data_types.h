// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: data types for word_finder class
// ==================================================================

#ifndef WORD_FINDER_DATA_TYPES_H
#define WORD_FINDER_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../lib/src/json.hpp"

using namespace common_data_types_ns;
using json = nlohmann::json;

namespace word_finder_data_types_ns {
    
    // node for word tree 
    struct letter_node {
        bool is_head; 
        char letter; // valid iff !is_head
        bool valid; // if this terminates a valid word

        // ptr to child words
        unordered_map<char, shared_ptr<letter_node> > next;

        // constructor to initialize new words/head
        letter_node(bool is_head, bool valid, char letter);
    };
} // word_finder_data_types_ns

#endif // WORD_FINDER_DATA_TYPES_H