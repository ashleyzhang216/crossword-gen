// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: data types for constraint satisfaction problem class
// ==================================================================

#ifndef CW_CSP_DATA_TYPES_H
#define CW_CSP_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../word_finder/word_finder_data_types.h"
#include "../word_finder/word_finder.h"

using namespace common_data_types_ns;
using namespace word_finder_data_types_ns;
using namespace word_finder_ns;

namespace cw_csp_data_types_ns {

    // direction a word can be placed in a crossword
    enum word_direction {
        HORIZONTAL = 0,
        VERTICAL   = 1,
    };

    const unordered_map<word_direction, string> word_dir_name = {
        {HORIZONTAL, "HORIZONTAL"},
        {VERTICAL, "VERTICAL"}
    };

    // a variable in a constraint satisfaction problem
    typedef struct cw_variable {
        uint origin_row;              // 0-indexed
        uint origin_col;              // 0-indexed
        uint length;                  // >= 0
        word_direction dir;           // direction of word of this var
        string pattern;               // original pattern used to populate domain
        unordered_set<string> domain; // all possible words that fit

        // standard constructor for cw_csp
        cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, shared_ptr<word_finder> finder);

        // testing-only constructor
        cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, unordered_set<string> domain);

        // for AC-3 based CSP reduction
        bool has_letter_at_pos(const char& letter, const uint& letter_pos) const;

        bool operator==(const cw_variable& rhs) const;
    } cw_variable;

    // operator to print out cw_variable for debug
    ostream& operator<<(ostream& os, const cw_variable& var);

    // equality constraints between 2 letters in 2 cw vars
    // uni-directional, in constraint set both a constraint and its reverse must both exist
    typedef struct cw_constraint {
        uint lhs_index; // index of shared letter in lhs
        uint rhs_index; // index of shared letter in rhs
        shared_ptr<cw_variable> lhs = nullptr;
        shared_ptr<cw_variable> rhs = nullptr;

        // blank constructor for initialization
        cw_constraint() {}
        
        // value constructor
        cw_constraint(uint lhs_index, uint rhs_index, shared_ptr<cw_variable> lhs, shared_ptr<cw_variable> rhs);

        // AC-3 step; remove all words in lhs domain that don't have a corresponding rhs word in its domain
        unordered_set<string> prune_domain(); 
        
        bool operator==(const cw_constraint& rhs) const;
    } cw_constraint;

    // operator to print out cw_constraint for debug
    ostream& operator<<(ostream& os, const cw_constraint& var);

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