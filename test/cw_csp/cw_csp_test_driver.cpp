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
 * @param expected_variables ref to expected contents of variables set after initialization
 * @param expected_constraints ref to expected contents of constraints set after initialization
 * @param expected_constr_dependencies ref to expected dereferenced contents of constraint dependencies map after initialization
 * @return true iff successful
*/
bool cw_csp_test_driver::test_constructor_empty(
    uint length, uint height, string filepath,
    const unordered_set<unique_ptr<cw_variable> >& expected_variables,
    const unordered_set<unique_ptr<cw_constraint> >& expected_constraints,
    const unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& expected_constr_dependencies
) {
    stringstream cw_name;
    stringstream dut_name;
    cw_name  << name << " test_constructor_empty(): " << length << ", " << height << " cw";
    dut_name << name << " test_constructor_empty(): " << length << ", " << height;

    crossword cw(cw_name.str(), length, height);
    dut = make_unique<cw_csp>(dut_name.str(), std::move(cw), filepath, false, false);

    bool result = true;
    unordered_set<unique_ptr<cw_variable> >   result_variables   = dut->get_variables();
    unordered_set<unique_ptr<cw_constraint> > result_constraints = dut->get_constraints();
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > result_constr_dependencies = dut->get_constr_dependencies();
    unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > result_var_domains;
    unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > expected_var_domains;
    for(const auto& var : result_variables) {
        vector<word_t> domain_vec = var->domain.get_cur_domain();
        result_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
    }
    for(const auto& var : expected_variables) {
        vector<word_t> domain_vec = var->domain.get_cur_domain();
        expected_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
    }

    result &= check_condition(dut_name.str() + " vars",         set_contents_equal(result_variables,   expected_variables, true));
    result &= check_condition(dut_name.str() + " constraints",  set_contents_equal(result_constraints, expected_constraints, true));
    result &= check_condition(dut_name.str() + " dependencies", map_to_set_contents_equal(result_constr_dependencies, expected_constr_dependencies, true));    
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
 * @param expected_constr_dependencies ref to expected dereferenced contents of constraint dependencies map after initialization
 * @return true iff successful
*/
bool cw_csp_test_driver::test_constructor_contents(
    uint length, uint height, string contents, string filepath,
    const unordered_set<unique_ptr<cw_variable> >& expected_variables,
    const unordered_set<unique_ptr<cw_constraint> >& expected_constraints,
    const unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& expected_constr_dependencies
) {
    stringstream cw_name;
    stringstream dut_name;
    cw_name  << name << " test_constructor_contents(): " << length << ", " << height << " cw";
    dut_name << name << " test_constructor_contents(): " << length << ", " << height;

    crossword cw(cw_name.str(), length, height, contents);
    dut = make_unique<cw_csp>(dut_name.str(), std::move(cw), filepath, false, false);

    bool result = true;
    unordered_set<unique_ptr<cw_variable> >   result_variables   = dut->get_variables();
    unordered_set<unique_ptr<cw_constraint> > result_constraints = dut->get_constraints();
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > result_constr_dependencies = dut->get_constr_dependencies();
    unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > result_var_domains;
    unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > expected_var_domains;
    for(const auto& var : result_variables) {
        vector<word_t> domain_vec = var->domain.get_cur_domain();
        result_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
    }
    for(const auto& var : expected_variables) {
        vector<word_t> domain_vec = var->domain.get_cur_domain();
        expected_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
    }

    result &= check_condition(dut_name.str() + " vars",         set_contents_equal(result_variables,   expected_variables, true));
    result &= check_condition(dut_name.str() + " constraints",  set_contents_equal(result_constraints, expected_constraints, true));
    result &= check_condition(dut_name.str() + " dependencies", map_to_set_contents_equal(result_constr_dependencies, expected_constr_dependencies, true));
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
    stringstream cw_name;
    stringstream dut_name;
    cw_name  << name << " test_ac3_validity(): " << length << ", " << height << " cw";
    dut_name << name << " test_ac3_validity(): " << length << ", " << height;

    crossword cw(cw_name.str(), length, height, contents);
    dut = make_unique<cw_csp>(dut_name.str(), std::move(cw), filepath, false, false);

    bool result = true;
    unordered_set<unique_ptr<cw_variable> > original_variables = dut->get_variables();

    result &= check_condition(dut_name.str() + " ac3 validity", expected_result == dut->ac3());
    if(!expected_result) {
        // check var equality
        unordered_set<unique_ptr<cw_variable> > unchanged_variables = dut->get_variables();
        result &= check_condition(dut_name.str() + " unchanged vars", set_contents_equal(original_variables, unchanged_variables, true));

        // check var domain equality
        unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > result_var_domains;
        unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > expected_var_domains;
        for(const auto& var : unchanged_variables) {
            vector<word_t> domain_vec = var->domain.get_cur_domain();
            result_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
        }
        for(const auto& var : original_variables) {
            vector<word_t> domain_vec = var->domain.get_cur_domain();
            expected_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
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
bool cw_csp_test_driver::test_ac3(uint length, uint height, string contents, string filepath, bool expected_result, const unordered_set<unique_ptr<cw_variable> >& expected_variables) {
    stringstream cw_name;
    stringstream dut_name;
    cw_name  << name << " test_ac3(): " << length << ", " << height << " cw";
    dut_name << name << " test_ac3(): " << length << ", " << height;

    crossword cw(cw_name.str(), length, height, contents);
    dut = make_unique<cw_csp>(dut_name.str(), std::move(cw), filepath, false, false);

    bool result = true;
    unordered_set<unique_ptr<cw_variable> > original_variables = dut->get_variables();

    result &= check_condition(dut_name.str() + " ac3 validity", expected_result == dut->ac3());
    unordered_set<unique_ptr<cw_variable> > result_variables = dut->get_variables();
    unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > result_var_domains;
    unordered_map<unique_ptr<cw_variable>, unordered_set<word_t> > expected_var_domains;
    if(!expected_result) {
        // invalid crosswords
        result &= check_condition(dut_name.str() + " unchanged vars", set_contents_equal(result_variables, original_variables, true));

        // check var domain equality
        for(const auto& var : result_variables) {
            vector<word_t> domain_vec = var->domain.get_cur_domain();
            result_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
        }
        for(const auto& var : original_variables) {
            vector<word_t> domain_vec = var->domain.get_cur_domain();
            expected_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
        }
        result &= check_condition(dut_name.str() + " var domains",  map_to_set_contents_equal(result_var_domains, expected_var_domains, true));
    } else {
        // valid crosswords
        result &= check_condition(dut_name.str() + " simplified vars", set_contents_equal(result_variables, expected_variables, true));

        // check var domain equality
        for(const auto& var : result_variables) {
            vector<word_t> domain_vec = var->domain.get_cur_domain();
            result_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
        }
        for(const auto& var : expected_variables) {
            vector<word_t> domain_vec = var->domain.get_cur_domain();
            expected_var_domains.insert(std::make_pair(var->clone(), unordered_set<word_t>(domain_vec.begin(), domain_vec.end())));
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
    stringstream cw_name;
    stringstream dut_name;
    cw_name  << name << " test_backtracking_validity(): " << length << ", " << height << " cw";
    dut_name << name << " test_backtracking_validity(): " << length << ", " << height;

    crossword cw(cw_name.str(), length, height, contents);
    dut = make_unique<cw_csp>(dut_name.str(), std::move(cw), filepath, false, false);

    bool result = check_condition(dut_name.str() + " backtracking validity", dut->solve(BACKTRACKING, MIN_REMAINING_VALUES) == expected_result);
    if(result && expected_result && do_print) {
        utils.log(DEBUG, dut->result());
    }
    return result;
}
