// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: operator implementations for common data types
// ==================================================================

#include "common_data_types.h"

using namespace common_data_types_ns;

/**
 * @brief operator to print word candidates for testing
*/
ostream& common_data_types_ns::operator<<(ostream& os, const word_t& w) {
    os << w.word;
    return os;
}

/**
 * @brief hash function for word_t
*/
size_t hash<word_t>::operator()(const word_t& w) const {
    return hash<string>{}(w.word);
}
