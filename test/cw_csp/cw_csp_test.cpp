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
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_1_1 = {}; // no constraints
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
        cw_constraint(0, 0, make_shared<cw_variable>(vars_2_2[0]), make_shared<cw_variable>(vars_2_2[2])), // 0
        cw_constraint(0, 1, make_shared<cw_variable>(vars_2_2[1]), make_shared<cw_variable>(vars_2_2[2])), // 1
        cw_constraint(1, 0, make_shared<cw_variable>(vars_2_2[0]), make_shared<cw_variable>(vars_2_2[3])), // 2
        cw_constraint(1, 1, make_shared<cw_variable>(vars_2_2[1]), make_shared<cw_variable>(vars_2_2[3])), // 3

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_2_2[2]), make_shared<cw_variable>(vars_2_2[0])), // 4
        cw_constraint(1, 0, make_shared<cw_variable>(vars_2_2[2]), make_shared<cw_variable>(vars_2_2[1])), // 5
        cw_constraint(0, 1, make_shared<cw_variable>(vars_2_2[3]), make_shared<cw_variable>(vars_2_2[0])), // 6
        cw_constraint(1, 1, make_shared<cw_variable>(vars_2_2[3]), make_shared<cw_variable>(vars_2_2[1])), // 7
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_2_2 = {
        {vars_2_2[0], {contrs_2_2[4], contrs_2_2[6]}},
        {vars_2_2[1], {contrs_2_2[5], contrs_2_2[7]}},
        {vars_2_2[2], {contrs_2_2[0], contrs_2_2[1]}},
        {vars_2_2[3], {contrs_2_2[2], contrs_2_2[3]}},
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
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3[0]), make_shared<cw_variable>(vars_3_3[3])), // 0
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_3[1]), make_shared<cw_variable>(vars_3_3[3])), // 1
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3[2]), make_shared<cw_variable>(vars_3_3[3])), // 2
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_3[0]), make_shared<cw_variable>(vars_3_3[4])), // 3
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_3[1]), make_shared<cw_variable>(vars_3_3[4])), // 4
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_3[2]), make_shared<cw_variable>(vars_3_3[4])), // 5
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3[0]), make_shared<cw_variable>(vars_3_3[5])), // 6
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_3[1]), make_shared<cw_variable>(vars_3_3[5])), // 7
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3[2]), make_shared<cw_variable>(vars_3_3[5])), // 8

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3[3]), make_shared<cw_variable>(vars_3_3[0])), // 9
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_3[3]), make_shared<cw_variable>(vars_3_3[1])), // 10
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3[3]), make_shared<cw_variable>(vars_3_3[2])), // 11
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_3[4]), make_shared<cw_variable>(vars_3_3[0])), // 12
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_3[4]), make_shared<cw_variable>(vars_3_3[1])), // 13
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_3[4]), make_shared<cw_variable>(vars_3_3[2])), // 14
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3[5]), make_shared<cw_variable>(vars_3_3[0])), // 15
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_3[5]), make_shared<cw_variable>(vars_3_3[1])), // 16
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3[5]), make_shared<cw_variable>(vars_3_3[2])), // 17
        
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_3_3 = {
        {vars_3_3[0], {contrs_3_3[9], contrs_3_3[12], contrs_3_3[15]}},
        {vars_3_3[1], {contrs_3_3[10], contrs_3_3[13], contrs_3_3[16]}},
        {vars_3_3[2], {contrs_3_3[11], contrs_3_3[14], contrs_3_3[17]}},
        {vars_3_3[3], {contrs_3_3[0], contrs_3_3[1], contrs_3_3[2]}},
        {vars_3_3[4], {contrs_3_3[3], contrs_3_3[4], contrs_3_3[5]}},
        {vars_3_3[5], {contrs_3_3[6], contrs_3_3[7], contrs_3_3[8]}},
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
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[3])), // 0
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[3])), // 1
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[3])), // 2
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[4])), // 3
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[4])), // 4
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[4])), // 5
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[5])), // 6
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[5])), // 7
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[5])), // 8
        cw_constraint(3, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[6])), // 9
        cw_constraint(3, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[6])), // 10
        cw_constraint(3, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[6])), // 11
        
        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[0])), // 12
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[1])), // 13
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[2])), // 14
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[0])), // 15
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[1])), // 16
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[2])), // 17
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[0])), // 18
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[1])), // 19
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[2])), // 20
        cw_constraint(0, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[0])), // 21
        cw_constraint(1, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[1])), // 22
        cw_constraint(2, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[2])), // 23
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_3_4 = {
        {vars_3_4[0], {contrs_3_4[12], contrs_3_4[15], contrs_3_4[18], contrs_3_4[21]}},
        {vars_3_4[1], {contrs_3_4[13], contrs_3_4[16], contrs_3_4[19], contrs_3_4[22]}},
        {vars_3_4[2], {contrs_3_4[14], contrs_3_4[17], contrs_3_4[20], contrs_3_4[23]}},
        {vars_3_4[3], {contrs_3_4[0], contrs_3_4[1], contrs_3_4[2]}},
        {vars_3_4[4], {contrs_3_4[3], contrs_3_4[4], contrs_3_4[5]}},
        {vars_3_4[5], {contrs_3_4[6], contrs_3_4[7], contrs_3_4[8]}},
        {vars_3_4[6], {contrs_3_4[9], contrs_3_4[10], contrs_3_4[11]}},
    };
    unordered_set<cw_variable>   vars_3_4_set  (vars_3_4.begin(),   vars_3_4.end());
    unordered_set<cw_constraint> contrs_3_4_set(contrs_3_4.begin(), contrs_3_4.end());
    REQUIRE(dut->test_constructor_empty(3, 4, dict_barebones_path, &vars_3_4_set, &contrs_3_4_set, &arc_dep_3_4));
}

