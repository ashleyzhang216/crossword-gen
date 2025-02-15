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
    cw_assert(c == WILDCARD || c == BLACK || (c >= 'a' && c <= 'z'));
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
    cw_assert(type != TILE_BLACK);

    initial_val = BLACK;
    type = char_to_tile_type(initial_val);
    cw_assert(type == TILE_BLACK);

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
 * @param l value for loc
 */
permutation_score::permutation_score(const vector<uint>& ol, const vector<uint>& nl, const vector<uint>& n, const vector<uint>& c, const vector<uint>& t, bool b, const pair<uint, uint>& l)
    : old_lens(ol),
      new_lens(nl),
      neighborhood_size(n),
      cluster_size(c),
      times_invalid(t),
      on_boundary(b),
      loc(l) {

    // only should measure adjacent 8 tiles
    for(auto v : n) cw_assert(v <= NUM_ADJACENT_TILES);
    for(auto v : c) cw_assert(v <= NUM_ADJACENT_TILES);

    // sort for easy comparison operator later
    std::sort(old_lens.begin(), old_lens.end());
    std::sort(new_lens.begin(), new_lens.end());
    std::sort(neighborhood_size.begin(), neighborhood_size.end());
    std::sort(cluster_size.begin(), cluster_size.end());
    std::sort(times_invalid.begin(), times_invalid.end());
}

/**
 * @brief comparison operator for permutation_score
 */
bool permutation_score::operator<(const permutation_score& other) const {
    // refer to permutation_score declaration for priority

    // ############### old_lens ###############

    // first priority is # of elements
    if(old_lens.size() != other.old_lens.size()) {
        return old_lens.size() < other.old_lens.size();
    }

    // compare products
    uint lhs_ol_product = 1u;
    uint rhs_ol_product = 1u;
    for(uint v : old_lens) lhs_ol_product *= v;
    for(uint v : other.old_lens) rhs_ol_product *= v;
    if(lhs_ol_product != rhs_ol_product) {
        return lhs_ol_product < rhs_ol_product;
    }

    // ############### new_lens ###############

    // first priority is # of elements
    if(new_lens.size() != other.new_lens.size()) {
        return new_lens.size() < other.new_lens.size();
    }

    // compare products
    uint lhs_nl_product = 1u;
    uint rhs_nl_product = 1u;
    for(uint v : new_lens) lhs_nl_product *= v;
    for(uint v : other.new_lens) rhs_nl_product *= v;
    if(lhs_nl_product != rhs_nl_product) {
        return lhs_nl_product < rhs_nl_product;
    }

    // ############### neighborhood_size ###############

    // first priority is # of elements
    if(neighborhood_size.size() != other.neighborhood_size.size()) {
        return neighborhood_size.size() < other.neighborhood_size.size();
    }

    // compare sums
    uint lhs_ns_sum = std::accumulate(neighborhood_size.begin(), neighborhood_size.end(), 0u);
    uint rhs_ns_sum = std::accumulate(other.neighborhood_size.begin(), other.neighborhood_size.end(), 0u);
    if(lhs_ns_sum != rhs_ns_sum) {
        return lhs_ns_sum > rhs_ns_sum; // lower better
    }

    // ############### cluster_size ###############

    // first priority is # of elements
    if(cluster_size.size() != other.cluster_size.size()) {
        return cluster_size.size() < other.cluster_size.size();
    }

    // compare sums
    uint lhs_cs_sum = std::accumulate(cluster_size.begin(), cluster_size.end(), 0u);
    uint rhs_cs_sum = std::accumulate(other.cluster_size.begin(), other.cluster_size.end(), 0u);
    if(lhs_cs_sum != rhs_cs_sum) {
        return lhs_cs_sum < rhs_cs_sum; // higher better
    }

    // ############### times_invalid ###############

    // first priority is # of elements
    if(times_invalid.size() != other.times_invalid.size()) {
        return times_invalid.size() < other.times_invalid.size();
    }

    // compare sums
    uint lhs_ti_sum = std::accumulate(times_invalid.begin(), times_invalid.end(), 0u);
    uint rhs_ti_sum = std::accumulate(other.times_invalid.begin(), other.times_invalid.end(), 0u);
    if(lhs_ti_sum != rhs_ti_sum) {
        return lhs_ti_sum < rhs_ti_sum; // higher better
    }

    // ############### on_boundary ###############

    if(on_boundary != other.on_boundary) {
        return on_boundary; // false is better
    }

    // ############### loc tiebreaker ###############

    // tiebreaker, arbitrarily prioritize earlier black tiles
    if(loc.first != other.loc.first) {
        return loc.first < other.loc.first;
    }
    return loc.second < other.loc.second;
}

/**
 * @brief non-member operator< for permutation_score in crossword_data_types_ns
 */
bool crossword_data_types_ns::operator<(const permutation_score& lhs, const permutation_score& rhs) {
    return lhs.operator<(rhs);
}

/**
 * @brief operator to print out permutation_score for debug
 */
ostream& crossword_data_types_ns::operator<<(ostream& os, const permutation_score& s) {
    os << "permutation_score{";

    os << "old_lens=(";
    for(uint v : s.old_lens) os << v << ",";
    os << "), ";

    os << "new_lens=(";
    for(uint v : s.new_lens) os << v << ",";
    os << "), ";

    os << "neighborhood_size=(";
    for(uint v : s.neighborhood_size) os << v << ",";
    os << "), ";

    os << "cluster_size=(";
    for(uint v : s.cluster_size) os << v << ",";
    os << "), ";

    os << "times_invalid=(";
    for(uint v : s.times_invalid) os << v << ",";
    os << "), ";

    os << "on_boundary=" << s.on_boundary << ", ";

    os << "loc=(" << s.loc.first << ", " << s.loc.second << ")";

    return os;
}
