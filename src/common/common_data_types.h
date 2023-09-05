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
#include <queue>
#include <stack>
#include <tuple>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

#define MIN_WORD_LEN 2  // max length for a single word
#define MAX_WORD_LEN 20 // max length for a single word
#define WILDCARD '?'    // tile/pattern where any letter can go
#define BLACK ' '       // tile where no letter can be placed
#define WCD WILDCARD    // shorthand
#define BLK BLACK       // shorthand

using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::string;
using std::stringstream;
using std::vector;
using std::queue;
using std::stack;
using std::tuple;
using std::ifstream;
using std::ostream;
using std::unordered_set;
using std::unordered_map;
using std::hash;

// RAII
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

namespace common_data_types_ns {

    // TODO: implement
    
} // common_data_types_ns

#endif // COMMON_DATA_TYPES_H