/**
 * constructor test for cw_csp with contents
*/
TEST_CASE("cw_csp constructor_with_contents", "[cw_csp],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp constructor_with_contents");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";

    // baseline 2x2 blank crossword
    vector<cw_variable> vars_2_2 = {
        cw_variable(0, 0, 2, VERTICAL,   {"an", "at", "on", "to"}),
        cw_variable(0, 1, 2, VERTICAL,   {"an", "at", "on", "to"}),
        cw_variable(0, 0, 2, HORIZONTAL, {"an", "at", "on", "to"}),
        cw_variable(1, 0, 2, HORIZONTAL, {"an", "at", "on", "to"}),
    };
    vector<cw_constraint> contrs_2_2 = {
        // forward (vertical --> horizontal)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_2_2[0]), make_shared<cw_variable>(vars_2_2[2])), // 0
        cw_constraint(0, 1, make_shared<cw_variable>(vars_2_2[1]), make_shared<cw_variable>(vars_2_2[2])), // 1
        cw_constraint(1, 0, make_shared<cw_variable>(vars_2_2[0]), make_shared<cw_variable>(vars_2_2[3])), // 2
        cw_constraint(1, 1, make_shared<cw_variable>(vars_2_2[1]), make_shared<cw_variable>(vars_2_2[3])), // 3

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_2_2[2]), make_shared<cw_variable>(vars_2_2[0])), // 4
        cw_constraint(1, 0, make_shared<cw_variable>(vars_2_2[2]), make_shared<cw_variable>(vars_2_2[1])), // 5
        cw_constraint(0, 1, make_shared<cw_variable>(vars_2_2[3]), make_shared<cw_variable>(vars_2_2[0])), // 6
        cw_constraint(1, 1, make_shared<cw_variable>(vars_2_2[3]), make_shared<cw_variable>(vars_2_2[1])), // 7
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_2_2 = {
        {vars_2_2[0], {contrs_2_2[4], contrs_2_2[6]}},
        {vars_2_2[1], {contrs_2_2[5], contrs_2_2[7]}},
        {vars_2_2[2], {contrs_2_2[0], contrs_2_2[1]}},
        {vars_2_2[3], {contrs_2_2[2], contrs_2_2[3]}},
    };
    unordered_set<cw_variable>   vars_2_2_set  (vars_2_2.begin(),   vars_2_2.end());
    unordered_set<cw_constraint> contrs_2_2_set(contrs_2_2.begin(), contrs_2_2.end());
    stringstream contents_2_2;
    contents_2_2 << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_constructor_contents(2, 2, contents_2_2.str(), dict_barebones_path, &vars_2_2_set, &contrs_2_2_set, &arc_dep_2_2));

    // baseline 3x4 blank crossword
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
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[3])), // 0
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[3])), // 1
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[3])), // 2
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[4])), // 3
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[4])), // 4
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[4])), // 5
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[5])), // 6
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[5])), // 7
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[5])), // 8
        cw_constraint(3, 0, make_shared<cw_variable>(vars_3_4[0]), make_shared<cw_variable>(vars_3_4[6])), // 9
        cw_constraint(3, 1, make_shared<cw_variable>(vars_3_4[1]), make_shared<cw_variable>(vars_3_4[6])), // 10
        cw_constraint(3, 2, make_shared<cw_variable>(vars_3_4[2]), make_shared<cw_variable>(vars_3_4[6])), // 11
        
        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[0])), // 12
        cw_constraint(1, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[1])), // 13
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_4[3]), make_shared<cw_variable>(vars_3_4[2])), // 14
        cw_constraint(0, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[0])), // 15
        cw_constraint(1, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[1])), // 16
        cw_constraint(2, 1, make_shared<cw_variable>(vars_3_4[4]), make_shared<cw_variable>(vars_3_4[2])), // 17
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[0])), // 18
        cw_constraint(1, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[1])), // 19
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_4[5]), make_shared<cw_variable>(vars_3_4[2])), // 20
        cw_constraint(0, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[0])), // 21
        cw_constraint(1, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[1])), // 22
        cw_constraint(2, 3, make_shared<cw_variable>(vars_3_4[6]), make_shared<cw_variable>(vars_3_4[2])), // 23
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_3_4 = {
        {vars_3_4[0], {contrs_3_4[12], contrs_3_4[15], contrs_3_4[18], contrs_3_4[21]}},
        {vars_3_4[1], {contrs_3_4[13], contrs_3_4[16], contrs_3_4[19], contrs_3_4[22]}},
        {vars_3_4[2], {contrs_3_4[14], contrs_3_4[17], contrs_3_4[20], contrs_3_4[23]}},
        {vars_3_4[3], {contrs_3_4[0], contrs_3_4[1], contrs_3_4[2]}},
        {vars_3_4[4], {contrs_3_4[3], contrs_3_4[4], contrs_3_4[5]}},
        {vars_3_4[5], {contrs_3_4[6], contrs_3_4[7], contrs_3_4[8]}},
        {vars_3_4[6], {contrs_3_4[9], contrs_3_4[10], contrs_3_4[11]}},
    };
    unordered_set<cw_variable>   vars_3_4_set  (vars_3_4.begin(),   vars_3_4.end());
    unordered_set<cw_constraint> contrs_3_4_set(contrs_3_4.begin(), contrs_3_4.end());
    stringstream contents_3_4;
    contents_3_4 << WCD << WCD << WCD
                 << WCD << WCD << WCD
                 << WCD << WCD << WCD
                 << WCD << WCD << WCD;
    REQUIRE(dut->test_constructor_contents(3, 4, contents_3_4.str(), dict_barebones_path, &vars_3_4_set, &contrs_3_4_set, &arc_dep_3_4));

    // 3x4 crossword of larger size with black tiles blocking it
    stringstream contents_3_4_black_border;
    contents_3_4_black_border << WCD << WCD << WCD << BLK << BLK 
                              << WCD << WCD << WCD << BLK << BLK 
                              << WCD << WCD << WCD << BLK << BLK 
                              << WCD << WCD << WCD << BLK << BLK 
                              << BLK << BLK << BLK << BLK << BLK
                              << BLK << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_constructor_contents(5, 6, contents_3_4_black_border.str(), dict_barebones_path, &vars_3_4_set, &contrs_3_4_set, &arc_dep_3_4));

    // 3x4 crossword embedded in a donut of black tiles and random single wildcards
    stringstream contents_3_4_black_border_with_dots;
    contents_3_4_black_border_with_dots << WCD << WCD << WCD << BLK << WCD << BLK << WCD
                                        << WCD << WCD << WCD << BLK << BLK << WCD << BLK
                                        << WCD << WCD << WCD << BLK << WCD << BLK << WCD
                                        << WCD << WCD << WCD << BLK << BLK << WCD << BLK
                                        << BLK << BLK << BLK << WCD << BLK << BLK << WCD
                                        << BLK << WCD << BLK << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_constructor_contents(7, 6, contents_3_4_black_border_with_dots.str(), dict_barebones_path, &vars_3_4_set, &contrs_3_4_set, &arc_dep_3_4));

    // 3x3 donut crossword
    vector<cw_variable> vars_3_3_donut = {
        cw_variable(0, 0, 3, VERTICAL,   {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(0, 2, 3, VERTICAL,   {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(0, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
        cw_variable(2, 0, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
    };
    vector<cw_constraint> contrs_3_3_donut = {
        // forward (vertical --> horizontal)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3_donut[0]), make_shared<cw_variable>(vars_3_3_donut[2])), // 0
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3_donut[0]), make_shared<cw_variable>(vars_3_3_donut[3])), // 1
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3_donut[1]), make_shared<cw_variable>(vars_3_3_donut[2])), // 2
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3_donut[1]), make_shared<cw_variable>(vars_3_3_donut[3])), // 3

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3_donut[2]), make_shared<cw_variable>(vars_3_3_donut[0])), // 4
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3_donut[3]), make_shared<cw_variable>(vars_3_3_donut[0])), // 5
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3_donut[2]), make_shared<cw_variable>(vars_3_3_donut[1])), // 6
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3_donut[3]), make_shared<cw_variable>(vars_3_3_donut[1])), // 7
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_3_3_donut = {
        {vars_3_3_donut[0], {contrs_3_3_donut[4], contrs_3_3_donut[5]}},
        {vars_3_3_donut[1], {contrs_3_3_donut[6], contrs_3_3_donut[7]}},
        {vars_3_3_donut[2], {contrs_3_3_donut[0], contrs_3_3_donut[2]}},
        {vars_3_3_donut[3], {contrs_3_3_donut[1], contrs_3_3_donut[3]}},
    };
    unordered_set<cw_variable>   vars_3_3_donut_set  (vars_3_3_donut.begin(),   vars_3_3_donut.end());
    unordered_set<cw_constraint> contrs_3_3_donut_set(contrs_3_3_donut.begin(), contrs_3_3_donut.end());
    stringstream contents_3_3_donut;
    contents_3_3_donut << WCD << WCD << WCD 
                       << WCD << BLK << WCD 
                       << WCD << WCD << WCD;
    REQUIRE(dut->test_constructor_contents(3, 3, contents_3_3_donut.str(), dict_barebones_path, &vars_3_3_donut_set, &contrs_3_3_donut_set, &arc_dep_3_3_donut));

    // single cross crossword with a single letter filled in
    vector<cw_variable> vars_5_6_cross = {
        cw_variable(1, 2, 4, VERTICAL,   {"boob", "been"}),
        cw_variable(2, 1, 3, HORIZONTAL, {"ace", "and", "cat", "cab", "can", "cup", "cub", "dan", "dab"}),
    };
    vector<cw_constraint> contrs_5_6_cross = {
        // forward (vertical --> horizontal)
        cw_constraint(1, 1, make_shared<cw_variable>(vars_5_6_cross[0]), make_shared<cw_variable>(vars_5_6_cross[1])),

        // backward (horizontal --> vertical)
        cw_constraint(1, 1, make_shared<cw_variable>(vars_5_6_cross[1]), make_shared<cw_variable>(vars_5_6_cross[0])),
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_5_6_cross = {
        {vars_5_6_cross[0], {contrs_5_6_cross[1]}},
        {vars_5_6_cross[1], {contrs_5_6_cross[0]}}
    };
    unordered_set<cw_variable>   vars_5_6_cross_set  (vars_5_6_cross.begin(),   vars_5_6_cross.end());
    unordered_set<cw_constraint> contrs_5_6_cross_set(contrs_5_6_cross.begin(), contrs_5_6_cross.end());
    stringstream contents_5_6_cross;
    contents_5_6_cross << BLK << BLK << BLK << BLK << BLK 
                       << BLK << BLK << 'b' << BLK << BLK
                       << BLK << WCD << WCD << WCD << BLK
                       << BLK << BLK << WCD << BLK << BLK
                       << BLK << BLK << WCD << BLK << BLK
                       << BLK << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_constructor_contents(5, 6, contents_5_6_cross.str(), dict_barebones_path, &vars_5_6_cross_set, &contrs_5_6_cross_set, &arc_dep_5_6_cross));

    // simple crossword with single variables & no constraints
    vector<cw_variable> vars_5_3_no_constraints = {
        cw_variable(0, 0, 3, VERTICAL,   {"eta"}), // 0
        cw_variable(0, 2, 3, HORIZONTAL, {"ace", "and", "are", "bed", "cab", "can", "cat", "cub", "cup", "dab", "dan", "eta", "gap", "pot"}), // 14
        cw_variable(2, 2, 3, HORIZONTAL, {"cab", "can", "cat", "dab", "dan", "gap"}), // 14
    };
    vector<cw_constraint> contrs_5_3_no_constraints = {}; // no intersections between words
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_5_3_no_constraints = {}; // no constraints
    unordered_set<cw_variable>   vars_5_3_no_constraints_set  (vars_5_3_no_constraints.begin(),   vars_5_3_no_constraints.end());
    unordered_set<cw_constraint> contrs_5_3_no_constraints_set(contrs_5_3_no_constraints.begin(), contrs_5_3_no_constraints.end());
    stringstream contents_5_3_no_constraints;
    contents_5_3_no_constraints << 'e' << BLK << WCD << WCD << WCD
                                << 't' << BLK << BLK << BLK << BLK
                                << 'a' << BLK << WCD << 'a' << WCD;
    REQUIRE(dut->test_constructor_contents(5, 3, contents_5_3_no_constraints.str(), dict_simple_path, &vars_5_3_no_constraints_set, &contrs_5_3_no_constraints_set, &arc_dep_5_3_no_constraints));

    // 6x7 crossword with complex intersections & mix of wildcards and letters
    vector<cw_variable> vars_6_7_complex = {
        cw_variable(0, 0, 4, VERTICAL,   {"atol", "atom"}), // 0
        cw_variable(5, 0, 2, VERTICAL,   {"ad", "an", "at", "be", "on", "to"}), // 1
        cw_variable(3, 1, 4, VERTICAL,   {"plan"}), // 2
        cw_variable(4, 2, 3, VERTICAL,   {}), // 3
        cw_variable(0, 3, 3, VERTICAL,   {"bed"}), // 4
        cw_variable(2, 4, 4, VERTICAL,   {"boob", "knob"}), // 5
        cw_variable(0, 5, 4, VERTICAL,   {"pole", "pore"}), // 6
        cw_variable(5, 5, 2, VERTICAL,   {"an"}), // 7
        cw_variable(0, 3, 3, HORIZONTAL, {"gap", "cup"}), // 8
        cw_variable(1, 0, 4, HORIZONTAL, {"tore", "tote"}), // 9
        cw_variable(2, 3, 3, HORIZONTAL, {"ace", "and", "are", "bed", "cab", "can", "cat", "cub", "cup", "dab", "dan", "eta", "gap", "pot"}), // 10
        cw_variable(3, 0, 2, HORIZONTAL, {"ad", "an", "at", "be", "on", "to"}), // 11
        cw_variable(3, 4, 2, HORIZONTAL, {"be"}), // 12
        cw_variable(4, 1, 4, HORIZONTAL, {"halo"}), // 13
        cw_variable(5, 0, 3, HORIZONTAL, {"cat"}), // 14
        cw_variable(5, 4, 2, HORIZONTAL, {}), // 15
        cw_variable(6, 0, 4, HORIZONTAL, {"knob", "know"}), // 16
    };
    vector<cw_constraint> contrs_6_7_complex = {
        // forward (vertical --> horizontal)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[4]), make_shared<cw_variable>(vars_6_7_complex[8])), // 0
        cw_constraint(0, 2, make_shared<cw_variable>(vars_6_7_complex[6]), make_shared<cw_variable>(vars_6_7_complex[8])), // 1
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[0]), make_shared<cw_variable>(vars_6_7_complex[9])), // 2
        cw_constraint(1, 3, make_shared<cw_variable>(vars_6_7_complex[4]), make_shared<cw_variable>(vars_6_7_complex[9])), // 3
        cw_constraint(2, 0, make_shared<cw_variable>(vars_6_7_complex[4]), make_shared<cw_variable>(vars_6_7_complex[10])), // 4
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[10])), // 5
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[6]), make_shared<cw_variable>(vars_6_7_complex[10])), // 6
        cw_constraint(3, 0, make_shared<cw_variable>(vars_6_7_complex[0]), make_shared<cw_variable>(vars_6_7_complex[11])), // 7
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[11])), // 8
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[12])), // 9
        cw_constraint(3, 1, make_shared<cw_variable>(vars_6_7_complex[6]), make_shared<cw_variable>(vars_6_7_complex[12])), // 10
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[13])), // 11
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[3]), make_shared<cw_variable>(vars_6_7_complex[13])), // 12
        cw_constraint(2, 3, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[13])), // 13
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[1]), make_shared<cw_variable>(vars_6_7_complex[14])), // 14
        cw_constraint(2, 1, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[14])), // 15
        cw_constraint(1, 2, make_shared<cw_variable>(vars_6_7_complex[3]), make_shared<cw_variable>(vars_6_7_complex[14])), // 16
        cw_constraint(3, 0, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[15])), // 17
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[7]), make_shared<cw_variable>(vars_6_7_complex[15])), // 18
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[1]), make_shared<cw_variable>(vars_6_7_complex[16])), // 19
        cw_constraint(3, 1, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[16])), // 20
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[3]), make_shared<cw_variable>(vars_6_7_complex[16])), // 21

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[8]), make_shared<cw_variable>(vars_6_7_complex[4])), // 22
        cw_constraint(2, 0, make_shared<cw_variable>(vars_6_7_complex[8]), make_shared<cw_variable>(vars_6_7_complex[6])), // 23
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[9]), make_shared<cw_variable>(vars_6_7_complex[0])), // 24
        cw_constraint(3, 1, make_shared<cw_variable>(vars_6_7_complex[9]), make_shared<cw_variable>(vars_6_7_complex[4])), // 25
        cw_constraint(0, 2, make_shared<cw_variable>(vars_6_7_complex[10]), make_shared<cw_variable>(vars_6_7_complex[4])), // 26
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[10]), make_shared<cw_variable>(vars_6_7_complex[5])), // 27
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[10]), make_shared<cw_variable>(vars_6_7_complex[6])), // 28
        cw_constraint(0, 3, make_shared<cw_variable>(vars_6_7_complex[11]), make_shared<cw_variable>(vars_6_7_complex[0])), // 29
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[11]), make_shared<cw_variable>(vars_6_7_complex[2])), // 30
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[12]), make_shared<cw_variable>(vars_6_7_complex[5])), // 31
        cw_constraint(1, 3, make_shared<cw_variable>(vars_6_7_complex[12]), make_shared<cw_variable>(vars_6_7_complex[6])), // 32
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[13]), make_shared<cw_variable>(vars_6_7_complex[2])), // 33
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[13]), make_shared<cw_variable>(vars_6_7_complex[3])), // 34
        cw_constraint(3, 2, make_shared<cw_variable>(vars_6_7_complex[13]), make_shared<cw_variable>(vars_6_7_complex[5])), // 35
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[14]), make_shared<cw_variable>(vars_6_7_complex[1])), // 36
        cw_constraint(1, 2, make_shared<cw_variable>(vars_6_7_complex[14]), make_shared<cw_variable>(vars_6_7_complex[2])), // 37
        cw_constraint(2, 1, make_shared<cw_variable>(vars_6_7_complex[14]), make_shared<cw_variable>(vars_6_7_complex[3])), // 38
        cw_constraint(0, 3, make_shared<cw_variable>(vars_6_7_complex[15]), make_shared<cw_variable>(vars_6_7_complex[5])), // 39
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[15]), make_shared<cw_variable>(vars_6_7_complex[7])), // 40
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[16]), make_shared<cw_variable>(vars_6_7_complex[1])), // 41
        cw_constraint(1, 3, make_shared<cw_variable>(vars_6_7_complex[16]), make_shared<cw_variable>(vars_6_7_complex[2])), // 42
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[16]), make_shared<cw_variable>(vars_6_7_complex[3])), // 43
    };
    unordered_map<cw_variable, unordered_set<cw_constraint> > arc_dep_6_7_complex = {
        {vars_6_7_complex[0], {contrs_6_7_complex[24], contrs_6_7_complex[29]}},
        {vars_6_7_complex[1], {contrs_6_7_complex[36], contrs_6_7_complex[41]}},
        {vars_6_7_complex[2], {contrs_6_7_complex[30], contrs_6_7_complex[33], contrs_6_7_complex[37], contrs_6_7_complex[42]}},
        {vars_6_7_complex[3], {contrs_6_7_complex[34], contrs_6_7_complex[38], contrs_6_7_complex[43]}},
        {vars_6_7_complex[4], {contrs_6_7_complex[22], contrs_6_7_complex[25], contrs_6_7_complex[26]}},
        {vars_6_7_complex[5], {contrs_6_7_complex[27], contrs_6_7_complex[31], contrs_6_7_complex[35], contrs_6_7_complex[39]}},
        {vars_6_7_complex[6], {contrs_6_7_complex[23], contrs_6_7_complex[28], contrs_6_7_complex[32]}},
        {vars_6_7_complex[7], {contrs_6_7_complex[40]}},
        {vars_6_7_complex[8], {contrs_6_7_complex[0], contrs_6_7_complex[1]}},
        {vars_6_7_complex[9], {contrs_6_7_complex[2], contrs_6_7_complex[3]}},
        {vars_6_7_complex[10], {contrs_6_7_complex[4], contrs_6_7_complex[5], contrs_6_7_complex[6]}},
        {vars_6_7_complex[11], {contrs_6_7_complex[7], contrs_6_7_complex[8]}},
        {vars_6_7_complex[12], {contrs_6_7_complex[9], contrs_6_7_complex[10]}},
        {vars_6_7_complex[13], {contrs_6_7_complex[11], contrs_6_7_complex[12], contrs_6_7_complex[13]}},
        {vars_6_7_complex[14], {contrs_6_7_complex[14], contrs_6_7_complex[15], contrs_6_7_complex[16]}},
        {vars_6_7_complex[15], {contrs_6_7_complex[17], contrs_6_7_complex[18]}},
        {vars_6_7_complex[16], {contrs_6_7_complex[19], contrs_6_7_complex[20], contrs_6_7_complex[21]}},
    };
    unordered_set<cw_variable>   vars_6_7_complex_set  (vars_6_7_complex.begin(),   vars_6_7_complex.end());
    unordered_set<cw_constraint> contrs_6_7_complex_set(contrs_6_7_complex.begin(), contrs_6_7_complex.end());
    stringstream contents_6_7_complex;
    contents_6_7_complex << WCD << BLK << BLK << WCD << WCD << 'p' 
                         << 't' << WCD << WCD << 'e' << BLK << WCD 
                         << WCD << BLK << BLK << WCD << WCD << WCD 
                         << WCD << WCD << BLK << BLK << WCD << 'e' 
                         << BLK << WCD << 'a' << WCD << 'o' << BLK 
                         << WCD << 'a' << 't' << BLK << 'b' << 'a' 
                         << WCD << 'n' << WCD << WCD << BLK << 'n';
    REQUIRE(dut->test_constructor_contents(6, 7, contents_6_7_complex.str(), dict_simple_path, &vars_6_7_complex_set, &contrs_6_7_complex_set, &arc_dep_6_7_complex));
}

