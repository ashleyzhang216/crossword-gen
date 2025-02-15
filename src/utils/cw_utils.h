// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module declaration
// ==================================================================

#ifndef CW_UTILS_H
#define CW_UTILS_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

#include "cw_assert.hpp"
#include "cw_containers.hpp"
#include "cw_logging.hpp"

using namespace cw; // TODO: temp

/**
 * @brief template function to compare contents of hashsets for testing, T must have << operator defined
 * 
 * @param lhs ref to lhs set
 * @param rhs ref to rhs set
 * @param debug_prints prints debug messages iff debug_prints == true
 * @returns true iff contents of lhs & rhs are identical
*/
template <typename T> 
inline bool set_contents_equal(const unordered_set<T>& lhs, const unordered_set<T>& rhs, bool debug_prints) {

    cw::cw_utils utils("set_contents_equal()", VERBOSITY);
    bool result = true;

    if(lhs.size() != rhs.size()) {
        if(debug_prints) {
            utils.log(WARNING, "mismatched size: ", lhs.size(), " & ", rhs.size());
        }
        result = false;
    }

    for(const T& t : lhs) {
        if(rhs.count(t) == 0) {
            if(debug_prints) {
                utils.log(WARNING, "missing from rhs: ", t);
            }
            result = false;
        }
    }

    // not necessary for correctness checking, only for debug
    for(const T& t : rhs) {
        if(lhs.count(t) == 0) {
            if(debug_prints) {
                utils.log(WARNING, "missing from lhs: ", t);
            }
            result = false;
        }
    }

    return result;
}

/**
 * @brief template function to compare contents of hashmap to hashset for testing
 * 
 * @param lhs ref to lhs map
 * @param rhs ref to rhs map
 * @param debug_prints prints debug messages iff debug_prints == true
 * @returns true iff contents of lhs & rhs are identical
*/
template <typename K, typename V>
inline bool map_to_set_contents_equal(const unordered_map<K, unordered_set<V> >& lhs, const unordered_map<K, unordered_set<V> >& rhs, bool debug_prints) {
    
    cw::cw_utils utils("map_to_set_contents_equal()", VERBOSITY);
    bool result = true;

    if(lhs.size() != rhs.size()) {
        if(debug_prints) {
            utils.log(WARNING, "mismatched size: ", lhs.size(), " & ", rhs.size());
        }
        result = false;
    }
    
    for(const auto& pair : lhs) {
        if(rhs.find(pair.first) == rhs.end()) {
            if(debug_prints) {
                utils.log(WARNING, "key missing from rhs: ", pair.first);
            }
            result = false;
            continue;
        }
        if(!set_contents_equal(rhs.at(pair.first), pair.second, debug_prints)) {
            if(debug_prints) {
                utils.log(WARNING, "set contents unequal for key: ", pair.first);
            }
            result = false;
        }
    }

    return result;
}

/**
 * @cite https://stackoverflow.com/a/73674943
*/
template <class T> auto move_to_unique(T&& t) {
    return std::make_unique<std::remove_reference_t<T>>(std::move(t));
}
template <class V, class ... Args> auto make_vector_unique(Args ... args) {
    std::vector<std::unique_ptr<V>> rv;
    (rv.push_back(move_to_unique(args)), ...);
    return rv;
}
template <class V, class ... Args> auto make_unordered_set_unique(Args ... args) {
    std::unordered_set<std::unique_ptr<V>> rv;
    (rv.insert(move_to_unique(args)), ...);
    return rv;
}

#endif // CW_UTILS_H