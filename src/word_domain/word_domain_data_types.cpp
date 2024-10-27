// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: data type, function implementations for trie and related data structures
// ==================================================================

#include "word_domain_data_types.h"

using namespace word_domain_data_types_ns;

/**
 * @brief explicitly copy underlying node object
 * 
 * @return ptr to deep copy of node
*/
unique_ptr<trie_node> trie_node::clone() const { 
    return make_unique<trie_node>(*this);
}