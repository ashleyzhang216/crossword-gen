// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: implementation for crossword representation class
// ==================================================================

#include "crossword.h"

using namespace crossword_ns;

/**
 * @brief base constructor for crossword w/ no contents
 * 
 * @param name name of this crossword object
 * @param length # of columns
 * @param height # of rows
*/
crossword::crossword(string name, uint length, uint height) : common_parent(name) {
    assert(length > 0);
    assert(height > 0);

    this->length = length;
    this->height = height;

    // declare puzzle vector size, initialize all tiles to empty
    puzzle.resize(height);
    for(uint i = 0; i < height; i++) {
        puzzle[i].resize(length);
        for(uint j = 0; j < length; j++) {
            puzzle[i][j] = {
                .type = TILE_EMPTY,
                .c = WILDCARD
            };
        }
    }
}

/**
 * @brief constructor for crossword with puzzle contents
 * @param length # of columns
 * @param height # of rows
 * @param contents contents of puzzle
*/
crossword::crossword(string name, uint length, uint height, vector<vector<char> > contents) : crossword(name, length, height) {
    // validate input puzzle size
    assert(puzzle.size() == height);
    for(uint i = 0; i < height; i++) assert(puzzle[i].size() == length);

    // update puzzle contents
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < length; j++) {
            puzzle[i][j] = {
                .type = char_to_tile_type(contents[i][j]),
                .c = contents[i][j]
            };
        }
    }
}

/**
 * write puzzle contents at a single tile 
 * @param c the char to write
 * @param row target row
 * @param col target column
 * @throws assertion_failure_exception iff out of bounds
*/
void crossword::write_at(char c, uint row, uint col) {
    assert(row >= 0 && row <= rows());
    assert(col >= 0 && col <= cols());

    puzzle[row][col] = {
        .type = char_to_tile_type(c),
        .c = c
    };
}

/**
 * reads char contained at a single tile
 * @param row target row
 * @param col target column
 * @throws assertion_failure_exception iff out of bounds
*/
char crossword::read_at(uint row, uint col) const {
    assert(row >= 0 && row <= rows());
    assert(col >= 0 && col <= cols());

    return puzzle[row][col].c;
}

/**
 * @brief destructor for crossword object
*/
crossword::~crossword() {
    // do nothing
}

/**
 * @brief operator to print crossword contents for testing
*/
ostream& operator<<(ostream& os, const crossword& cw) {
    for(uint row = 0; row < cw.rows(); row++) {
        os << endl;
        for(uint col = 0; col < cw.cols(); col++) {
            os << cw.read_at(row, col) << " ";
        }
    }

    return os;
}