/**
 * simple AC-3 test for cw_csp for valid/invalid checking
*/
TEST_CASE("cw_csp ac3_valid_check", "[cw_csp],[ac3],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp ac3_valid_check");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";

    // ############### invalid crosswords ###############

    // initially invalid 6x7 crossword with complex intersections & mix of wildcards and letters
    stringstream contents_6_7_complex_invalid;
    contents_6_7_complex_invalid << WCD << BLK << BLK << WCD << WCD << 'p' 
                                 << 't' << WCD << WCD << 'e' << BLK << WCD 
                                 << WCD << BLK << BLK << WCD << WCD << WCD 
                                 << WCD << WCD << BLK << BLK << WCD << 'e' 
                                 << BLK << WCD << 'a' << WCD << 'o' << BLK 
                                 << WCD << 'a' << 't' << BLK << 'b' << 'a' 
                                 << WCD << 'n' << WCD << WCD << BLK << 'n';
    REQUIRE(dut->test_ac3_validity(6, 7, contents_6_7_complex_invalid.str(), dict_simple_path, false));

    // simple invalid 2x2 crossword
    stringstream contents_2_2_invalid;
    contents_2_2_invalid << WCD << BLK 
                         << WCD << 't';
    REQUIRE(dut->test_ac3_validity(2, 2, contents_2_2_invalid.str(), dict_barebones_path, false));

    // ############### valid crosswords ###############

    // simple valid 2x2 crossword
    stringstream contents_2_2_valid;
    contents_2_2_valid << WCD << BLK 
                       << WCD << WCD;
    REQUIRE(dut->test_ac3_validity(2, 2, contents_2_2_valid.str(), dict_barebones_path, true));

    // simple valid blank 3x3 crossword
    stringstream contents_3_3_blank_valid;
    contents_3_3_blank_valid << WCD << BLK << WCD 
                             << WCD << WCD << WCD 
                             << WCD << BLK << WCD;
    REQUIRE(dut->test_ac3_validity(3, 3, contents_3_3_blank_valid.str(), dict_simple_path, true));

    // simple valid blank 4x4 crossword
    stringstream contents_4_4_blank_valid;
    contents_4_4_blank_valid << WCD << WCD << WCD << WCD 
                             << WCD << BLK << BLK << WCD 
                             << WCD << BLK << BLK << WCD 
                             << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_ac3_validity(4, 4, contents_4_4_blank_valid.str(), dict_simple_path, true));

    // valid 6x7 crossword with complex intersections & mix of wildcards and letters
    stringstream contents_6_7_complex_valid;
    contents_6_7_complex_valid << WCD << BLK << BLK << WCD << BLK << 'p' 
                               << 't' << WCD << WCD << 'e' << BLK << BLK 
                               << WCD << BLK << BLK << WCD << WCD << WCD 
                               << WCD << BLK << WCD << BLK << WCD << BLK 
                               << BLK << BLK << 'a' << BLK << 'o' << BLK 
                               << WCD << 'a' << 't' << BLK << WCD << BLK 
                               << BLK << 'n' << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_ac3_validity(6, 7, contents_6_7_complex_valid.str(), dict_simple_path, true));
}

