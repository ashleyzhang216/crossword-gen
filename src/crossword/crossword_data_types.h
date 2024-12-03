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
        // current type. cannot switch between BLACK and FILLED/EMPTY through writing/erasing words 
        cw_tile_type type;

        // input value on grid construction from contents, cannot change
        // TODO: labeled this as const and deal with associated struct copying issues
        char input_val;

        // when changed to modify grid, must reset values of across/down and recalculate type
        char initial_val;

        // values written to this tile by across/down vars
        // type != TILE_FILLED --> both have no value
        optional<char> across;
        optional<char> down;

        // construct with input value
        cw_tile(char c);

        // set black, without changing input_val
        void set_black();
    };

    // direction a word can be placed in a crossword
    enum word_direction {
        ACROSS = 0,
        DOWN   = 1,
    };

    // for more interpretable prints
    const unordered_map<word_direction, string> word_dir_name = {
        {ACROSS, "ACROSS"},
        {DOWN,   "DOWN"  }
    };

    // represent writing of one word to the grid
    struct word_assignment {
        uint origin_row;
        uint origin_col;
        string word;
        word_direction dir;
    };

    // hard requirements on all permutations
    // these do not apply the whole grid, only the new black tile
    // the only exception to the above rule is connected
    struct permutation_reqs {
        // puzzle[i][j] becomes black --> puzzle[rows() - i - 1][cols() - j - 1] must also become black
        bool symmetric;

        // every fillable tile must be reachable from each other, applies to entire grid
        // i.e. no closed off sections, or graph of var nodes & constraint edges is connected
        bool connected;

        // for each word a new black tile intersects, it either shorten them or splits them into 2 new words
        // minimum length of the new words created
        uint min_new_word_len;

        // default value
        permutation_reqs();

        // value constructor
        // TODO: use this to allow user to input non-default reqs via CLI
        permutation_reqs(bool s, bool c, uint l);
    };

    // evaluation criteria for permutation
    // criteria based on solvability and basic grid construction tips
    // fields listed in order of absolute priority
    struct permutation_score {
        // length of words split up, larger and more values is better
        vector<uint> old_lens;

        // length of new words generated, larger and more values is better
        vector<uint> new_lens;

        // number of the adjacent 8 tiles initially set as black, lower is better
        uint neighborhood_size;

        // number of the adjacent 8 tiles set to black in a parent permutation, higher i better
        uint cluster_size;

        // touching boundary of puzzle
        bool on_boundary;

        // value constructor
        permutation_score(const vector<uint>& old_lens, const vector<uint>& new_lens, uint n, uint c, bool b);
    };

} // crossword_data_types_ns

#endif // CROSSWORD_DATA_TYPES_H