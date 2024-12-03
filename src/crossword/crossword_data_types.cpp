// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: crossword clas data type operators/functions
// ==================================================================

#include "crossword_data_types.h"

using namespace crossword_data_types_ns;

/**
 * @brief convert char to proper tile type it belongs in
 * @param c char to convert
 * @throws assertion_failure_exception iff c is not a valid cw char 
 * @return TILE_EMPTY iff c == WILDCARD, TILE_BLACK iff c == BLACK, TILE_FILLED otherwise
*/
cw_tile_type crossword_data_types_ns::char_to_tile_type(const char& c) {
    assert(c == WILDCARD || c == BLACK || (c >= 'a' && c <= 'z'));
    if(c == WILDCARD) return TILE_EMPTY;
    if(c == BLACK) return TILE_BLACK;
    return TILE_FILLED;
}

/**
 * @brief constructor for cw_tile, using input value from contents
 */
cw_tile::cw_tile(char c)
    : type(char_to_tile_type(c)),
      input_val(c),
      initial_val(c),
      across(std::nullopt),
      down(std::nullopt) {
    // do nothing else
}

/**
 * @brief set this fillable tile to black, without changing the input val
 */
void cw_tile::set_black() {
    assert(type != TILE_BLACK);

    initial_val = BLACK;
    type = char_to_tile_type(initial_val);
    assert(type == TILE_BLACK);

    across.reset();
    down.reset();
}

/**
 * @brief constructor for permutation_reqs with default values
 */
permutation_reqs::permutation_reqs()
    : symmetric(true),
      connected(true),
      min_new_word_len(MIN_WORD_LEN) {
    // do nothing
}

/**
 * @brief constructor for permutation_reqs with custom, potentially non-default values
 *
 * @param s value for symmetric
 * @param c value for connected
 * @param l value for min_new_word_len
 */
permutation_reqs::permutation_reqs(bool s, bool c, uint l)
    : symmetric(s),
      connected(c),
      min_new_word_len(l) {
    // do nothing
}

/**
 * @brief constructor for permutation_score with data from crossword::permute()
 *
 * @param ol value for old_lens
 * @param nl value for new_lens
 * @param n value for neighborhood_size
 * @param c value for cluster_size
 * @param t value for times_invalid
 * @param b value for on_boundary
 */
permutation_score::permutation_score(const vector<uint>& ol, const vector<uint>& nl, const vector<uint>& n, const vector<uint>& c, const vector<uint>& t, bool b)
    : old_lens(ol),
      new_lens(nl),
      neighborhood_size(n),
      cluster_size(c),
      times_invalid(t),
      on_boundary(b) {

    // only should measure adjacent 8 tiles
    for(auto v : n) assert(v <= NUM_ADJACENT_TILES);
    for(auto v : c) assert(v <= NUM_ADJACENT_TILES);

    // sort for easy comparison operator later
    std::sort(old_lens.begin(), old_lens.end());
    std::sort(new_lens.begin(), new_lens.end());
    std::sort(neighborhood_size.begin(), neighborhood_size.end());
    std::sort(cluster_size.begin(), cluster_size.end());
    std::sort(times_invalid.begin(), times_invalid.end());
}
