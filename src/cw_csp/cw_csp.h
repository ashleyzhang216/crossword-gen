// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class declaration for crossword constraint satisfaction problem 
// ==================================================================

#ifndef CW_CSP_H
#define CW_CSP_H

#include "cw_csp_data_types.h"
#include "../common/common_parent.h"

using namespace cw_csp_data_types_ns;
using namespace common_parent_ns;

namespace cw_csp_ns {
    /**
     * @brief class representation of a crossword constraint satisfaction problem
    */
    class cw_csp : public common_parent {
        public:
            // base constructor
            cw_csp(string name);

        private:
            // csp structures
            unordered_set<cw_variable>     variables;
            unordered_set<cw_constraint>   constraints;

    }; // cw_csp
} // cw_csp_ns

#endif // CW_CSP_H