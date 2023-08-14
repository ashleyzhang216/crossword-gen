// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: test driver implementation for crossword
// ==================================================================

#include "crossword_test_driver.h"

using namespace crossword_test_driver_ns;

/**
 * @brief basic constructor for word_finder test driver
*/
crossword_test_driver::crossword_test_driver(string name) : common_parent(name) {
    // do nothing
}

/**
 * @brief test for crossword constructor accuracy without contents
 * @param length the length of crossword
 * @param height the height of crossword
 * @param ground_truth expected result of << operator cast to string
*/
bool crossword_test_driver::test_constructor_empty(uint length, uint height, string ground_truth) {
    stringstream name;
    name << "test_constructor_empty(): " << length << ", " << height;
    dut = new crossword(name.str(), length, height);

    stringstream result;
    result << *dut;
    return check_condition(name.str(), result.str() == ground_truth);
}

/**
 * @brief test for crossword constructor accuracy with contents in vector form
 * @param length the length of crossword
 * @param height the height of crossword
 * @param contents new contents of crossword
 * @param ground_truth expected result of << operator cast to string
*/
bool crossword_test_driver::test_constructor_contents_vector(uint length, uint height, vector<vector<char> > contents, string ground_truth) {
    stringstream name;
    name << "test_constructor_contents_vector(): " << length << ", " << height;
    dut = new crossword(name.str(), length, height, contents);

    stringstream result;
    result << *dut;
    return check_condition(name.str(), result.str() == ground_truth);
}

/**
 * @brief test for crossword constructor accuracy with contents in string form
 * @param length the length of crossword
 * @param height the height of crossword
 * @param contents new contents of crossword
 * @param ground_truth expected result of << operator cast to string
*/
bool crossword_test_driver::test_constructor_contents_string(uint length, uint height, string contents, string ground_truth) {
    stringstream name;
    name << "test_constructor_contents_string(): " << length << ", " << height;
    dut = new crossword(name.str(), length, height, contents);

    stringstream result;
    result << *dut;
    return check_condition(name.str(), result.str() == ground_truth);
}