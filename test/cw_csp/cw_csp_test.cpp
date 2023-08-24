// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: Catch2 tests for cw_csp class
// ==================================================================

#include "../catch/catch.hpp"
#include "cw_csp_test_driver.h"

using namespace cw_csp_test_driver_ns;

/**
 * hello world test for cw_csp
*/
TEST_CASE("cw_csp hello_world", "[cw_csp],[hello_world],[quick]") {
    REQUIRE(true);
}

/**
 * constructor test for cw_csp without contents
*/
TEST_CASE("cw_csp constructor_no_contents", "[cw_csp],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp constructor_no_contents");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";

    vector<cw_variable> vars_1_1 = {
        cw_variable(0, 0, 1, VERTICAL,   {"a"}),
        cw_variable(0, 0, 1, HORIZONTAL, {"a"})
    };
    vector<cw_constraint> contrs_1_1 = {
        cw_constraint(0, 0, make_shared<cw_variable>(vars_1_1[0]), make_shared<cw_variable>(vars_1_1[1])),
        cw_constraint(0, 0, make_shared<cw_variable>(vars_1_1[1]), make_shared<cw_variable>(vars_1_1[0]))
    };
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_1_1 = {
        {vars_1_1[0], {vars_1_1[1]}},
        {vars_1_1[1], {vars_1_1[0]}},
    };
    unordered_set<cw_variable>   vars_1_1_set  (vars_1_1.begin(),   vars_1_1.end());
    unordered_set<cw_constraint> contrs_1_1_set(contrs_1_1.begin(), contrs_1_1.end());
    REQUIRE(dut->test_constructor_empty(1, 1, dict_barebones_path, &vars_1_1_set, &contrs_1_1_set, &arc_dep_1_1));
}