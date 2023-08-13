// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: common virtual class implementation
// ==================================================================

#include "common_parent.h"

using namespace common_parent_ns;

/**
 * @brief common constructor for all blocks
 * 
 * @param name name of this block
*/
common_parent::common_parent(string name) {
    this->name = name;
    utils = new cw_utils(name, VERBOSITY);
}