// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: crossword clas data type operators/functions
// ==================================================================

#include "crossword_data_types.h"

using namespace crossword_data_types_ns;

/**
 * @brief convert char to proper tile type it belongs in
 * @param c char to convert
 * @throws assertion_failure_exception iff c is not a valid cw char 
 * @return TILE_EMPTY iff c == WILDCARD, TILE_BLACK iff c == BLACK, TILE_FILLED otherwise
*/
cw_tile_type crossword_data_types_ns::char_to_tile_type(const char& c) {
    assert(c == WILDCARD || c == BLACK || (c >= 'a' && c <= 'z'));
    if(c == WILDCARD) return TILE_EMPTY;
    if(c == BLACK) return TILE_BLACK;
    return TILE_FILLED;
}