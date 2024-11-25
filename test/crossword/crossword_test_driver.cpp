// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: test driver implementation for crossword
// ==================================================================

#include "crossword_test_driver.h"

using namespace crossword_test_driver_ns;

/**
 * @brief basic constructor for crossword test driver
*/
crossword_test_driver::crossword_test_driver(string name) : common_parent(name, VERBOSITY) {
    // do nothing
}

/**
 * @brief test for crossword constructor accuracy without contents
 * @param length the length of crossword
 * @param height the height of crossword
 * @param ground_truth expected result of << operator cast to string
*/
bool crossword_test_driver::test_constructor_empty(uint length, uint height, string ground_truth) {
    stringstream dut_name;
    dut_name << name << " test_constructor_empty(): " << length << ", " << height;
    dut = make_unique<crossword>(dut_name.str(), length, height);

    return check_condition(dut_name.str(), dut->serialize_result() == ground_truth);
}

/**
 * @brief test for crossword constructor accuracy with contents in vector form
 * @param length the length of crossword
 * @param height the height of crossword
 * @param contents new contents of crossword
 * @param ground_truth expected result of << operator cast to string
 * 
 * @return true iff successful
*/
bool crossword_test_driver::test_constructor_contents_vector(uint length, uint height, vector<vector<char> > contents, string ground_truth) {
    stringstream dut_name;
    dut_name << name << " test_constructor_contents_vector(): " << length << ", " << height;
    dut = make_unique<crossword>(dut_name.str(), length, height, contents);

    return check_condition(dut_name.str(), dut->serialize_result() == ground_truth);
}

/**
 * @brief test for crossword constructor accuracy with contents in string form
 * @param length the length of crossword
 * @param height the height of crossword
 * @param contents new contents of crossword
 * @param ground_truth expected result of << operator cast to string
 * 
 * @return true iff successful
*/
bool crossword_test_driver::test_constructor_contents_string(uint length, uint height, string contents, string ground_truth) {
    stringstream dut_name;
    dut_name << name << " test_constructor_contents_string(): " << length << ", " << height;
    dut = make_unique<crossword>(dut_name.str(), length, height, contents);

    return check_condition(dut_name.str(), dut->serialize_result() == ground_truth);
}

/**
 * @brief test for writing 
 * @param length the length of crossword
 * @param height the height of crossword
 * @param contents new contents of crossword
 * @param assignments vec of assignments to execute on grid
 * @param ground_truth expected crossword state after each assignment
 * @pre assignments.size() == ground_truths.size()
 * 
 * @return true iff successful
*/
bool crossword_test_driver::test_modification(uint length, uint height, const string& contents, vector<word_assignment>& assignments, const vector<string>& ground_truth) {
    assert(assignments.size() == ground_truth.size());

    stringstream dut_name;
    dut_name << name << " test_modification(): " << length << ", " << height;
    dut = make_unique<crossword>(dut_name.str(), length, height, contents);

    std::function<bool(size_t)> test_at_depth;
    test_at_depth = [this, &test_at_depth, &assignments, &ground_truth, &contents, &length](size_t i) -> bool {
        if(i >= assignments.size()) {
            return true;
        }

        bool result = true;
        string cur_word = assignments[i].word;

        dut->write(std::move(assignments[i]));
        result &= dut->serialize_result() == ground_truth[i];
        result &= dut->serialize_initial() == construct_cw_output(contents, length);

        result &= test_at_depth(i + 1);

        result &= dut->serialize_result() == ground_truth[i];
        result &= dut->serialize_initial() == construct_cw_output(contents, length);
        result &= dut->undo_prev_write() == cur_word;

        return result;
    };

    return test_at_depth(0);
}

/**
 * @brief returns new string with a std::endl inserted before every 'length' chars and a space after every char
*/
string crossword_test_driver_ns::construct_cw_output(const string& ground_truth, uint length) {
    assert(ground_truth.size() % length == 0);

    stringstream result;
    for(size_t i = 0; i < ground_truth.size(); i += length) {
        result << endl;
        for(size_t j = 0; j < length; ++j) {
            result << ground_truth.at(i + j) << ' ';
        }
    }

    return result.str();
}
