// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: common virtual class implementation
// ==================================================================

#include "common_parent.h"

using namespace common_parent_ns;

/**
 * @brief common constructor for all cw objects and test drivers
 * 
 * @param name the name of this object
 * @param min_verbosity the min verbosity for the util object
*/
common_parent::common_parent(const string_view& name, const verbosity_t& min_verbosity) 
    : name(name), 
      utils(make_unique<cw_utils>(name, min_verbosity)) {
    // do nothing, initializer list is sufficient
}

/**
 * @brief shorthand for test drivers; prints error message if condition is false, returns condition
 * 
 * @param condition_name name of this condition
 * @param condition the condition to test
*/
bool common_parent::check_condition(const string_view& condition_name, bool condition) {
    if(!condition) {
        utils->log(WARNING, "failed condition: ", condition_name);
    }

    return condition;
}