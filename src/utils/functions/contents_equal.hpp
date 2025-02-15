// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   2/15/2025
// Description: equality comparators between contents of STL containers
// ==================================================================

#ifndef CW_CONTENTS_EQUAL_HPP
#define CW_CONTENTS_EQUAL_HPP

namespace cw {
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
}

#endif // CW_CONTENTS_EQUAL_HPP