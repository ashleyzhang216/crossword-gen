// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class declaration for crossword constraint satisfaction problem 
// ==================================================================

#ifndef CW_CSP_H
#define CW_CSP_H

#include "cw_csp_data_types.h"
#include "../common/common_parent.h"
#include "../crossword/crossword_data_types.h"
#include "../crossword/crossword.h"

using namespace cw_csp_data_types_ns;
using namespace common_parent_ns;
using namespace crossword_data_types_ns;
using namespace crossword_ns;

namespace cw_csp_ns {
    /**
     * @brief class representation of a crossword constraint satisfaction problem
    */
    class cw_csp : public common_parent {
        public:
            // constructor w/o puzzle contents
            cw_csp(string name, uint length, uint height);

            // constructor with puzzle contents
            cw_csp(string name, uint length, uint height, string contents);
        
        protected:
            // populate variables & constraints
            void initialize_csp();

        private:
            // crossword to be solved
            unique_ptr<crossword> cw = nullptr;

            // csp structures
            unordered_set<cw_variable>     variables;
            unordered_set<cw_constraint>   constraints;

    }; // cw_csp
} // cw_csp_ns

#endif // CW_CSP_H