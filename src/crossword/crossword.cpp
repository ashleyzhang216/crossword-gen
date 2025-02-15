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
      puzzle(height, vector<cw_tile>(length, cw_tile(WILDCARD))),
      invalid_freq(height, vector<uint>(length, 0u)),
      num_fillable_tiles(length * height),
      reqs(reqs) {
    cw_assert(length > 0);
    cw_assert(height > 0);
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
    cw_assert(contents.size() == length * height);

    // update non-wildcard puzzle contents, other constructor inits puzzle to all wildcards
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < length; j++) {
            char cur = contents.at(i*length + j);
            cw_tile_type type = char_to_tile_type(cur);

            if(type != TILE_EMPTY) {
                puzzle[i][j] = cw_tile(cur);
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
    cw_assert(row < rows());
    cw_assert(col < cols());
    cw_assert(c >= 'a' && c <= 'z');

    optional<char>& this_opt = (dir == ACROSS ? puzzle[row][col].across : puzzle[row][col].down);

    cw_assert(puzzle[row][col].type != TILE_BLACK);
    cw_assert(!this_opt.has_value());
    cw_assert(puzzle[row][col].initial_val == c || puzzle[row][col].initial_val == WILDCARD);

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
    cw_assert(row < rows());
    cw_assert(col < cols());
    cw_assert(expected_c >= 'a' && expected_c <= 'z');

    optional<char>&       this_opt  = (dir == ACROSS ? puzzle[row][col].across : puzzle[row][col].down  );
    const optional<char>& other_opt = (dir == ACROSS ? puzzle[row][col].down   : puzzle[row][col].across);

    cw_assert(puzzle[row][col].type == TILE_FILLED);
    cw_assert(this_opt.has_value());
    cw_assert(this_opt.value() == expected_c);
    cw_assert(puzzle[row][col].initial_val == expected_c || puzzle[row][col].initial_val == WILDCARD);

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
    cw_assert(row < rows());
    cw_assert(col < cols());

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
        cw_assert(has_across || has_down);

        // enforce equality if written in both directions
        if(has_across) {
            const char across_val = puzzle[row][col].across.value();
            cw_assert(across_val == puzzle[row][col].down.value_or(across_val));
            return across_val;
        } else {
            const char down_val = puzzle[row][col].down.value();
            cw_assert(down_val == puzzle[row][col].across.value_or(down_val));
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
    cw_assert(row < rows());
    cw_assert(col < cols());

    return puzzle[row][col].initial_val;
}

/**
 * reads input char value from a single tile
 * @param row target row
 * @param col target column
*/
char crossword::read_input_at(uint row, uint col) const {
    cw_assert(row < rows());
    cw_assert(col < cols());

    return puzzle[row][col].input_val;
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
    cw_assert(!prev_written_words.empty());

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
            cw_assert(!puzzle[row][col].across.has_value());
            cw_assert(!puzzle[row][col].down.has_value());
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
    vector<pair<crossword, permutation_score> > candidates;
    
    for(uint row = 0; row < rows(); row++) {
        for(uint col = 0; col < cols(); col++) {
            if(puzzle[row][col].type != TILE_BLACK) {
                crossword next = clone();
                optional<permutation_score> result = next.permute(row, col, explored_grids);

                if(result.has_value()) {
                    candidates.push_back(std::make_pair(std::move(next), std::move(result.value())));
                }
            }
        }
    }

    // sort by decreasing score 
    auto compare = [](const pair<crossword, permutation_score>& lhs, const pair<crossword, permutation_score>& rhs) {
        return lhs.second.operator<(rhs.second);
    };
    std::sort(candidates.begin(), candidates.end(), compare);
    std::reverse(candidates.begin(), candidates.end()); // TODO: implement std::greater<permutation_score> to avoid doing this

    // unpack to separate crosswords from their scores, caller doesn't need to know that info
    vector<crossword> res;
    for(auto& c : candidates) {
        res.push_back(std::move(c.first));
    }
    return res;
}

/**
 * @brief try to create a valid permutation by setting a fillable tile to a black tile
 *
 * @param row target row of fillable tile to set to black tile
 * @param col target col of fillable tile to set to black tile
 * @param explored_grids ref to set of grids already being explored, to avoid duplicates
 * @return optional with score value for this permutation iff successful, i.e. making this tile black does not violate any grid restrictions
*/
optional<permutation_score> crossword::permute(uint row, uint col, unordered_set<string>& explored_grids) {
    cw_assert(prev_written_words.empty());
    cw_assert(row < rows());
    cw_assert(col < cols());
    cw_assert(puzzle[row][col].type != TILE_BLACK);

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
                cw_assert(visited[cur.first][cur.second]);
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

        const uint up    = length_in_dir(static_cast<int>(row) - 1, static_cast<int>(col),     std::make_pair(-1,  0));
        const uint down  = length_in_dir(static_cast<int>(row) + 1, static_cast<int>(col),     std::make_pair( 1,  0));
        const uint left  = length_in_dir(static_cast<int>(row),     static_cast<int>(col) - 1, std::make_pair( 0, -1));
        const uint right = length_in_dir(static_cast<int>(row),     static_cast<int>(col) + 1, std::make_pair( 0,  1));

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

    /**
     * @brief get initial and input values for adjacent tiles
     */
    auto explore_adjacent = [this](uint row, uint col, vector<uint>& neighborhood_size, vector<uint>& cluster_size, vector<uint>& times_invalid, vector<bool>& on_boundary) -> void {
        const bool boundary_up    = row > 0;
        const bool boundary_down  = row + 1u < rows();
        const bool boundary_left  = col > 0;
        const bool boundary_right = col + 1u < cols();
        
        uint num_in_bounds = 0u;
        uint neighborhood = 0u;
        uint cluster = 0u;
        auto update_search = [this, &num_in_bounds, &neighborhood, &cluster](uint row, uint col) {
            const char input   = read_input_at  (row, col);
            const char initial = read_initial_at(row, col);

            if(input == BLACK) ++neighborhood;
            if(input != BLACK && initial == BLACK) ++cluster;
            ++num_in_bounds;
        };

        if(boundary_up   ) update_search(row - 1u, col);
        if(boundary_down ) update_search(row + 1u, col);
        if(boundary_left ) update_search(row, col - 1u);
        if(boundary_right) update_search(row, col + 1u);

        if(boundary_up   && boundary_left ) update_search(row - 1u, col - 1u);
        if(boundary_up   && boundary_right) update_search(row - 1u, col + 1u);
        if(boundary_down && boundary_left ) update_search(row + 1u, col - 1u);
        if(boundary_down && boundary_right) update_search(row + 1u, col + 1u);

        neighborhood_size.push_back(neighborhood);
        cluster_size.push_back(cluster);
        times_invalid.push_back(invalid_freq[row][col]);
        on_boundary.push_back(num_in_bounds != permutation_score::NUM_ADJACENT_TILES);
    };

    // for calls to intersection_info() later
    vector<uint> old_lens;
    vector<uint> new_lens;

    // for calls to explore_adjacent() later
    vector<uint> neighborhood_size;
    vector<uint> cluster_size;
    vector<uint> times_invalid;
    vector<bool> boundary;

    // controls whether or not funcs are run again for mirrored tile
    bool added_symmetrical_black = false;

    // set black, then test for violations of grid restrictions
    puzzle[row][col].set_black();
    num_fillable_tiles--;

    // add second black tile if symmetric
    if(reqs.symmetric && puzzle[rows() - row - 1][cols() - col - 1].type != TILE_BLACK) {
        puzzle[rows() - row - 1][cols() - col - 1].set_black();
        num_fillable_tiles--;
        added_symmetrical_black = true;
    }

    // validate word lengths of intersection for this black tile
    if(!intersection_info(row, col, old_lens, new_lens)) {
        return std::nullopt;
    }

    // validate word lengths of intersection for mirrored black tile
    if(added_symmetrical_black && !intersection_info(rows() - row - 1, cols() - col - 1, old_lens, new_lens)) {
        return std::nullopt;
    }

    // enforce connectedness
    if(reqs.connected && !connected()) {
        return std::nullopt;
    }

    // make sure this isn't a duplicate grid
    string serialized = serialize_initial();
    if(explored_grids.count(serialized)) {
        return std::nullopt;
    }
    explored_grids.insert(std::move(serialized));

    // get scoring info
    explore_adjacent(row, col, neighborhood_size, cluster_size, times_invalid, boundary);
    cw_assert(neighborhood_size.size() == 1u);
    cw_assert(cluster_size.size()      == 1u);
    cw_assert(times_invalid.size()     == 1ul);
    cw_assert(boundary.size()          == 1ul);
    if(added_symmetrical_black) {
        explore_adjacent(rows() - row - 1, cols() - col - 1, neighborhood_size, cluster_size, times_invalid, boundary);
        cw_assert(neighborhood_size.size() >= 2 * 1u);
        cw_assert(cluster_size.size()      >= 2 * 1u);
        cw_assert(times_invalid.size()     == 2 * 1ul);
        cw_assert(boundary.size()          == 2 * 1ul);
        cw_assert(boundary[0] == boundary[1]);
    }

    invalid_freq = vector<vector<uint> >(height, vector<uint>(length, 0u));
    return std::make_optional(permutation_score(
        std::move(old_lens),
        std::move(new_lens),
        std::move(neighborhood_size),
        std::move(cluster_size),
        std::move(times_invalid),
        boundary[0],
        std::make_pair(row, col)
    ));
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
