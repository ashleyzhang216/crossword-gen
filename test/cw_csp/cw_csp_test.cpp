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

    // 1x1 blank crossword
    vector<cw_variable> vars_1_1 = {}; // no words with len > 1
    vector<cw_constraint> contrs_1_1 = {}; // no variables
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_1_1 = {}; // no constraints
    unordered_set<cw_variable>   vars_1_1_set  (vars_1_1.begin(),   vars_1_1.end());
    unordered_set<cw_constraint> contrs_1_1_set(contrs_1_1.begin(), contrs_1_1.end());
    REQUIRE(dut->test_constructor_empty(1, 1, dict_barebones_path, &vars_1_1_set, &contrs_1_1_set, &arc_dep_1_1));

    // 2x2 blank crossword
    vector<cw_variable> vars_2_2 = {
        cw_variable(0, 0, 2, VERTICAL,   {"an", "at", "on", "to"}),
        cw_variable(0, 1, 2, VERTICAL,   {"an", "at", "on", "to"}),
        cw_variable(0, 0, 2, HORIZONTAL, {"an", "at", "on", "to"}),
        cw_variable(1, 0, 2, HORIZONTAL, {"an", "at", "on", "to"}),
    };
    vector<cw_constraint> contrs_2_2 = {
        // forward (vertical --> horizontal)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_2_2[0]), make_shared<cw_variable>(vars_2_2[2])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_2_2[1]), make_shared<cw_variable>(vars_2_2[2])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_2_2[0]), make_shared<cw_variable>(vars_2_2[3])),
        cw_constraint(1, 1, make_shared<cw_variable>(vars_2_2[1]), make_shared<cw_variable>(vars_2_2[3])),

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_2_2[2]), make_shared<cw_variable>(vars_2_2[0])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_2_2[2]), make_shared<cw_variable>(vars_2_2[1])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_2_2[3]), make_shared<cw_variable>(vars_2_2[0])),
        cw_constraint(1, 1, make_shared<cw_variable>(vars_2_2[3]), make_shared<cw_variable>(vars_2_2[1])),
    };
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_2_2 = {
        {vars_2_2[0], {vars_2_2[2], vars_2_2[3]}},
        {vars_2_2[1], {vars_2_2[2], vars_2_2[3]}},
        {vars_2_2[2], {vars_2_2[0], vars_2_2[1]}},
        {vars_2_2[3], {vars_2_2[0], vars_2_2[1]}},
    };
    unordered_set<cw_variable>   vars_2_2_set  (vars_2_2.begin(),   vars_2_2.end());
    unordered_set<cw_constraint> contrs_2_2_set(contrs_2_2.begin(), contrs_2_2.end());
    REQUIRE(dut->test_constructor_empty(2, 2, dict_barebones_path, &vars_2_2_set, &contrs_2_2_set, &arc_dep_2_2));

    // 3x3 blank crossword
    vector<cw_variable> vars_3_3 = {
        cw_variable(0, 0, 3, VERTICAL,   {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(0, 1, 3, VERTICAL,   {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(0, 2, 3, VERTICAL,   {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(0, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(1, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(2, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
    };
    vector<cw_constraint> contrs_3_3 = {
        // forward (vertical --> horizontal)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3[0]), make_shared<cw_variable>(vars_3_3[3])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_3[1]), make_shared<cw_variable>(vars_3_3[3])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3[2]), make_shared<cw_variable>(vars_3_3[3])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_3[0]), make_shared<cw_variable>(vars_3_3[4])),
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_3[1]), make_shared<cw_variable>(vars_3_3[4])),
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_3[2]), make_shared<cw_variable>(vars_3_3[4])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3[0]), make_shared<cw_variable>(vars_3_3[5])),
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_3[1]), make_shared<cw_variable>(vars_3_3[5])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3[2]), make_shared<cw_variable>(vars_3_3[5])),

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3[3]), make_shared<cw_variable>(vars_3_3[0])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_3[3]), make_shared<cw_variable>(vars_3_3[1])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3[3]), make_shared<cw_variable>(vars_3_3[2])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_3[4]), make_shared<cw_variable>(vars_3_3[0])),
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_3[4]), make_shared<cw_variable>(vars_3_3[1])),
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_3[4]), make_shared<cw_variable>(vars_3_3[2])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3[5]), make_shared<cw_variable>(vars_3_3[0])),
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_3[5]), make_shared<cw_variable>(vars_3_3[1])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3[5]), make_shared<cw_variable>(vars_3_3[2])),
        
    };
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_3_3 = {
        {vars_3_3[0], {vars_3_3[3], vars_3_3[4], vars_3_3[5]}},
        {vars_3_3[1], {vars_3_3[3], vars_3_3[4], vars_3_3[5]}},
        {vars_3_3[2], {vars_3_3[3], vars_3_3[4], vars_3_3[5]}},
        {vars_3_3[3], {vars_3_3[0], vars_3_3[1], vars_3_3[2]}},
        {vars_3_3[4], {vars_3_3[0], vars_3_3[1], vars_3_3[2]}},
        {vars_3_3[5], {vars_3_3[0], vars_3_3[1], vars_3_3[2]}},
    };
    unordered_set<cw_variable>   vars_3_3_set  (vars_3_3.begin(),   vars_3_3.end());
    unordered_set<cw_constraint> contrs_3_3_set(contrs_3_3.begin(), contrs_3_3.end());
    REQUIRE(dut->test_constructor_empty(3, 3, dict_barebones_path, &vars_3_3_set, &contrs_3_3_set, &arc_dep_3_3));

    // 3x4 blank crossword
    vector<cw_variable> vars_3_4 = {
        cw_variable(0, 0, 4, VERTICAL,   {"cars", "caps", "core", "door", "boob", "been", "node"}),
        cw_variable(0, 1, 4, VERTICAL,   {"cars", "caps", "core", "door", "boob", "been", "node"}),
        cw_variable(0, 2, 4, VERTICAL,   {"cars", "caps", "core", "door", "boob", "been", "node"}),
        cw_variable(0, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(1, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(2, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(3, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
    };
    vector<cw_constraint> contrs_3_4 = {
        // forward (vertical --> horizontal)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[3])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[3])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[3])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[4])),
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[4])),
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[4])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[5])),
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[5])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[5])),
        cw_constraint(3, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[6])),
        cw_constraint(3, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[6])),
        cw_constraint(3, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[6])),
        
        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[0])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[1])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[2])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[0])),
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[1])),
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[2])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[0])),
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[1])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[2])),
        cw_constraint(0, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[0])),
        cw_constraint(1, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[1])),
        cw_constraint(2, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[2])),
    };
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_3_4 = {
        {vars_3_4[0], {vars_3_4[3], vars_3_4[4], vars_3_4[5], vars_3_4[6]}},
        {vars_3_4[1], {vars_3_4[3], vars_3_4[4], vars_3_4[5], vars_3_4[6]}},
        {vars_3_4[2], {vars_3_4[3], vars_3_4[4], vars_3_4[5], vars_3_4[6]}},
        {vars_3_4[3], {vars_3_4[0], vars_3_4[1], vars_3_4[2]}},
        {vars_3_4[4], {vars_3_4[0], vars_3_4[1], vars_3_4[2]}},
        {vars_3_4[5], {vars_3_4[0], vars_3_4[1], vars_3_4[2]}},
        {vars_3_4[6], {vars_3_4[0], vars_3_4[1], vars_3_4[2]}},
    };
    unordered_set<cw_variable>   vars_3_4_set  (vars_3_4.begin(),   vars_3_4.end());
    unordered_set<cw_constraint> contrs_3_4_set(contrs_3_4.begin(), contrs_3_4.end());
    REQUIRE(dut->test_constructor_empty(3, 4, dict_barebones_path, &vars_3_4_set, &contrs_3_4_set, &arc_dep_3_4));
}