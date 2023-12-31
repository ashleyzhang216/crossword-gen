// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structure implementations for cw_csp and word_finder
// ==================================================================

#include "cw_trie.h"

using namespace cw_trie_ns;

/**
 * @brief constructor for cw_trie block
 * 
 * @param name the name of this block
*/
cw_trie::cw_trie(string name) : common_parent(name) {
    trie = make_shared<trie_node>(true, false, '_');
}