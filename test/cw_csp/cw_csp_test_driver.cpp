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
 * @param expected_variables ptr to expected contents of variables set after initialization
 * @param expected_constraints ptr to expected contents of constraints set after initialization
 * @param expected_arc_dependencies expected dereferenced contents of arc dependencies map after initialization
 * @return true iff successful
*/
bool cw_csp_test_driver::test_constructor_empty(
    uint length, uint height, string filepath,
    unordered_set<cw_variable>* expected_variables,
    unordered_set<cw_constraint>* expected_constraints,
    unordered_map<cw_variable, unordered_set<cw_constraint> >* expected_arc_dependencies
) {
    stringstream dut_name;
    dut_name << name << " test_constructor_empty(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, filepath);

    bool result = true;
    unordered_set<cw_variable>   result_variables   = dut->get_variables();
    unordered_set<cw_constraint> result_constraints = dut->get_constraints();
    unordered_map<cw_variable, unordered_set<cw_constraint> > result_arc_dependencies = dut->get_arc_dependencies();

    result &= check_condition(dut_name.str() + " vars",         set_contents_equal(&result_variables,   expected_variables, true));
    result &= check_condition(dut_name.str() + " constraints",  set_contents_equal(&result_constraints, expected_constraints, true));
    result &= check_condition(dut_name.str() + " dependencies", map_to_set_contents_equal(&result_arc_dependencies, expected_arc_dependencies, true));

    return result;
}

/**
 * @brief test for proper csp initialization of crossword w/o predefined contents
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param contents str representation of predefined crossword contents
 * @param filepath relative filepath to dictionary of words file 
 * @param expected_variables ptr to expected contents of variables set after initialization
 * @param expected_constraints ptr to expected contents of constraints set after initialization
 * @param expected_arc_dependencies expected dereferenced contents of arc dependencies map after initialization
 * @return true iff successful
*/
bool cw_csp_test_driver::test_constructor_contents(
    uint length, uint height, string contents, string filepath,
    unordered_set<cw_variable>* expected_variables,
    unordered_set<cw_constraint>* expected_constraints,
    unordered_map<cw_variable, unordered_set<cw_constraint> >* expected_arc_dependencies
) {
    stringstream dut_name;
    dut_name << name << " test_constructor_contents(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath);

    bool result = true;
    unordered_set<cw_variable>   result_variables   = dut->get_variables();
    unordered_set<cw_constraint> result_constraints = dut->get_constraints();
    unordered_map<cw_variable, unordered_set<cw_constraint> > result_arc_dependencies = dut->get_arc_dependencies();

    result &= check_condition(dut_name.str() + " vars",         set_contents_equal(&result_variables,   expected_variables, true));
    result &= check_condition(dut_name.str() + " constraints",  set_contents_equal(&result_constraints, expected_constraints, true));
    result &= check_condition(dut_name.str() + " dependencies", map_to_set_contents_equal(&result_arc_dependencies, expected_arc_dependencies, true));

    return result;
}

/**
 * @brief simple test for ac3 algorithm to check if resulting CSP is valid or not
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param contents str representation of predefined crossword contents
 * @param filepath relative filepath to dictionary of words file 
 * @param expected_result expected validity of resulting CSP
 * @return true iff successful
*/
bool cw_csp_test_driver::test_ac3_validity(uint length, uint height, string contents, string filepath, bool expected_result) {
    stringstream dut_name;
    dut_name << name << " test_ac3_validity(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath);

    bool result = true;
    unordered_set<cw_variable> original_variables = dut->get_variables();

    result &= check_condition(dut_name.str() + " ac3 validity", expected_result == dut->ac3());
    if(!expected_result) {
        unordered_set<cw_variable> unchanged_variables = dut->get_variables();
        result &= check_condition(dut_name.str() + " unchanged vars", set_contents_equal(&original_variables, &unchanged_variables, true));
    }

    return result;
}

/**
 * @brief test for domain simplification of AC-3 algorithm
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param contents str representation of predefined crossword contents
 * @param filepath relative filepath to dictionary of words file 
 * @param expected_result expected validity of resulting CSP
 * @param expected_variables ptr to expected contents of variables set after running AC-3
 * @return true iff successful
*/
bool cw_csp_test_driver::test_ac3(uint length, uint height, string contents, string filepath, bool expected_result, unordered_set<cw_variable>* expected_variables) {
    stringstream dut_name;
    dut_name << name << " test_ac3(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath);

    bool result = true;
    unordered_set<cw_variable> original_variables = dut->get_variables();

    result &= check_condition(dut_name.str() + " ac3 validity", expected_result == dut->ac3());
    unordered_set<cw_variable> result_variables = dut->get_variables();
    if(!expected_result) {
        // invalid crosswords
        result &= check_condition(dut_name.str() + " unchanged vars", set_contents_equal(&result_variables, &original_variables, true));
    } else {
        // valid crosswords
        result &= check_condition(dut_name.str() + " simplified vars", set_contents_equal(&result_variables, expected_variables, true));
    }

    return result;
}

/**
 * @brief test for if solving CSP with backtracking is possible
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param contents str representation of predefined crossword contents
 * @param filepath relative filepath to dictionary of words file 
 * @param expected_result expected validity of solving the crossword
 * @param do_print if true, print the resulting solved crossword
 * @return true iff successful
*/
bool cw_csp_test_driver::test_backtracking_validity(uint length, uint height, string contents, string filepath, bool expected_result, bool do_print) {
    stringstream dut_name;
    dut_name << name << " test_backtracking_validity(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath);
    stringstream cw_result;

    bool result = check_condition(dut_name.str() + " backtracking validity", dut->solve(BACKTRACKING, MIN_REMAINING_VALUES) == expected_result);
    if(result && expected_result && do_print) {
        cw_result << dut->result();
        utils->print_msg(&cw_result, DEBUG);
    }
    return result;
}