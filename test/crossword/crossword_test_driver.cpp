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

    stringstream result;
    result << *dut;
    return check_condition(dut_name.str(), result.str() == ground_truth);
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

    stringstream result;
    result << *dut;
    return check_condition(dut_name.str(), result.str() == ground_truth);
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

    stringstream result;
    result << *dut;
    return check_condition(dut_name.str(), result.str() == ground_truth);
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
    test_at_depth = [this, &test_at_depth, &assignments, &ground_truth](size_t i) -> bool {
        if(i >= assignments.size()) {
            return true;
        }

        bool result = true;
        string cur_word = assignments[i].word;

        dut->write(std::move(assignments[i]));
        stringstream pre_result;
        pre_result << *dut;
        result &= pre_result.str() == ground_truth[i];

        result &= test_at_depth(i + 1);

        stringstream post_result;
        post_result << *dut;
        result &= post_result.str() == ground_truth[i];
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

/**
 * @brief test for proper functionality of write_at(), MUST have run constructor test first
 * @param c the char to write
 * @param row target row
 * @param col target column
 * @param ground_truth expected result of << cast to string AFTER running write_at()
*/
// bool crossword_test_driver::test_write_at(char c, uint row, uint col, string ground_truth) {
//     assert(dut != nullptr);
//     dut->write_at(c, row, col);

//     stringstream dut_name;
//     dut_name << name << " test_write_at(): " << row << ", " << col << " w/ " << c;

//     stringstream result;
//     result << *dut;
//     return check_condition(dut_name.str(), result.str() == ground_truth);
// }

/**
 * @brief test for proper functionality of read_at(), MUST have run constructor test first
 * @param row target row
 * @param col target column
 * @param ground_truth expected output of read_at();
*/
// bool crossword_test_driver::test_read_at(uint row, uint col, char ground_truth) {
//     assert(dut != nullptr);

//     stringstream dut_name;
//     dut_name << name << " test_read_at(): " << row << ", " << col;
//     return check_condition(dut_name.str(), ground_truth == dut->read_at(row, col));
// }
