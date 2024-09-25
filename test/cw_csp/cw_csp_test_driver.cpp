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
cw_csp_test_driver::cw_csp_test_driver(string name) : common_parent(name, VERBOSITY) {
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
 * @param expected_arc_dependencies ref to expected dereferenced contents of arc dependencies map after initialization
 * @return true iff successful
*/
bool cw_csp_test_driver::test_constructor_empty(
    uint length, uint height, string filepath,
    const unordered_set<cw_variable>& expected_variables,
    const unordered_set<cw_constraint>& expected_constraints,
    const unordered_map<cw_variable, unordered_set<cw_constraint> >& expected_arc_dependencies
) {
    stringstream dut_name;
    dut_name << name << " test_constructor_empty(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, filepath, false, false);

    bool result = true;
    unordered_set<cw_variable>   result_variables   = dut->get_variables();
    unordered_set<cw_constraint> result_constraints = dut->get_constraints();
    unordered_map<cw_variable, unordered_set<cw_constraint> > result_arc_dependencies = dut->get_arc_dependencies();
    unordered_map<cw_variable, unordered_set<word_t> > result_var_domains;
    unordered_map<cw_variable, unordered_set<word_t> > expected_var_domains;
    for(const cw_variable& var : result_variables) {
        vector<word_t> domain_vec = var.domain.get_cur_domain();
        result_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
    }
    for(const cw_variable& var : expected_variables) {
        vector<word_t> domain_vec = var.domain.get_cur_domain();
        expected_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
    }

    result &= check_condition(dut_name.str() + " vars",         set_contents_equal(result_variables,   expected_variables, true));
    result &= check_condition(dut_name.str() + " constraints",  set_contents_equal(result_constraints, expected_constraints, true));
    result &= check_condition(dut_name.str() + " dependencies", map_to_set_contents_equal(result_arc_dependencies, expected_arc_dependencies, true));    
    result &= check_condition(dut_name.str() + " var domains",  map_to_set_contents_equal(result_var_domains, expected_var_domains, true));

    return result;
}

/**
 * @brief test for proper csp initialization of crossword w/o predefined contents
 * 
 * @param length the length of the crossword
 * @param height the height of the crossword
 * @param contents str representation of predefined crossword contents
 * @param filepath relative filepath to dictionary of words file 
 * @param expected_variables ref to expected contents of variables set after initialization
 * @param expected_constraints ref to expected contents of constraints set after initialization
 * @param expected_arc_dependencies ref to expected dereferenced contents of arc dependencies map after initialization
 * @return true iff successful
*/
bool cw_csp_test_driver::test_constructor_contents(
    uint length, uint height, string contents, string filepath,
    const unordered_set<cw_variable>& expected_variables,
    const unordered_set<cw_constraint>& expected_constraints,
    const unordered_map<cw_variable, unordered_set<cw_constraint> >& expected_arc_dependencies
) {
    stringstream dut_name;
    dut_name << name << " test_constructor_contents(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath, false, false);

    bool result = true;
    unordered_set<cw_variable>   result_variables   = dut->get_variables();
    unordered_set<cw_constraint> result_constraints = dut->get_constraints();
    unordered_map<cw_variable, unordered_set<cw_constraint> > result_arc_dependencies = dut->get_arc_dependencies();
    unordered_map<cw_variable, unordered_set<word_t> > result_var_domains;
    unordered_map<cw_variable, unordered_set<word_t> > expected_var_domains;
    for(const cw_variable& var : result_variables) {
        vector<word_t> domain_vec = var.domain.get_cur_domain();
        result_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
    }
    for(const cw_variable& var : expected_variables) {
        vector<word_t> domain_vec = var.domain.get_cur_domain();
        expected_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
    }

    result &= check_condition(dut_name.str() + " vars",         set_contents_equal(result_variables,   expected_variables, true));
    result &= check_condition(dut_name.str() + " constraints",  set_contents_equal(result_constraints, expected_constraints, true));
    result &= check_condition(dut_name.str() + " dependencies", map_to_set_contents_equal(result_arc_dependencies, expected_arc_dependencies, true));
    result &= check_condition(dut_name.str() + " var domains",  map_to_set_contents_equal(result_var_domains, expected_var_domains, true));

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
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath, false, false);

    bool result = true;
    unordered_set<cw_variable> original_variables = dut->get_variables();

    result &= check_condition(dut_name.str() + " ac3 validity", expected_result == dut->ac3());
    if(!expected_result) {
        // check var equality
        unordered_set<cw_variable> unchanged_variables = dut->get_variables();
        result &= check_condition(dut_name.str() + " unchanged vars", set_contents_equal(original_variables, unchanged_variables, true));

        // check var domain equality
        unordered_map<cw_variable, unordered_set<word_t> > result_var_domains;
        unordered_map<cw_variable, unordered_set<word_t> > expected_var_domains;
        for(const cw_variable& var : unchanged_variables) {
            vector<word_t> domain_vec = var.domain.get_cur_domain();
            result_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
        }
        for(const cw_variable& var : original_variables) {
            vector<word_t> domain_vec = var.domain.get_cur_domain();
            expected_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
        }
        result &= check_condition(dut_name.str() + " var domains",  map_to_set_contents_equal(result_var_domains, expected_var_domains, true));
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
 * @param expected_variables ref to expected contents of variables set after running AC-3
 * @return true iff successful
*/
bool cw_csp_test_driver::test_ac3(uint length, uint height, string contents, string filepath, bool expected_result, const unordered_set<cw_variable>& expected_variables) {
    stringstream dut_name;
    dut_name << name << " test_ac3(): " << length << ", " << height;
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath, false, false);

    bool result = true;
    unordered_set<cw_variable> original_variables = dut->get_variables();

    result &= check_condition(dut_name.str() + " ac3 validity", expected_result == dut->ac3());
    unordered_set<cw_variable> result_variables = dut->get_variables();
    if(!expected_result) {
        // invalid crosswords
        result &= check_condition(dut_name.str() + " unchanged vars", set_contents_equal(result_variables, original_variables, true));

        // check var domain equality
        unordered_map<cw_variable, unordered_set<word_t> > result_var_domains;
        unordered_map<cw_variable, unordered_set<word_t> > expected_var_domains;
        for(const cw_variable& var : result_variables) {
            vector<word_t> domain_vec = var.domain.get_cur_domain();
            result_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
        }
        for(const cw_variable& var : original_variables) {
            vector<word_t> domain_vec = var.domain.get_cur_domain();
            expected_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
        }
        result &= check_condition(dut_name.str() + " var domains",  map_to_set_contents_equal(result_var_domains, expected_var_domains, true));
    } else {
        // valid crosswords
        result &= check_condition(dut_name.str() + " simplified vars", set_contents_equal(result_variables, expected_variables, true));

        // check var domain equality
        unordered_map<cw_variable, unordered_set<word_t> > result_var_domains;
        unordered_map<cw_variable, unordered_set<word_t> > expected_var_domains;
        for(const cw_variable& var : result_variables) {
            vector<word_t> domain_vec = var.domain.get_cur_domain();
            result_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
        }
        for(const cw_variable& var : expected_variables) {
            vector<word_t> domain_vec = var.domain.get_cur_domain();
            expected_var_domains.insert({var, unordered_set<word_t>(domain_vec.begin(), domain_vec.end())});
        }
        result &= check_condition(dut_name.str() + " var domains",  map_to_set_contents_equal(result_var_domains, expected_var_domains, true));
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
    dut = new cw_csp(dut_name.str(), length, height, contents, filepath, false, false);

    bool result = check_condition(dut_name.str() + " backtracking validity", dut->solve(BACKTRACKING, MIN_REMAINING_VALUES) == expected_result);
    if(result && expected_result && do_print) {
        utils.log(DEBUG, dut->result());
    }
    return result;
}