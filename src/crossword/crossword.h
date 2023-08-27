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

            // access/modify puzzle
            void write_at(char c, uint row, uint col);
            char read_at(uint row, uint col) const;

            // for printing puzzle out
            friend ostream& operator<<(ostream& os, const crossword& cw);

            // destructor
            ~crossword();

        private:
            // dimensions of crossword puzzle
            uint length;
            uint height;

            // crossword puzzle, indexed by row then column, ex:
            // { {tile, tile},
            //   {tile, tile} }
            vector<vector<cw_tile> > puzzle;

    }; // crossword
    
    // for printing crossword contents
    ostream& operator<<(ostream& os, const crossword& cw);
} // crossword_ns

#endif // CROSSWORD_H