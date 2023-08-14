// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: operator implementations for common data types
// ==================================================================

#include "common_data_types.h"

using namespace common_data_types_ns;

/**
 * @brief compares contents of two string hashsets
 * 
 * @param lhs ptr to lhs set
 * @param rhs ptr to rhs set
 * @return true iff contents of lhs and rhs are identical
*/
bool common_data_types_ns::string_sets_equal(const unordered_set<string>* lhs, const unordered_set<string>* rhs) {

    if(lhs->size() != rhs->size()) return false;

    for(string s : *lhs) {
        if(rhs->count(s) == 0) return false;
    }

    return true;
}