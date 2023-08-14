// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal common data types
// ==================================================================

#ifndef COMMON_DATA_TYPES_H
#define COMMON_DATA_TYPES_H

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

#define MAX_WORD_LEN 20
#define PATTERN_PLACEHOLDER '?'

using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::string;
using std::stringstream;
using std::vector;
using std::ifstream;
using std::unordered_set;
using std::unordered_map;

namespace common_data_types_ns {

    // TODO: template this function for all datatypes
    // compare contents of string hashsets
    bool string_sets_equal(const unordered_set<string>* lhs, const unordered_set<string>* rhs);
    
} // common_data_types_ns

#endif // COMMON_DATA_TYPES_H