/**
 * simple AC-3 test for cw_csp for duplicate word prevention
 * TODO: should this pass? perhaps uncomment when word inequality constraint added
*/
// TEST_CASE("cw_csp ac3_valid_check_duplicates", "[cw_csp],[ac3],[duplicates],[quick]") {
//     cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp ac3_valid_check_duplicates");
//     const string dict_single_word = "cw_csp/data/dict_single_word.txt";

//     // simple crossword that requires duplicates
//     stringstream contents_2_2_duplicate_invalid;
//     contents_2_2_duplicate_invalid << WCD << WCD
//                                    << WCD << WCD;
//     REQUIRE(dut->test_ac3_validity(2, 2, contents_2_2_duplicate_invalid.str(), dict_single_word, false));
// }

/**
 * AC-3 test for cw_csp for proper domain pruning
*/
TEST_CASE("cw_csp ac3_pruning", "[cw_csp],[ac3],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp ac3_pruning");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";
    const string dict_nytimes_8_28_23 = "cw_csp/data/dict_nytimes_8_28_23.txt";
    const string dict_nytimes_10_17_13 = "cw_csp/data/dict_nytimes_10_17_13.txt";
    const string dict_nytimes_2_3_17 = "cw_csp/data/dict_nytimes_2_3_17.txt";

    // simple valid 2x2 crossword
    unordered_set<cw_variable> vars_2_2 = {
        cw_variable(0, 0, 2, VERTICAL,   {"at", "to"}),
        cw_variable(1, 0, 2, HORIZONTAL, {"to", "on"}),
    };
    stringstream contents_2_2;
    contents_2_2 << WCD << BLK 
                 << WCD << WCD;
    REQUIRE(dut->test_ac3(2, 2, contents_2_2.str(), dict_barebones_path, true, &vars_2_2));

    // simple valid H-shaped 3x3 crossword
    unordered_set<cw_variable> vars_3_3_h = {
        cw_variable(0, 0, 3, VERTICAL,   {"ace", "bed", "cab", "can", "cat", "dab", "dan", "gap"}),
        cw_variable(1, 0, 3, HORIZONTAL, {"cat", "can", "eta", "ace", "are"}),
        cw_variable(0, 2, 3, VERTICAL,   {"eta", "and", "cab", "can", "cat", "dab", "dan", "gap", "bed"}),
    };
    stringstream contents_3_3_h;
    contents_3_3_h << WCD << BLK << WCD 
                   << WCD << WCD << WCD 
                   << WCD << BLK << WCD;
    REQUIRE(dut->test_ac3(3, 3, contents_3_3_h.str(), dict_simple_path, true, &vars_3_3_h));

    // simple valid H-shaped 4x4 crossword
    unordered_set<cw_variable> vars_4_4_h = {
        cw_variable(0, 0, 4, VERTICAL,   {"atol", "atom", "caps", "cars", "halo", "knob", "know", "pant"}),
        cw_variable(1, 0, 4, HORIZONTAL, {"tear", "tore", "tote", "trot", "atol", "near", "node"}),
        cw_variable(0, 3, 4, VERTICAL,   {"trot", "troy", "been", "near", "pear", "tear", "atol", "atom", "plan"}),
    };
    stringstream contents_4_4_h;
    contents_4_4_h << WCD << BLK << BLK << WCD 
                   << WCD << WCD << WCD << WCD 
                   << WCD << BLK << BLK << WCD 
                   << WCD << BLK << BLK << WCD;
    REQUIRE(dut->test_ac3(4, 4, contents_4_4_h.str(), dict_simple_path, true, &vars_4_4_h));

    // 5x5 nytimes crossword 8/28/2023
    unordered_set<cw_variable> vars_nytimes_8_28_23 = {
        cw_variable(2, 0, 3, VERTICAL,   {"bmx"}),
        cw_variable(0, 1, 5, VERTICAL,   {"broom"}),
        cw_variable(0, 2, 5, VERTICAL,   {"leave"}),
        cw_variable(0, 3, 5, VERTICAL,   {"eaten"}),
        cw_variable(0, 4, 3, VERTICAL,   {"wds"}),
        cw_variable(0, 1, 4, HORIZONTAL, {"blew"}),
        cw_variable(1, 1, 4, HORIZONTAL, {"read"}),
        cw_variable(2, 0, 5, HORIZONTAL, {"boats"}),
        cw_variable(3, 0, 4, HORIZONTAL, {"move"}),
        cw_variable(4, 0, 4, HORIZONTAL, {"xmen"}),
    };
    stringstream contents_nytimes_8_28_23;
    contents_nytimes_8_28_23 << BLK << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_ac3(5, 5, contents_nytimes_8_28_23.str(), dict_nytimes_8_28_23, true, &vars_nytimes_8_28_23));

    // 5x5 nytimes crossword 10/17/13
    unordered_set<cw_variable> vars_nytimes_10_17_13 = {
        cw_variable(0, 0, 4, VERTICAL,   {"doze"}),
        cw_variable(2, 1, 3, VERTICAL,   {"eno"}),
        cw_variable(0, 2, 5, VERTICAL,   {"cyber"}),
        cw_variable(0, 3, 3, VERTICAL,   {"ear"}),
        cw_variable(1, 4, 4, VERTICAL,   {"mayo"}),
        cw_variable(0, 0, 4, HORIZONTAL, {"dice"}),
        cw_variable(1, 2, 3, HORIZONTAL, {"yam"}),
        cw_variable(2, 0, 5, HORIZONTAL, {"zebra"}),
        cw_variable(3, 0, 3, HORIZONTAL, {"ene"}),
        cw_variable(4, 1, 4, HORIZONTAL, {"oreo"}),
    };
    stringstream contents_nytimes_10_17_13;
    contents_nytimes_10_17_13 << WCD << 'i' << WCD << WCD << BLK 
                              << WCD << BLK << WCD << WCD << WCD
                              << WCD << WCD << WCD << WCD << WCD 
                              << WCD << WCD << WCD << BLK << WCD 
                              << BLK << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_ac3(5, 5, contents_nytimes_10_17_13.str(), dict_nytimes_10_17_13, true, &vars_nytimes_10_17_13));

    // 5x5 nytimes crossword 2/3/17
    unordered_set<cw_variable> vars_nytimes_2_3_17 = {
        cw_variable(2, 0, 3, VERTICAL,   {"may"}),
        cw_variable(1, 1, 4, VERTICAL,   {"sale"}),
        cw_variable(0, 2, 5, VERTICAL,   {"april"}),
        cw_variable(0, 3, 5, VERTICAL,   {"decal"}),
        cw_variable(0, 4, 4, VERTICAL,   {"ochs"}),
        cw_variable(0, 2, 3, HORIZONTAL, {"ado"}),
        cw_variable(1, 1, 4, HORIZONTAL, {"spec"}),
        cw_variable(2, 0, 5, HORIZONTAL, {"march"}),
        cw_variable(3, 0, 5, HORIZONTAL, {"alias"}),
        cw_variable(4, 0, 4, HORIZONTAL, {"yell"}),
    };
    stringstream contents_nytimes_2_3_17;
    contents_nytimes_2_3_17 << BLK << BLK << 'a' << WCD << WCD 
                            << BLK << WCD << WCD << WCD << WCD
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_ac3(5, 5, contents_nytimes_2_3_17.str(), dict_nytimes_2_3_17, true, &vars_nytimes_2_3_17));
}

