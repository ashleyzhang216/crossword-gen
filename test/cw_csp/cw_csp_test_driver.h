// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: test driver declaration for cw_csp
// ==================================================================

#ifndef CW_CSP_TEST_DRIVER_H
#define CW_CSP_TEST_DRIVER_H

#include "../../src/common/common_data_types.h"
#include "../../src/common/common_parent.h"
#include "../../src/cw_csp/cw_csp_data_types.h"
#include "../../src/cw_csp/cw_csp.h"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace cw_csp_data_types_ns;
using namespace cw_csp_ns;

namespace cw_csp_test_driver_ns {
    class cw_csp_test_driver : public common_parent {
        public:
            // base constructor
            cw_csp_test_driver(string name);

            // csp initialization tests
            bool test_constructor_empty(uint length, uint height, string filepath, 
                unordered_set<cw_variable>& expected_variables,
                unordered_set<cw_constraint>& expected_constraints,
                unordered_map<cw_variable, unordered_set<cw_constraint> >& expected_arc_dependencies
            );
            bool test_constructor_contents(uint length, uint height, string contents, string filepath,
                unordered_set<cw_variable>& expected_variables,
                unordered_set<cw_constraint>& expected_constraints,
                unordered_map<cw_variable, unordered_set<cw_constraint> >& expected_arc_dependencies
            );

            // csp ac3 tests
            bool test_ac3_validity(uint length, uint height, string contents, string filepath, bool expected_result);
            bool test_ac3(uint length, uint height, string contents, string filepath, bool expected_result, unordered_set<cw_variable>* expected_variables);

            // csp solving w/ backtracking test
            bool test_backtracking_validity(uint length, uint height, string contents, string filepath, bool expected_result, bool do_print);
            
            // destructor
            ~cw_csp_test_driver() { delete dut; }

        private:
            // TODO: change to use unique_ptr
            cw_csp* dut = nullptr;
    }; // cw_csp_test_driver
} // cw_csp_test_driver_ns

#endif // CW_CSP_TEST_DRIVER_H