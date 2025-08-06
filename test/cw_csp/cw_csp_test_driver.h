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
            bool test_constructor_empty(uint length, uint height, const std::filesystem::path& filepath,
                const unordered_set<unique_ptr<cw_variable> >& expected_variables,
                const unordered_set<unique_ptr<cw_constraint> >& expected_constraints,
                const unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& expected_constr_dependencies
            );
            bool test_constructor_contents(uint length, uint height, string contents, const std::filesystem::path& filepath,
                const unordered_set<unique_ptr<cw_variable> >& expected_variables,
                const unordered_set<unique_ptr<cw_constraint> >& expected_constraints,
                const unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& expected_constr_dependencies
            );

            // csp ac3 tests
            bool test_ac3_validity(uint length, uint height, string contents, const std::filesystem::path& filepath, bool expected_result);
            bool test_ac3(uint length, uint height, string contents, const std::filesystem::path& filepath, bool expected_result, const unordered_set<unique_ptr<cw_variable> >& expected_variables);

            // csp solving w/ backtracking test
            bool test_backtracking_validity(uint length, uint height, string contents, const std::filesystem::path& filepath, bool expected_result, bool do_print);

        private:
            unique_ptr<cw_csp> dut;
    }; // cw_csp_test_driver

    // helper for constructing ground truth constraint dependencies
    template<typename... Args>
    void insert_into_constr_dep(
        unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& constr_dep,
        vector<unique_ptr<cw_variable> >& vars, size_t var_idx,
        vector<unique_ptr<cw_constraint> >& contrs, Args... args
    ) {
        auto insert_into_constr_dep_impl = [](
            auto& self, unordered_set<unique_ptr<cw_constraint> >& contrs_set, 
            vector<unique_ptr<cw_constraint> >& contrs, auto idx, auto... rest
        ) {
            contrs_set.insert(contrs.at(static_cast<size_t>(idx))->clone());
            if constexpr(sizeof...(rest) > 0) {
                self(self, contrs_set, contrs, rest...);
            }
        };

        constr_dep[vars[var_idx]->clone()] = unordered_set<unique_ptr<cw_constraint> >();
        insert_into_constr_dep_impl(insert_into_constr_dep_impl, constr_dep[vars[var_idx]->clone()], contrs, args...);
    }
} // cw_csp_test_driver_ns

#endif // CW_CSP_TEST_DRIVER_H