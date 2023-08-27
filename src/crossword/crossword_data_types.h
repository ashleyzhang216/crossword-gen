// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: data types for crossword class
// ==================================================================

#ifndef CROSSWORD_DATA_TYPES_H
#define CROSSWORD_DATA_TYPES_H

#include "../common/common_data_types.h"

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
    typedef struct cw_tile {
        cw_tile_type type; // type/status of tile
        char c;            // WILDCARD iff type == TILE_EMPTY, BLACK iff type == TILE_BLACK
    } cw_tile;

} // crossword_data_types_ns

#endif // CROSSWORD_DATA_TYPES_H