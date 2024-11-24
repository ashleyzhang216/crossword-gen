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
crossword::crossword(string name, uint length, uint height) : common_parent(name, VERBOSITY) {
    assert(length > 0);
    assert(height > 0);

    this->length = length;
    this->height = height;

    // declare puzzle vector size, initialize all tiles to empty
    puzzle.resize(height);
    for(uint i = 0; i < height; i++) {
        puzzle[i].resize(length);
        for(uint j = 0; j < length; j++) {
            // puzzle[i][j] = {
            //     .type = TILE_EMPTY,
            //     .c = WILDCARD
            // };
            puzzle[i][j] = {
                .type        = TILE_EMPTY,
                .initial_val = WILDCARD,
                .across      = std::nullopt,
                .down        = std::nullopt
            };
        }
    }
}

/**
 * @brief constructor for crossword with puzzle contents in vector
 * @param length # of columns
 * @param height # of rows
 * @param contents vector contents of puzzle
*/
crossword::crossword(string name, uint length, uint height, vector<vector<char> > contents) : crossword(name, length, height) {
    // validate input puzzle size
    assert(puzzle.size() == height);
    for(uint i = 0; i < height; i++) assert(puzzle[i].size() == length);

    // update puzzle contents
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < length; j++) {
            // puzzle[i][j] = {
            //     .type = char_to_tile_type(contents[i][j]),
            //     .c = contents[i][j]
            // };
            cw_tile_type tile_type = char_to_tile_type(contents[i][j]);
            puzzle[i][j] = {
                .type        = tile_type,
                .initial_val = contents[i][j],
                .across      = tile_type == TILE_FILLED ? std::make_optional<char>(contents[i][j]) : std::nullopt,
                .down        = tile_type == TILE_FILLED ? std::make_optional<char>(contents[i][j]) : std::nullopt
            };
        }
    }
}

/**
 * @brief constructor for crossword with puzzle contents in string
 * @param length # of columns
 * @param height # of rows
 * @param contents string contents of puzzle, indexed by row then column
*/
crossword::crossword(string name, uint length, uint height, string contents) : crossword(name, length, height) {
    assert(contents.size() == length * height);

    // update puzzle contents
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < length; j++) {
            // puzzle[i][j] = {
            //     .type = char_to_tile_type(contents.at(i*length + j)),
            //     .c = contents.at(i*length + j)
            // };
            cw_tile_type tile_type = char_to_tile_type(contents.at(i*length + j));
            puzzle[i][j] = {
                .type        = tile_type,
                .initial_val = contents.at(i*length + j),
                .across      = tile_type == TILE_FILLED ? std::make_optional<char>(contents.at(i*length + j)) : std::nullopt,
                .down        = tile_type == TILE_FILLED ? std::make_optional<char>(contents.at(i*length + j)) : std::nullopt
            };
        }
    }
}

/**
 * write a single tile 
 * @param c the char to write
 * @param row target row
 * @param col target column
 * @param dir direction of word associated with this write
*/
void crossword::write_at(char c, uint row, uint col, word_direction dir) {
    assert(row < rows());
    assert(col < cols());

    optional<char>&       this_opt  = (dir == ACROSS ? puzzle[row][col].across : puzzle[row][col].down  );
    const optional<char>& other_opt = (dir == ACROSS ? puzzle[row][col].down   : puzzle[row][col].across);

    assert(
        // empty tile to write to
        puzzle[row][col].type == TILE_EMPTY || 

        // writing to tile with this char already assigned
        (puzzle[row][col].type == TILE_FILLED && other_opt.has_value() && other_opt.value() == c)
    );
    assert(!this_opt.has_value());
    assert(puzzle[row][col].initial_val == c || puzzle[row][col].initial_val == WILDCARD);

    puzzle[row][col].type = TILE_FILLED;
    this_opt = std::make_optional<char>(c);

    // puzzle[row][col] = {
    //     .type = char_to_tile_type(c),
    //     .c = c
    // };
}

/**
 * erase a char written to a tile in a single direction
 * @param expected_c expected char at the tile
 * @param row target row
 * @param col target column
 * @param dir direction of word associated with this erase
*/
void crossword::erase_at(char expected_c, uint row, uint col, word_direction dir) {
    assert(row < rows());
    assert(col < cols());

    optional<char>&       this_opt  = (dir == ACROSS ? puzzle[row][col].across : puzzle[row][col].down  );
    const optional<char>& other_opt = (dir == ACROSS ? puzzle[row][col].down   : puzzle[row][col].across);

    assert(puzzle[row][col].type == TILE_FILLED);
    assert(this_opt.has_value());
    assert(this_opt.value() == expected_c);
    assert(puzzle[row][col].initial_val == expected_c || puzzle[row][col].initial_val == WILDCARD);

    this_opt.reset();
    if(!other_opt.has_value()) {
        puzzle[row][col].type = TILE_EMPTY;
    }
}

/**
 * reads char contained at a single tile
 * @param row target row
 * @param col target column
 * @throws assertion_failure_exception iff out of bounds
*/
char crossword::read_at(uint row, uint col) const {
    assert(row < rows());
    assert(col < cols());

    if(puzzle[row][col].type == TILE_BLACK) {
        return BLACK;
    } else if(puzzle[row][col].type == TILE_EMPTY) {
        return WILDCARD;
    } else {
        // filled tile
        assert(puzzle[row][col].across.has_value() || puzzle[row][col].down.has_value());
        if(puzzle[row][col].across.has_value() && puzzle[row][col].down.has_value()) {
            assert(puzzle[row][col].across.value() == puzzle[row][col].down.value());
        }
        return puzzle[row][col].across.value_or(puzzle[row][col].down.value());
    }
}

/**
 * @brief operator to print crossword contents for testing
*/
ostream& crossword_ns::operator<<(ostream& os, const crossword& cw) {
    for(uint row = 0; row < cw.rows(); row++) {
        os << endl;
        for(uint col = 0; col < cw.cols(); col++) {
            os << cw.read_at(row, col) << " ";
        }
    }

    return os;
}

/**
 * @brief assign a word to this grid
 * @param assignment rval assignment params struct
*/
void crossword::write_word(word_assignment&& assignment) {
    switch(assignment.dir) {
        case ACROSS: {
            for(uint j = 0; j < assignment.word.size(); ++j) {
                write_at(assignment.word.at(j), assignment.origin_row, assignment.origin_col + j, ACROSS);
            }
        } break;
        case DOWN: {
            for(uint i = 0; i < assignment.word.size(); ++i) {
                write_at(assignment.word.at(i), assignment.origin_row + i, assignment.origin_col, DOWN);
            }
        } break;
    }

    prev_written_words.push(std::move(assignment));
}

/**
 * @brief undo the prev call to write_word()
 * @pre at least one previous unresolved call to write_word()
 * 
 * @return word removed from grid, purely for defensive programming
*/
string crossword::undo_prev_write_word() {
    assert(!prev_written_words.empty());

    word_assignment assignment = prev_written_words.top();
    prev_written_words.pop();

    switch(assignment.dir) {
        case ACROSS: {
            for(uint j = 0; j < assignment.word.size(); ++j) {
                erase_at(assignment.word.at(j), assignment.origin_row, assignment.origin_col + j, ACROSS);
            }
        } break;
        case DOWN: {
            for(uint i = 0; i < assignment.word.size(); ++i) {
                erase_at(assignment.word.at(i), assignment.origin_row + i, assignment.origin_col, DOWN);
            }
        } break;
    }

    return assignment.word;
}
