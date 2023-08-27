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
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3_donut[0]), make_shared<cw_variable>(vars_3_3_donut[2])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3_donut[0]), make_shared<cw_variable>(vars_3_3_donut[3])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3_donut[1]), make_shared<cw_variable>(vars_3_3_donut[2])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3_donut[1]), make_shared<cw_variable>(vars_3_3_donut[3])),

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_3_3_donut[2]), make_shared<cw_variable>(vars_3_3_donut[0])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_3_3_donut[3]), make_shared<cw_variable>(vars_3_3_donut[0])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_3_3_donut[2]), make_shared<cw_variable>(vars_3_3_donut[1])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_3_3_donut[3]), make_shared<cw_variable>(vars_3_3_donut[1])),
    };
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_3_3_donut = {
        {vars_3_3_donut[0], {vars_3_3_donut[2], vars_3_3_donut[3]}},
        {vars_3_3_donut[1], {vars_3_3_donut[2], vars_3_3_donut[3]}},
        {vars_3_3_donut[2], {vars_3_3_donut[0], vars_3_3_donut[1]}},
        {vars_3_3_donut[3], {vars_3_3_donut[0], vars_3_3_donut[1]}},
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
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_5_6_cross = {
        {vars_5_6_cross[0], {vars_5_6_cross[1]}},
        {vars_5_6_cross[1], {vars_5_6_cross[0]}}
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
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[4]), make_shared<cw_variable>(vars_6_7_complex[8])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_6_7_complex[6]), make_shared<cw_variable>(vars_6_7_complex[8])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[0]), make_shared<cw_variable>(vars_6_7_complex[9])),
        cw_constraint(1, 3, make_shared<cw_variable>(vars_6_7_complex[4]), make_shared<cw_variable>(vars_6_7_complex[9])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_6_7_complex[4]), make_shared<cw_variable>(vars_6_7_complex[10])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[10])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[6]), make_shared<cw_variable>(vars_6_7_complex[10])),
        cw_constraint(3, 0, make_shared<cw_variable>(vars_6_7_complex[0]), make_shared<cw_variable>(vars_6_7_complex[11])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[11])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[12])),
        cw_constraint(3, 1, make_shared<cw_variable>(vars_6_7_complex[6]), make_shared<cw_variable>(vars_6_7_complex[12])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[13])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[3]), make_shared<cw_variable>(vars_6_7_complex[13])),
        cw_constraint(2, 3, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[13])),
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[1]), make_shared<cw_variable>(vars_6_7_complex[14])),
        cw_constraint(2, 1, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[14])),
        cw_constraint(1, 2, make_shared<cw_variable>(vars_6_7_complex[3]), make_shared<cw_variable>(vars_6_7_complex[14])),
        cw_constraint(3, 0, make_shared<cw_variable>(vars_6_7_complex[5]), make_shared<cw_variable>(vars_6_7_complex[15])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[7]), make_shared<cw_variable>(vars_6_7_complex[15])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[1]), make_shared<cw_variable>(vars_6_7_complex[16])),
        cw_constraint(3, 1, make_shared<cw_variable>(vars_6_7_complex[2]), make_shared<cw_variable>(vars_6_7_complex[16])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[3]), make_shared<cw_variable>(vars_6_7_complex[16])),

        // backward (horizontal --> vertical)
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[8]), make_shared<cw_variable>(vars_6_7_complex[4])),
        cw_constraint(2, 0, make_shared<cw_variable>(vars_6_7_complex[8]), make_shared<cw_variable>(vars_6_7_complex[6])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[9]), make_shared<cw_variable>(vars_6_7_complex[0])),
        cw_constraint(3, 1, make_shared<cw_variable>(vars_6_7_complex[9]), make_shared<cw_variable>(vars_6_7_complex[4])),
        cw_constraint(0, 2, make_shared<cw_variable>(vars_6_7_complex[10]), make_shared<cw_variable>(vars_6_7_complex[4])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[10]), make_shared<cw_variable>(vars_6_7_complex[5])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[10]), make_shared<cw_variable>(vars_6_7_complex[6])),
        cw_constraint(0, 3, make_shared<cw_variable>(vars_6_7_complex[11]), make_shared<cw_variable>(vars_6_7_complex[0])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[11]), make_shared<cw_variable>(vars_6_7_complex[2])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[12]), make_shared<cw_variable>(vars_6_7_complex[5])),
        cw_constraint(1, 3, make_shared<cw_variable>(vars_6_7_complex[12]), make_shared<cw_variable>(vars_6_7_complex[6])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[13]), make_shared<cw_variable>(vars_6_7_complex[2])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[13]), make_shared<cw_variable>(vars_6_7_complex[3])),
        cw_constraint(3, 2, make_shared<cw_variable>(vars_6_7_complex[13]), make_shared<cw_variable>(vars_6_7_complex[5])),
        cw_constraint(0, 0, make_shared<cw_variable>(vars_6_7_complex[14]), make_shared<cw_variable>(vars_6_7_complex[1])),
        cw_constraint(1, 2, make_shared<cw_variable>(vars_6_7_complex[14]), make_shared<cw_variable>(vars_6_7_complex[2])),
        cw_constraint(2, 1, make_shared<cw_variable>(vars_6_7_complex[14]), make_shared<cw_variable>(vars_6_7_complex[3])),
        cw_constraint(0, 3, make_shared<cw_variable>(vars_6_7_complex[15]), make_shared<cw_variable>(vars_6_7_complex[5])),
        cw_constraint(1, 0, make_shared<cw_variable>(vars_6_7_complex[15]), make_shared<cw_variable>(vars_6_7_complex[7])),
        cw_constraint(0, 1, make_shared<cw_variable>(vars_6_7_complex[16]), make_shared<cw_variable>(vars_6_7_complex[1])),
        cw_constraint(1, 3, make_shared<cw_variable>(vars_6_7_complex[16]), make_shared<cw_variable>(vars_6_7_complex[2])),
        cw_constraint(2, 2, make_shared<cw_variable>(vars_6_7_complex[16]), make_shared<cw_variable>(vars_6_7_complex[3])),
    };
    unordered_map<cw_variable, unordered_set<cw_variable> > arc_dep_6_7_complex = {
        {vars_6_7_complex[0], {vars_6_7_complex[9], vars_6_7_complex[11]}},
        {vars_6_7_complex[1], {vars_6_7_complex[14], vars_6_7_complex[16]}},
        {vars_6_7_complex[2], {vars_6_7_complex[11], vars_6_7_complex[13], vars_6_7_complex[14], vars_6_7_complex[16]}},
        {vars_6_7_complex[3], {vars_6_7_complex[13], vars_6_7_complex[14], vars_6_7_complex[16]}},
        {vars_6_7_complex[4], {vars_6_7_complex[8], vars_6_7_complex[9], vars_6_7_complex[10]}},
        {vars_6_7_complex[5], {vars_6_7_complex[10], vars_6_7_complex[12], vars_6_7_complex[13], vars_6_7_complex[15]}},
        {vars_6_7_complex[6], {vars_6_7_complex[8], vars_6_7_complex[10], vars_6_7_complex[12]}},
        {vars_6_7_complex[7], {vars_6_7_complex[15]}},
        {vars_6_7_complex[8], {vars_6_7_complex[4], vars_6_7_complex[6]}},
        {vars_6_7_complex[9], {vars_6_7_complex[0], vars_6_7_complex[4]}},
        {vars_6_7_complex[10], {vars_6_7_complex[4], vars_6_7_complex[5], vars_6_7_complex[6]}},
        {vars_6_7_complex[11], {vars_6_7_complex[0], vars_6_7_complex[2]}},
        {vars_6_7_complex[12], {vars_6_7_complex[5], vars_6_7_complex[6]}},
        {vars_6_7_complex[13], {vars_6_7_complex[2], vars_6_7_complex[3], vars_6_7_complex[5]}},
        {vars_6_7_complex[14], {vars_6_7_complex[1], vars_6_7_complex[2], vars_6_7_complex[3]}},
        {vars_6_7_complex[15], {vars_6_7_complex[5], vars_6_7_complex[7]}},
        {vars_6_7_complex[16], {vars_6_7_complex[1], vars_6_7_complex[2], vars_6_7_complex[3]}},
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