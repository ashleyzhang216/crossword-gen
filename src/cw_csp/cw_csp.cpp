// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class implementation for crossword constraint satisfaction problem 
// ==================================================================

#include "cw_csp.h"

using namespace cw_csp_ns;

/**
 * @brief constructor for constraint satisfaction problem w/o puzzle contents
 * 
 * @param length the length of puzzle to be created
 * @param height the height of puzzle to be created
*/
cw_csp::cw_csp(string name, uint length, uint height) : common_parent(name) {
    cw = std::make_unique<crossword>(name + " cw", length, height);
    initialize_csp();
}

/**
 * @brief constructor for constraint satisfaction problem with puzzle contents
 * 
 * @param length the length of puzzle to be created
 * @param height the height of puzzle to be created
 * @param contents the contents to populate puzzle with
*/
cw_csp::cw_csp(string name, uint length, uint height, string contents) : common_parent(name) {
    cw = std::make_unique<crossword>(name + " cw", length, height, contents);
    initialize_csp();
}

/**
 * @brief constructor helper method to populate all csp variables/constraints based on cw
*/
void cw_csp::initialize_csp() {
    // TODO: implement
}