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