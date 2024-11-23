// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: data types for crossword class
// ==================================================================

#ifndef CROSSWORD_DATA_TYPES_H
#define CROSSWORD_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"

using namespace common_data_types_ns;

namespace crossword_data_types_ns {
    
    // types/status of one cw tile
    enum cw_tile_type {
        TILE_FILLED = 0, // a char occupies this tile
        TILE_EMPTY  = 1, // no char in this tile, can be added
        TILE_BLACK  = 2  // black tile, i.e. no char goes here
    };

    cw_tile_type char_to_tile_type(const char& c);

    // representation of one cw tile
    struct cw_tile {
        cw_tile_type type; // type/status of tile
        char c;            // WILDCARD iff type == TILE_EMPTY, BLACK iff type == TILE_BLACK
    };

    // direction a word can be placed in a crossword
    enum word_direction {
        HORIZONTAL = 0,
        VERTICAL   = 1,
    };

    // for more interpretable prints
    const unordered_map<word_direction, string> word_dir_name = {
        {HORIZONTAL, "HORIZONTAL"},
        {VERTICAL, "VERTICAL"}
    };

} // crossword_data_types_ns

#endif // CROSSWORD_DATA_TYPES_H