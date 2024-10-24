// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: data types for constraint satisfaction problem class
// ==================================================================

#ifndef CW_CSP_DATA_TYPES_H
#define CW_CSP_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../word_domain/word_domain_data_types.h"
#include "../word_domain/word_domain.h"

using namespace common_data_types_ns;
using namespace word_domain_data_types_ns;
using namespace word_domain_ns;

namespace cw_csp_data_types_ns {

    // direction a word can be placed in a crossword
    enum word_direction {
        HORIZONTAL = 0,
        VERTICAL   = 1,
    };

    // for more interpretable prints
    const unordered_map<word_direction, string> word_dir_name = {
        {HORIZONTAL, "HORIZONTAL"},
        {VERTICAL, "VERTICAL"}
    };

    // to choose between solving strategies, when more are added in the future
    enum csp_solving_strategy {
        BACKTRACKING = 0,
    };

    // to choose between variable selection method, when more are added in the future
    enum var_selection_method {
        MIN_REMAINING_VALUES = 0,
    };

    // a variable in a constraint satisfaction problem
    struct cw_variable {
        uint origin_row;       // 0-indexed
        uint origin_col;       // 0-indexed
        uint length;           // >= MIN_WORD_LEN && <= MAX_WORD_LEN
        word_direction dir;    // direction of word of this var
        string pattern;        // original pattern used to populate domain
        word_domain domain;    // all possible words that fit

        // standard constructor for cw_csp
        cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, unordered_set<word_t> domain);

        // testing-only constructor
        cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, unordered_set<word_t> domain);

        // equality operator, TODO: is this needed?
        bool operator==(const cw_variable& rhs) const;

        // operator to print out cw_variable for debug
        friend ostream& operator<<(ostream& os, const cw_variable& var);
    };

    // equality constraints between 2 letters in 2 cw vars
    // uni-directional, in constraint set both a constraint and its reverse must both exist
    struct cw_constraint {
        uint lhs_index; // index of shared letter in lhs
        uint rhs_index; // index of shared letter in rhs
        shared_ptr<cw_variable> lhs = nullptr;
        shared_ptr<cw_variable> rhs = nullptr;

        // blank constructor for initialization
        cw_constraint() {}
        
        // value constructor
        cw_constraint(uint lhs_index, uint rhs_index, shared_ptr<cw_variable> lhs, shared_ptr<cw_variable> rhs);

        // AC-3 step; remove all words in lhs domain that don't have a corresponding rhs word in its domain
        bool prune_domain(); 

        // used by solved() in cw_csp to check that this constraint is satisfied
        bool satisfied() const;
        
        // equality operator, TODO: is this needed?
        bool operator==(const cw_constraint& rhs) const;

        // operator to print out cw_constraint for debug
        friend ostream& operator<<(ostream& os, const cw_constraint& var);
    };

} // cw_csp_data_types_ns

/**
 * hash function specialization declarations
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
} // std


#endif // CW_CSP_DATA_TYPES_H