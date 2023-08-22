// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: data types for constraint satisfaction problem class
// ==================================================================

#ifndef CW_CSP_DATA_TYPES_H
#define CW_CSP_DATA_TYPES_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw_csp_data_types_ns {

    // direction a word can be placed in a crossword
    enum word_direction {
        HORIZONTAL = 0,
        VERTICAL   = 1,
    };

    // a variable in a constraint satisfaction problem
    typedef struct cw_variable {
        uint origin_row;              // 0-indexed
        uint origin_col;              // 0-indexed
        uint length;                  // >= 0
        word_direction dir;           // direction of word of this var
        unordered_set<string> domain; // all possible words that fit

        bool operator==(const cw_variable& rhs) const;
    } cw_variable;

    //size_t hash(const cw_variable& var) const;

    // equality constraints between 2 letters in 2 cw vars
    typedef struct cw_constraint {
        uint lhs_index; // index of shared letter in lhs
        uint rhs_index; // index of shared letter in rhs
        shared_ptr<cw_variable> lhs; 
        shared_ptr<cw_variable> rhs; 
        
        bool operator==(const cw_constraint& rhs) const;
    } cw_constraint;

} // cw_csp_data_types_ns

/**
 * hash function declarations in std for brevity
*/
namespace std {
    template <>
    struct hash<cw_csp_data_types_ns::cw_variable> {
        size_t operator()(const cw_csp_data_types_ns::cw_variable& var) const;
    };

    template <>
    struct hash<cw_csp_data_types_ns::cw_constraint> {
        size_t operator()(const cw_csp_data_types_ns::cw_constraint& var) const;
    };
}

#endif // CW_CSP_DATA_TYPES_H