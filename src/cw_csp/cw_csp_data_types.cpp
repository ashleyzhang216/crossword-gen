// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: constraint satisfaction problem class data type operators/functions
// ==================================================================

#include "cw_csp_data_types.h"

using namespace cw_csp_data_types_ns;

// ############### cw_variable ###############

/**
 * @brief == operator for cw_variable to use in hashset
*/
bool cw_variable::operator==(const cw_variable& rhs) const {
    return origin_row == rhs.origin_row
        && origin_col == rhs.origin_col
        && dir == rhs.dir;
}

/**
 * @brief hash function for cw_variable
*/
namespace std {
    size_t hash<cw_variable>::operator()(const cw_variable& var) const {
        size_t hash_row = hash<uint>{}(var.origin_row);
        size_t hash_col = hash<uint>{}(var.origin_col);
        return hash_row ^ hash_col; 
    }
}

/**
 * @brief constructor for cw_variable to be used by cw_csp
 * 
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param pattern word pattern to find matches for to populate domain
 * @param finder ptr to word_finder to populate domain
*/
cw_variable::cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, shared_ptr<word_finder> finder) {
    this->origin_row = origin_row;
    this->origin_col = origin_col;
    this->length = length;
    this->dir = dir;
    domain.clear();

    // populate domain
    finder->find_matches(&domain, pattern);
}

// ############### cw_constraint ###############

/**
 * @brief == operator for cw_constraint to use in hashset
*/
bool cw_constraint::operator==(const cw_constraint& other) const {
    return lhs_index == other.lhs_index
        && rhs_index == other.rhs_index
        && *lhs == (*other.lhs)
        && *rhs == (*other.rhs);
}

/**
 * @brief hash function for cw_constraint
*/
namespace std {
    size_t hash<cw_constraint>::operator()(const cw_constraint& var) const {
        return hash<uint>{}(var.lhs_index) ^ hash<uint>{}(var.rhs_index) ^ hash<cw_variable>{}(*(var.lhs)) ^ hash<cw_variable>{}(*(var.rhs));
    }
}