/**
 * simple backtracking solving test for cw_csp for valid/invalid checking
*/
TEST_CASE("cw_csp backtracking_valid_check", "[cw_csp],[backtracking],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp backtracking_valid_check");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_1000 = "cw_csp/data/words_top1000.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";
    const string dict_nytimes_8_28_23 = "cw_csp/data/dict_nytimes_8_28_23.txt";
    const string dict_nytimes_10_17_13 = "cw_csp/data/dict_nytimes_10_17_13.txt";
    const string dict_nytimes_2_3_17 = "cw_csp/data/dict_nytimes_2_3_17.txt";

    // ############### valid crosswords ###############

    // simple valid 2x2 crossword
    stringstream contents_2_2;
    contents_2_2 << WCD << BLK 
                 << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(2, 2, contents_2_2.str(), dict_barebones_path, true, true));

    // 5x5 nytimes crossword 8/28/2023
    stringstream contents_nytimes_8_28_23;
    contents_nytimes_8_28_23 << BLK << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_nytimes_8_28_23.str(), dict_nytimes_8_28_23, true, true));

    // 5x5 nytimes crossword 10/17/13
    stringstream contents_nytimes_10_17_13;
    contents_nytimes_10_17_13 << WCD << WCD << WCD << WCD << BLK 
                              << WCD << BLK << WCD << WCD << WCD
                              << WCD << WCD << WCD << WCD << WCD 
                              << WCD << WCD << WCD << BLK << WCD 
                              << BLK << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_nytimes_10_17_13.str(), dict_nytimes_10_17_13, true, true));

    // 5x5 nytimes crossword 2/3/17
    stringstream contents_nytimes_2_3_17;
    contents_nytimes_2_3_17 << BLK << BLK << WCD << WCD << WCD 
                            << BLK << WCD << WCD << WCD << WCD
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_nytimes_2_3_17.str(), dict_nytimes_2_3_17, true, true));

    // empty 3x3 crossword
    stringstream contents_3_3_empty;
    contents_3_3_empty << WCD << WCD << WCD 
                       << WCD << WCD << WCD 
                       << WCD << WCD << WCD ;
    REQUIRE(dut->test_backtracking_validity(3, 3, contents_3_3_empty.str(), dict_1000, true, true));

    // empty 4x4 crossword
    stringstream contents_4_3_empty;
    contents_4_3_empty << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD; 
    REQUIRE(dut->test_backtracking_validity(4, 3, contents_4_3_empty.str(), dict_1000, true, true));

    // ############### invalid crosswords ###############

    // empty 4x4 crossword
    stringstream contents_4_4_empty;
    contents_4_4_empty << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(4, 4, contents_4_4_empty.str(), dict_1000, false, true));
}