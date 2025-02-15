// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: common virtual class declaration
// ==================================================================

#ifndef COMMON_PARENT_H
#define COMMON_PARENT_H

#include "common_data_types.h"
#include "../utils/cw_utils.h"

using namespace common_data_types_ns;
using namespace cw;

namespace common_parent_ns {
    /**
     * @brief common class that all classes/test drivers inherit for universal utility access
    */
    class common_parent {
        public:
            // base constructor
            common_parent(const string_view& name, const verbosity_t& verbosity);

            // shorthand for test drivers only to print failed tests
            bool check_condition(const string_view& condition_name, bool condition);

        protected:
            cw::cw_utils utils;
            string name;
            verbosity_t verbosity;
    };
}

#endif // COMMON_PARENT_H