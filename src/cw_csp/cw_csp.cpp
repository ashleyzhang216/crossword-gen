// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class implementation for crossword constraint satisfaction problem 
// ==================================================================

#include "cw_csp.h"

using namespace cw_csp_ns;

/**
 * @brief constructor for constraint satisfaction problem w/o puzzle contents
 * 
 * @param length the length of puzzle to be created
 * @param height the height of puzzle to be created
 * @param filepath relative filepath to dictionary of words file
*/
cw_csp::cw_csp(string name, uint length, uint height, string filepath) : common_parent(name) {
    cw = std::make_unique<crossword>(name + " cw", length, height);
    finder = make_shared<word_finder>(name + " wf", filepath);
    initialize_csp();
}

/**
 * @brief constructor for constraint satisfaction problem with puzzle contents
 * 
 * @param length the length of puzzle to be created
 * @param height the height of puzzle to be created
 * @param contents the contents to populate puzzle with
 * @param filepath relative filepath to dictionary of words file
*/
cw_csp::cw_csp(string name, uint length, uint height, string contents, string filepath) : common_parent(name) {
    cw = std::make_unique<crossword>(name + " cw", length, height, contents);
    finder = make_shared<word_finder>(name + " wf", filepath);
    initialize_csp();
}

/**
 * @brief getter function for variables by value
 * 
 * @return copy of variables with all pointers dereferenced
*/
unordered_set<cw_variable> cw_csp::get_variables() const {
    unordered_set<cw_variable> result;
    for(shared_ptr<cw_variable> var_ptr : variables) {
        result.insert(*var_ptr);
    }
    return result;
}

/**
 * @brief getter function for constraints by value
 * 
 * @return copy of constraints with all pointers dereferenced
*/
unordered_set<cw_constraint> cw_csp::get_constraints() const {
    unordered_set<cw_constraint> result;
    for(shared_ptr<cw_constraint> constr_ptr : constraints) {
        result.insert(*constr_ptr);
    }
    return result;
}

/**
 * @brief getter function for arc_depenencies by value
 * 
 * @return copy of arc_depenencies with all pointers de-referenced
*/
unordered_map<cw_variable, unordered_set<cw_variable> > cw_csp::get_arc_dependencies() const {
    unordered_map<cw_variable, unordered_set<cw_variable> > result;
    for(const auto& pair : arc_dependencies) {
        for(const auto& ptr : arc_dependencies.at(pair.first)) {
            result[*(pair.first)].insert(*ptr);
        }
    }
    return result;
}

