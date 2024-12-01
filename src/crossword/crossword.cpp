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
crossword::crossword(const string& name, uint length, uint height) 
    : common_parent(name, VERBOSITY),
      length(length),
      height(height),
      puzzle(height, vector<cw_tile>(length, cw_tile(TILE_EMPTY, WILDCARD))),
      invalid_freq(height, vector<uint>(length, 0u)),
      num_fillable_tiles(length * height) {
    assert(length > 0);
    assert(height > 0);
}

/**
 * @brief constructor for crossword with puzzle contents in vector
 * @param length # of columns
 * @param height # of rows
 * @param contents vector contents of puzzle
*/
crossword::crossword(const string& name, uint length, uint height, vector<vector<char> > contents) 
    : crossword(
        name, 
        length, 
        height, 
        [&contents]() {
            string flat;
            for (const auto& row : contents) {
                flat.append(row.begin(), row.end());
            }
            return flat;
        }()
      ) {
    // do nothing
}

/**
 * @brief constructor for crossword with puzzle contents in string
 * @param length # of columns
 * @param height # of rows
 * @param contents string contents of puzzle, indexed by row then column
*/
crossword::crossword(const string& name, uint length, uint height, string contents) : crossword(name, length, height) {
    assert(contents.size() == length * height);

    // update non-wildcard puzzle contents, other constructor inits puzzle to all wildcards
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < length; j++) {
            char cur = contents.at(i*length + j);
            cw_tile_type type = char_to_tile_type(cur);

            if(type != TILE_EMPTY) {
                puzzle[i][j] = cw_tile(type, cur);
            }
            if(type == TILE_BLACK) {
                num_fillable_tiles--;
            }
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
    assert(c >= 'a' && c <= 'z');

    optional<char>& this_opt = (dir == ACROSS ? puzzle[row][col].across : puzzle[row][col].down);

    assert(puzzle[row][col].type != TILE_BLACK);
    assert(!this_opt.has_value());
    assert(puzzle[row][col].initial_val == c || puzzle[row][col].initial_val == WILDCARD);

    puzzle[row][col].type = TILE_FILLED;
    this_opt = std::make_optional<char>(c);
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
    assert(expected_c >= 'a' && expected_c <= 'z');

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
*/
char crossword::read_at(uint row, uint col) const {
    assert(row < rows());
    assert(col < cols());

    if(puzzle[row][col].type == TILE_BLACK) {
        return BLACK;
    } else if(puzzle[row][col].type == TILE_EMPTY) {
        return WILDCARD;
    } else {
        // catch edge case initially filled where vals of across/down don't matter
        const cw_tile_type initial_type = char_to_tile_type(puzzle[row][col].initial_val);
        if(initial_type == TILE_FILLED) {
            return puzzle[row][col].initial_val;
        }

        const bool has_across = puzzle[row][col].across.has_value();
        const bool has_down   = puzzle[row][col].down  .has_value();
        assert(has_across || has_down);

        // enforce equality if written in both directions
        if(has_across) {
            const char across_val = puzzle[row][col].across.value();
            assert(across_val == puzzle[row][col].down.value_or(across_val));
            return across_val;
        } else {
            const char down_val = puzzle[row][col].down.value();
            assert(down_val == puzzle[row][col].across.value_or(down_val));
            return down_val;
        }
    }
}

/**
 * reads initial char value from a single tile
 * @param row target row
 * @param col target column
*/
char crossword::read_initial_at(uint row, uint col) const {
    assert(row < rows());
    assert(col < cols());

    return puzzle[row][col].initial_val;
}

/**
 * @brief assign a word to this grid
 * @param assignment rval assignment params struct
*/
void crossword::write(word_assignment&& assignment) {
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
string crossword::undo_prev_write() {
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

/**
 * @brief report tiles that caused csp to become invalid in AC-3 algorithm
 * 
 * @param tiles rvalue vec of tile indices in the format of {row, col}  
*/
void crossword::report_invalidating_tiles(vector<pair<uint, uint> >&& tiles) {
    for(const auto& tile : tiles) {
        invalid_freq[tile.first][tile.second]++;
    }
}

/**
 * @brief resets all previous calls to write()
*/
void crossword::reset() {
    while(!prev_written_words.empty()) {
        undo_prev_write();
    }

    for(uint row = 0; row < rows(); row++) {
        for(uint col = 0; col < cols(); col++) {
            // for defensive programming
            assert(!puzzle[row][col].across.has_value());
            assert(!puzzle[row][col].down.has_value());
        }
    }
}

/**
 * @brief returns copy of this puzzle without progress
*/
crossword crossword::clone() const {
    crossword next = *this;
    next.reset();
    return next;
}

/**
 * @brief returns permutation of this puzzle without progress and with additional black tile
*/
crossword crossword::get_permutation() const {
    crossword next = clone();
    next.permute();

    return next;
}

/**
 * @brief add additional black tile to grid and zero out invalid_freq
 * @pre no progress made on this grid, i.e. prev_written_words is empty
 *
 * TODO: currently sets the first fillable tile it finds to black, add a real implementation
*/
void crossword::permute() {
    assert(prev_written_words.empty());

    // TODO: remove test implementation
    auto set_first_fillable_black = [this]() -> void {
        for(uint row = 0; row < rows(); row++) {
            for(uint col = 0; col < cols(); col++) {
                if(puzzle[row][col].type != TILE_BLACK) {
                    puzzle[row][col] = cw_tile(TILE_BLACK, BLACK);
                    num_fillable_tiles--;
                    return;
                }
            }
        }
    };

    set_first_fillable_black();

    // reset invalid_freq
    invalid_freq = vector<vector<uint> >(height, vector<uint>(length, 0u));
}

/**
 * @brief serialize crossword progress
*/
string crossword::serialize_result() const {
    stringstream ss;
    
    for(uint row = 0; row < rows(); row++) {
        ss << endl;
        for(uint col = 0; col < cols(); col++) {
            ss << read_at(row, col) << " ";
        }
    }

    return ss.str();
}

/**
 * @brief serialize initial crossword layout, with any grid modifications
*/
string crossword::serialize_initial() const {
    stringstream ss;
    
    for(uint row = 0; row < rows(); row++) {
        ss << endl;
        for(uint col = 0; col < cols(); col++) {
            ss << read_initial_at(row, col) << " ";
        }
    }

    return ss.str();
}
