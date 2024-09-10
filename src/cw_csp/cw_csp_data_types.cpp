// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: constraint satisfaction problem class data type operators/functions
// ==================================================================

#include "cw_csp_data_types.h"

using namespace cw_csp_data_types_ns;

// ############### set_contents_equal() ###############

template bool set_contents_equal<cw_variable>(
    const unordered_set<cw_variable>& lhs, 
    const unordered_set<cw_variable>& rhs, 
    bool debug_prints
);
template bool set_contents_equal<cw_constraint>(
    const unordered_set<cw_constraint>& lhs, 
    const unordered_set<cw_constraint>& rhs, 
    bool debug_prints
);

// ############### map_to_set_contents_equal() ###############

template bool map_to_set_contents_equal(
    const unordered_map<cw_variable, unordered_set<cw_constraint> >& lhs, 
    const unordered_map<cw_variable, unordered_set<cw_constraint> >& rhs, 
    bool debug_prints
);

// ############### cw_variable ###############

/**
 * @brief == operator for cw_variable to use in hashset
*/
bool cw_variable::operator==(const cw_variable& rhs) const {
    return origin_row == rhs.origin_row
        && origin_col == rhs.origin_col
        && length == rhs.length  
        && dir == rhs.dir;
        // TODO: this needs to check domain equality for test drivers
}

/**
 * @brief hash function for cw_variable
*/
size_t std::hash<cw_variable>::operator()(const cw_variable& var) const {
    size_t hash_row = hash<uint>{}(var.origin_row);
    size_t hash_col = hash<uint>{}(var.origin_col);
    return hash_row ^ hash_col; 
}

/**
 * @brief operator to print out cw_variable for debug
*/
ostream& cw_csp_data_types_ns::operator<<(ostream& os, const cw_variable& var) {
    os << "row: " << var.origin_row << ", col: " << var.origin_col << ", len: " << var.length
       << ", dir: " << cw_csp_data_types_ns::word_dir_name.at(var.dir) << ", pattern: " 
       << var.pattern << ", domain: {";
    for(word_t w : var.domain.get_cur_domain()) {
        os << w.word << ", ";
    }
    os << "}";
    return os;
}

/**
 * @brief constructor for cw_variable to be used by cw_csp
 * 
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param pattern word pattern to find matches for to populate domain
 * @param domain contents of domain of this var
*/
cw_variable::cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, unordered_set<word_t> domain) 
    : origin_row(origin_row),
      origin_col(origin_col),
      length(length),
      dir(dir),
      pattern(pattern),
      domain("cw_variable domain", domain) {
    // do nothing else
}

/**
 * @brief testing-only constructor for cw_variable to be used by test driver
 * 
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param domain contents of domain of this var
*/
cw_variable::cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, unordered_set<word_t> domain) 
    : cw_variable(origin_row, origin_col, length, dir, "(created w/ testing constructor)", domain) {
    // do nothing, delegated to constructor that does everything needed
}

// ############### cw_constraint ###############

/**
 * @brief == operator for cw_constraint to use in hashset
*/
bool cw_constraint::operator==(const cw_constraint& other) const {
    return lhs_index == other.lhs_index
        && rhs_index == other.rhs_index
        && lhs == other.lhs
        && rhs == other.rhs;
}

/**
 * @brief hash function for cw_constraint
*/
size_t std::hash<cw_constraint>::operator()(const cw_constraint& var) const {
    return hash<uint>{}(var.lhs_index) ^ hash<uint>{}(var.rhs_index) ^ hash<size_t>{}(var.lhs) ^ hash<size_t>{}(var.rhs);
}

/**
 * @brief operator to print out cw_constraint for debug
*/
ostream& cw_csp_data_types_ns::operator<<(ostream& os, const cw_constraint& var) {
    os << "lhs: " << var.lhs << " @ index " << var.lhs_index
       << ", rhs: " << var.rhs << " @ index " << var.rhs_index;
    return os;
}

/**
 * @brief testing-only constructor for cw_constraint to be used by test driver
 * 
 * @param lhs_index value of lhs_index to populate
 * @param rhs_index value of rhs_index to populate
 * @param lhs value of lhs index in an id_obj_manager to populate
 * @param rhs value of rhs index in an id_obj_manager to populate
*/
cw_constraint::cw_constraint(uint lhs_index, uint rhs_index, size_t lhs, size_t rhs) 
    : lhs_index(lhs_index), rhs_index(rhs_index), lhs(lhs), rhs(rhs)
{
    // do nothing, initialization is sufficient
}

/**
 * @brief AC-3 step to prune words in lhs domain without valid rhs words
 * 
 * @param  vars ref to id_obj_manager to index into to get lhs/rhs vars
 * @return true iff 1 or more words pruned, i.e. domain changed
*/
bool cw_constraint::prune_domain(id_obj_manager<cw_variable>& vars) {

    size_t num_removed = 0;
    for(char letter : vars[lhs]->domain.get_all_letters_at_index(lhs_index)) {
        if(vars[rhs]->domain.num_letters_at_index(rhs_index, letter) == 0) {
            // cannot satisfy constraint for this letter
            num_removed += vars[lhs]->domain.remove_matching_words(lhs_index, letter);
        }
    }

    return num_removed > 0;
}

/**
 * @brief checks that constraint is satisfied, used by solved() in cw_csp
 * 
 * @param  vars ref to id_obj_manager to index into to get lhs/rhs vars 
 * @return true iff constraint is satisfied
*/
bool cw_constraint::satisfied(const id_obj_manager<cw_variable>& vars) const {
    if(vars[lhs]->domain.size() != 1) return false;
    if(vars[rhs]->domain.size() != 1) return false;

    // since ac3() undoes invalid assignments, this should always be true
    assert_m(vars[lhs]->domain.get_cur_domain().at(0) != vars[rhs]->domain.get_cur_domain().at(0), "word equality between constrainted vars");
    assert_m(vars[lhs]->domain.get_cur_domain().at(0).word.at(lhs_index) == vars[rhs]->domain.get_cur_domain().at(0).word.at(rhs_index), "letter inequality at constraint");

    return true;
}