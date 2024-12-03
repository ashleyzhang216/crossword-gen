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
 * @param reqs optional non-default permutation requirements
*/
crossword::crossword(const string& name, uint length, uint height, permutation_reqs reqs)
    : common_parent(name, VERBOSITY),
      length(length),
      height(height),
      puzzle(height, vector<cw_tile>(length, cw_tile(TILE_EMPTY, WILDCARD))),
      invalid_freq(height, vector<uint>(length, 0u)),
      num_fillable_tiles(length * height),
      reqs(reqs) {
    assert(length > 0);
    assert(height > 0);
}

/**
 * @brief constructor for crossword with puzzle contents in vector
 * @param length # of columns
 * @param height # of rows
 * @param contents vector contents of puzzle
 * @param reqs optional non-default permutation requirements
*/
crossword::crossword(const string& name, uint length, uint height, vector<vector<char> > contents, permutation_reqs reqs) 
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
        }(),
        reqs
      ) {
    // do nothing
}

/**
 * @brief constructor for crossword with puzzle contents in string
 * @param length # of columns
 * @param height # of rows
 * @param contents string contents of puzzle, indexed by row then column
 * @param reqs optional non-default permutation requirements
*/
crossword::crossword(const string& name, uint length, uint height, string contents, permutation_reqs reqs) : crossword(name, length, height, reqs) {
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
 * @brief returns all permutations of this puzzle without progress and with additional black tile
 *
 * @param explored_grids ref to set of grids already being explored, to avoid duplicates
*/
vector<crossword> crossword::permutations(unordered_set<string>& explored_grids) const {
    vector<crossword> res;
    
    for(uint row = 0; row < rows(); row++) {
        for(uint col = 0; col < cols(); col++) {
            if(puzzle[row][col].type != TILE_BLACK) {
                crossword next = clone();

                if(next.permute(row, col, explored_grids)) {
                    res.push_back(std::move(next));
                }

                // TODO: use entry in invalid_freq and run other functions to record score for this permutation
            }
        }
    }

    // TODO: sort permutations by score
    return res;
}

/**
 * @brief try to create a valid permutation by setting a fillable tile to a black tile
 *
 * @param row target row of fillable tile to set to black tile
 * @param col target col of fillable tile to set to black tile
 * @param explored_grids ref to set of grids already being explored, to avoid duplicates
 * @return true iff successful, i.e. making this tile black does not violate any grid restrictions
*/
bool crossword::permute(uint row, uint col, unordered_set<string>& explored_grids) {
    assert(prev_written_words.empty());
    assert(row < rows());
    assert(col < cols());
    assert(puzzle[row][col].type != TILE_BLACK);

    /**
     * @brief check if no closed off section
     *
     * @return true iff all fillable tiles in grid accessible from one another
     */
    auto connected = [this]() -> bool {
        // finds first fillable tile
        auto find_first_fillable = [this]() -> optional<pair<uint, uint> > {
            for(uint row = 0; row < rows(); row++) {
                for(uint col = 0; col < cols(); col++) {
                    if(puzzle[row][col].type != TILE_BLACK) {
                        return std::make_optional(std::make_pair(row, col));
                    }
                }
            }

            return std::nullopt;
        };

        // finds number of fillable tiles accessible via adjacent tiles from a root
        auto num_accessible_tiles = [this](pair<uint, uint>& root) -> uint {
            vector<vector<bool> > visited(height, vector<bool>(length, false));
            stack<pair<uint, uint> > frontier;

            visited[root.first][root.second] = true;
            frontier.push(std::move(root));

            uint num_visited = 0u;
            while(!frontier.empty()) {
                pair<uint, uint> cur = frontier.top();
                frontier.pop();
                assert(visited[cur.first][cur.second]);
                ++num_visited;

                vector<pair<uint, uint> > candidates;
                if(cur.first > 0u) {
                    candidates.push_back(std::make_pair(cur.first - 1, cur.second));
                }
                if(cur.first + 1u < rows()) {
                    candidates.push_back(std::make_pair(cur.first + 1, cur.second));
                }
                if(cur.second > 0u) {
                    candidates.push_back(std::make_pair(cur.first, cur.second - 1));
                }
                if(cur.second + 1u < cols()) {
                    candidates.push_back(std::make_pair(cur.first, cur.second + 1));
                }

                for(pair<uint, uint>& c : candidates) {
                    if(!visited[c.first][c.second] && puzzle[c.first][c.second].type != TILE_BLACK) {
                        visited[c.first][c.second] = true;
                        frontier.push(std::move(c));
                    }
                }
            }

            return num_visited;
        };

        optional<pair<uint, uint> > root = find_first_fillable();
        if(!root.has_value()) {
            return false;
        }
        cout << "num_accessible_tiles=" << num_accessible_tiles(root.value()) << ", expected=" << num_fillable_tiles << endl; // DEBUG
        return num_accessible_tiles(root.value()) == num_fillable_tiles;
    };

    /**
     * @brief gets information about the word(s) a new black tile intersects
     *
     * @param row target row of new black tile
     * @param col target col of new black tile
     * @param old_lens ref to vector to add lengths of words this tile intersects to
     * @param new_lens ref to vector to add lengths of new words created by this black tile to
     * @return true iff reqs.min_new_word_len not violated
     */
    auto intersection_info = [this](uint row, uint col, vector<uint>& old_lens, vector<uint>& new_lens) -> bool {
        // finds number of fillable tiles in some direction
        std::function<uint(int, int, const pair<int, int>&)> length_in_dir;
        length_in_dir = [this, &length_in_dir](int cur_row, int cur_col, const pair<int, int>& dir) -> uint {
            if(cur_row < 0 || cur_row >= static_cast<int>(rows())) return 0u;
            if(cur_col < 0 || cur_col >= static_cast<int>(cols())) return 0u;
            if(puzzle[static_cast<size_t>(cur_row)][static_cast<size_t>(cur_col)].type == TILE_BLACK) return 0u;

            return 1u + length_in_dir(cur_row + dir.first, cur_col + dir.second, dir);
        };

        uint up    = length_in_dir(static_cast<int>(row) - 1, static_cast<int>(col),     std::make_pair(-1,  0));
        uint down  = length_in_dir(static_cast<int>(row) + 1, static_cast<int>(col),     std::make_pair( 1,  0));
        uint left  = length_in_dir(static_cast<int>(row),     static_cast<int>(col) - 1, std::make_pair( 0, -1));
        uint right = length_in_dir(static_cast<int>(row),     static_cast<int>(col) + 1, std::make_pair( 0,  1));

        cout << "root=(" << row << ", " << col << "), up=" << up << ", down=" << down << ", left=" << left << ", right=" << right << ", min=" << reqs.min_new_word_len << endl; // DEBUG

        // check if reqs.min_new_word_len violated
        if(
            (up    != 0u && up    < reqs.min_new_word_len) ||
            (down  != 0u && down  < reqs.min_new_word_len) ||
            (left  != 0u && left  < reqs.min_new_word_len) ||
            (right != 0u && right < reqs.min_new_word_len)
        ) {
            return false;
        }

        // ignore non-words (length of 1) that this tile split
        if(up != 0u || down != 0u) {
            old_lens.push_back(up + down + 1u);
        }
        if(left != 0u || right != 0u) {
            old_lens.push_back(left + right + 1u);
        }

        // ignore new words of length 0, i.e. this tile is adjacent to another black tile or boundary
        if(up    != 0u) new_lens.push_back(up);
        if(down  != 0u) new_lens.push_back(down);
        if(left  != 0u) new_lens.push_back(left);
        if(right != 0u) new_lens.push_back(right);

        return true;
    };

    // for calls to intersection_info() later
    vector<uint> old_lens;
    vector<uint> new_lens;

    bool added_symmetrical_black = false;

    // set black, then test for violations of grid restrictions
    puzzle[row][col] = cw_tile(TILE_BLACK, BLACK);
    num_fillable_tiles--;

    // add second black tile if symmetric
    if(reqs.symmetric && puzzle[rows() - row - 1][cols() - col - 1].type != TILE_BLACK) {
        puzzle[rows() - row - 1][cols() - col - 1] = cw_tile(TILE_BLACK, BLACK);
        num_fillable_tiles--;
        added_symmetrical_black = true;
    }

    cout << "considering: " << serialize_initial() << endl; // DEBUG

    // validate word lengths of intersection for this black tile
    if(!intersection_info(row, col, old_lens, new_lens)) {
        cout << "fail intersections for this" << endl << endl; // DEBUG
        return false;
    }

    // validate word lengths of intersection for mirrored black tile
    if(added_symmetrical_black && !intersection_info(rows() - row - 1, cols() - col - 1, old_lens, new_lens)) {
        cout << "fail intersections for mirror" << endl << endl; // DEBUG
        return false;
    }

    // enforce connectedness
    if(reqs.connected && !connected()) {
        cout << "not connected" << endl << endl; // DEBUG
        return false;
    }

    // make sure this isn't a duplicate grid
    string serialized = serialize_initial();
    if(explored_grids.count(serialized)) {
        cout << "duplicate" << endl << endl; // DEBUG
        return false;
    }
    explored_grids.insert(std::move(serialized));

    cout << "ok" << endl << endl; // DEBUG

    // TODO: do something with the data from calling intersection_info(), populate some real score struct

    invalid_freq = vector<vector<uint> >(height, vector<uint>(length, 0u));
    return true;
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
