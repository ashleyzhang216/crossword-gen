// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: test driver implementation for cw_csp
// ==================================================================

#include "cw_csp_test_driver.h"

using namespace cw_csp_test_driver_ns;

/**
 * @brief basic constructor for cw_csp test driver
*/
cw_csp_test_driver::cw_csp_test_driver(string name) : common_parent(name) {
    // do nothing
}

/**
 * @brief test for proper csp initialization of crossword w/o predefined contents
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param filepath relative filepath to dictionary of words file 
*/
bool cw_csp_test_driver::test_constructor_empty(uint length, uint height, string filepath) {
    stringstream dut_name;
    dut_name << name << " test_constructor_empty(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, filepath);

    return false;
}

/**
 * @brief test for proper csp initialization of crossword w/o predefined contents
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param contents str representation of predefined crossword contents
 * @param filepath relative filepath to dictionary of words file 
*/
bool cw_csp_test_driver::test_constructor_contents(uint length, uint height, string contents, string filepath) {
    stringstream dut_name;
    dut_name << name << " test_constructor_contents(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath);

    return false;
}