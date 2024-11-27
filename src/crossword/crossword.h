// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class declaration for crossword representation class
// ==================================================================

#ifndef CROSSWORD_H
#define CROSSWORD_H

#include "crossword_data_types.h"
#include "../common/common_parent.h"

using namespace crossword_data_types_ns;
using namespace common_parent_ns;

namespace crossword_ns {
    /**
     * @brief represents a crossword puzzle
    */
    class crossword : public common_parent {
        public:
            // base constructor
            crossword(string name, uint length, uint height);

            // constructor with declaration of puzzle contents in vector form
            crossword(string name, uint length, uint height, vector<vector<char> > contents);

            // constructor with declaration of puzzle contents in string form
            crossword(string name, uint length, uint height, string contents);

            // get dimensions
            uint rows() const { return height; }
            uint cols() const { return length; }

            // access tiles for csp
            char read_at(uint row, uint col) const;

            // access/modify puzzle for csp
            void write(word_assignment&& assignment);
            string undo_prev_write();
            
            // TODO: add func to add some extra black tile

            // report tiles whose intersection(s) caused csp to become invalid
            void report_invalidating_tiles(vector<pair<uint, uint> >&& tiles);

            // printing puzzle progress and original out
            string serialize_result()  const;
            string serialize_initial() const;

            // undo all word writes
            void reset();

            // returns copy of this grid without progress
            // TODO: implement
            // TODO: figure out if this should zero out invalid_freq
            crossword clone() const;

        protected:
            // dimensions of crossword puzzle
            uint length;
            uint height;

            // crossword puzzle, indexed by row then column, ex:
            // { {tile, tile},
            //   {tile, tile} }
            vector<vector<cw_tile> > puzzle;
            
            // previous word assignments
            stack<word_assignment> prev_written_words;

            // represents how frequent a tile's intersection caused a csp to become invalid
            // same shape as puzzle
            // TODO: figure out when this should be zero'd: on reset? or when black tile added?
            vector<vector<uint> > invalid_freq;

            // access/modify puzzle for internal use only
            void write_at(char c, uint row, uint col, word_direction dir);
            void erase_at(char expected_c, uint row, uint col, word_direction dir);
            char read_initial_at(uint row, uint col) const;

            // TODO: add function to convert a tile in grid to black tile, with symmetrical param
    }; // crossword
} // crossword_ns

#endif // CROSSWORD_H