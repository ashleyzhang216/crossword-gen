// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: operators/constructors for data types for word_finder class
// ==================================================================

#include "word_finder_data_types.h"

using namespace word_finder_data_types_ns;

/**
 * @brief constructor for letter_node object
 * @param is_head if this is the parent node for tree, i.e. ignore this letter
 * @param letter the letter this node contains, undefined iff is_head
*/
letter_node::letter_node(bool is_head, bool valid, char letter) {
    this->is_head = is_head;
    this->valid = valid;
    this->letter = letter;
}