/**
 * @brief constructor helper method to populate all csp variables/constraints based on cw
*/
void cw_csp::initialize_csp() {

    ss << "cw_csp starting csp initialization";
    utils->print_msg(&ss, DEBUG);

    // local vars for finding horizontal & vertical cw_variable
    bool traversing_word; // currently iterating through variable
    stringstream word_pattern; // pattern formed by word so far
    uint cur_var_row, cur_var_col, cur_var_len; // valid iff traversing_word

    ss << "cw_csp searching for horizontal variables";
    utils->print_msg(&ss, DEBUG);

    // find horizontal variables
    for(uint row = 0; row < cw->rows(); row++) {

        // reset
        traversing_word = false;
        word_pattern.str("");

        for(uint col = 0; col < cw->cols(); col++) {
            if(cw->read_at(row, col) != BLACK) {
                // not black tile, in a word
                
                if(!traversing_word) {
                    // not already in a word, is new
                    cur_var_row = row;
                    cur_var_col = col;
                    cur_var_len = 0;
                    word_pattern.str("");
                    traversing_word = true;
                }

                // add current char to pattern
                word_pattern << cw->read_at(row, col);
                cur_var_len++;

            } else {
                // black tile, not in a word

                if(traversing_word) {
                    // was previously iterating word
                    // save progress as new word

                    // single letters are not full words
                    if(cur_var_len >= MIN_WORD_LEN) {
                        // save new variable
                        shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), finder);
                        ss << "adding new variable: " << *new_var;
                        utils->print_msg(&ss, DEBUG);
                        variables.insert(new_var);
                    }

                } else {
                    // was not previously iterating word
                    // caused by black tile in first col of row, or 2+ consecutive black tiles
                    
                    // do nothing
                }

                traversing_word = false;
                word_pattern.str("");
            }
        }

        if(traversing_word && cur_var_len >= MIN_WORD_LEN) {
            // applicable if the last space in a row is blank
            shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), finder);
            ss << "adding new variable: " << *new_var;
            utils->print_msg(&ss, DEBUG);
            variables.insert(new_var);
        }
    }

    ss << "cw_csp searching for vertical variables";
    utils->print_msg(&ss, DEBUG);

    // find vertical variables
    for(uint col = 0; col < cw->cols(); col++) {

        // reset
        traversing_word = false;
        word_pattern.str("");

        for(uint row = 0; row < cw->rows(); row++) {
            if(cw->read_at(row, col) != BLACK) {
                // not black tile, in a word
                
                if(!traversing_word) {
                    // not already in a word, is new
                    cur_var_row = row;
                    cur_var_col = col;
                    cur_var_len = 0;
                    word_pattern.str("");
                    traversing_word = true;
                }

                // add current char to pattern
                word_pattern << cw->read_at(row, col);
                cur_var_len++;

            } else {
                // black tile, not in a word

                if(traversing_word) {
                    // was previously iterating word
                    // save progress as new word

                    // single letters are not full words
                    if(cur_var_len >= MIN_WORD_LEN) {
                        // save new variable
                        shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), finder);
                        ss << "adding new variable: " << *new_var;
                        utils->print_msg(&ss, DEBUG);
                        variables.insert(new_var);
                    }

                } else {
                    // was not previously iterating word
                    // caused by black tile in first row of col, or 2+ consecutive black tiles
                    
                    // do nothing
                }

                traversing_word = false;
                word_pattern.str("");
            }
        }

        if(traversing_word && cur_var_len >= MIN_WORD_LEN) {
            // applicable if the last 2+ spaces in a row are blank
            shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), finder);
            ss << "adding new variable: " << *new_var;
            utils->print_msg(&ss, DEBUG);
            variables.insert(new_var);
        }
    }

    // helper table to build valid constraints
    // var_intersect_table[i][j] corresponds to cw[i][j] 
    vector<vector<cw_constraint> > var_intersect_table(cw->rows(), vector<cw_constraint>(cw->cols()));

    ss << "cw_csp building var_intersect_table";
    utils->print_msg(&ss, DEBUG);

    // iterate across each variable to populate var_intersect_table
    for(shared_ptr<cw_variable> var_ptr : variables) {
        cw_variable var = *var_ptr;
        if(var.dir == HORIZONTAL) {
            for(uint letter = 0; letter < var.length; letter++) {
                if(var_intersect_table[var.origin_row][var.origin_col + letter].lhs == nullptr) {
                    // first variable for this tile
                    var_intersect_table[var.origin_row][var.origin_col + letter].lhs = var_ptr;
                    var_intersect_table[var.origin_row][var.origin_col + letter].lhs_index = letter;
                } else {
                    // second variable for this tile
                    var_intersect_table[var.origin_row][var.origin_col + letter].rhs = var_ptr;
                    var_intersect_table[var.origin_row][var.origin_col + letter].rhs_index = letter;
                }
            }
        } else { // var.dir == VERTICAL
            for(uint letter = 0; letter < var.length; letter++) {
                if(var_intersect_table[var.origin_row + letter][var.origin_col].lhs == nullptr) {
                    // first variable for this tile
                    var_intersect_table[var.origin_row + letter][var.origin_col].lhs = var_ptr;
                    var_intersect_table[var.origin_row + letter][var.origin_col].lhs_index = letter;
                } else {
                    // second variable for this tile
                    var_intersect_table[var.origin_row + letter][var.origin_col].rhs = var_ptr;
                    var_intersect_table[var.origin_row + letter][var.origin_col].rhs_index = letter;
                }
            }
        }
    }

    ss << "cw_csp populating constraints";
    utils->print_msg(&ss, DEBUG);

    // find the valid constraints in var_intersect_table (ones with 2 variables) to add to constraints
    for(uint row = 0; row < cw->rows(); row++) {
        for(uint col = 0; col < cw->cols(); col++) {
            if(var_intersect_table[row][col].rhs != nullptr) {
                // assert that 2 variables exist in constraint
                if(var_intersect_table[row][col].lhs == nullptr) {
                    ss << "var_intersect_table has nullptr lhs with non-nullptr rhs";
                    utils->print_msg(&ss, ERROR);
                }

                shared_ptr<cw_constraint> forward_arc = make_shared<cw_constraint>(
                    var_intersect_table[row][col].lhs_index,
                    var_intersect_table[row][col].rhs_index,
                    var_intersect_table[row][col].lhs,
                    var_intersect_table[row][col].rhs
                );
                ss << "adding forward arc: " << *forward_arc;
                utils->print_msg(&ss, DEBUG);
                
                shared_ptr<cw_constraint> backwards_arc = make_shared<cw_constraint>(
                    var_intersect_table[row][col].rhs_index,
                    var_intersect_table[row][col].lhs_index,
                    var_intersect_table[row][col].rhs,
                    var_intersect_table[row][col].lhs
                );
                ss << "adding backwards arc: " << *backwards_arc;
                utils->print_msg(&ss, DEBUG);

                // add arcs
                constraints.insert(forward_arc);
                constraints.insert(backwards_arc);
            }
        }
    }

    ss << "cw_csp building arc_dependencies";
    utils->print_msg(&ss, DEBUG);

    // build arc table to list out all dependencies for easy arc queueing in AC-3
    arc_dependencies.clear();
    for(shared_ptr<cw_constraint> constraint_ptr : constraints) {

        //arc_dependencies[variable_ptrs[*constraint.rhs]].insert(variable_ptrs[*constraint.lhs]);
        arc_dependencies[constraint_ptr->rhs].insert(constraint_ptr->lhs);
    }
}