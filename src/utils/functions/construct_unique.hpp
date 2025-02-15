// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   2/15/2025
// Description: functions to construct STL containers of unique_ptrs
// ==================================================================

#ifndef CW_CONSTRUCT_UNIQUE_HPP
#define CW_CONSTRUCT_UNIQUE_HPP

#include "../../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw {
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
} // namespace cw

#endif // CW_CONSTRUCT_UNIQUE_HPP