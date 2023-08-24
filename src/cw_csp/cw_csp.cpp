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
    finder = std::make_shared<word_finder>(name + " wf", filepath);
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
    finder = std::make_shared<word_finder>(name + " wf", filepath);
    initialize_csp();
}

/**
 * @brief constructor helper method to populate all csp variables/constraints based on cw
*/
void cw_csp::initialize_csp() {

    // local vars for finding horizontal & vertical cw_variable
    bool traversing_word; // currently iterating through variable
    stringstream word_pattern; // pattern formed by word so far
    uint cur_var_row, cur_var_col, cur_var_len; // valid iff traversing_word

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

                    // save new variable
                    variables.emplace(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), finder);

                } else {
                    // was not previously iterating word
                    // caused by black tile in first col of row, or 2+ consecutive black tiles
                    
                    // do nothing
                }

                traversing_word = false;
                word_pattern.str("");
            }
        }
    }

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

                    // save new variable
                    variables.emplace(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), finder);

                } else {
                    // was not previously iterating word
                    // caused by black tile in first row of col, or 2+ consecutive black tiles
                    
                    // do nothing
                }

                traversing_word = false;
                word_pattern.str("");
            }
        }
    }

    // create shared_ptr<T> for all vars/constraints added
    for(const cw_variable& var : variables) {
        const cw_variable* var_ptr = &var;
        variable_ptrs[var] = std::make_shared<cw_variable>(*var_ptr);
    }
    for(const cw_constraint& constraint : constraints) {
        const cw_constraint* constraint_ptr = &constraint;
        constraint_ptrs[constraint] = std::make_shared<cw_constraint>(*constraint_ptr);
    }

    // var_intersect_table[i][j] corresponds to cw[i][j] 
    vector<vector<cw_constraint> > var_intersect_table;

    // iterate across each variable to populate var_intersect_table
    for(cw_variable var : variables) {
        if(var.dir == HORIZONTAL) {
            for(uint letter = 0; letter < var.length; letter++) {
                if(var_intersect_table[var.origin_row][var.origin_col + letter].lhs == nullptr) {
                    // first variable for this tile
                    var_intersect_table[var.origin_row][var.origin_col + letter].lhs = variable_ptrs[var];
                    var_intersect_table[var.origin_row][var.origin_col + letter].lhs_index = letter;
                } else {
                    // second variable for this tile
                    var_intersect_table[var.origin_row][var.origin_col + letter].rhs = variable_ptrs[var];
                    var_intersect_table[var.origin_row][var.origin_col + letter].rhs_index = letter;
                }
            }
        } else { // var.dir == VERTICAL
            for(uint letter = 0; letter < var.length; letter++) {
                if(var_intersect_table[var.origin_row + letter][var.origin_col].lhs == nullptr) {
                    // first variable for this tile
                    var_intersect_table[var.origin_row + letter][var.origin_col].lhs = variable_ptrs[var];
                    var_intersect_table[var.origin_row + letter][var.origin_col].lhs_index = letter;
                } else {
                    // second variable for this tile
                    var_intersect_table[var.origin_row + letter][var.origin_col].rhs = variable_ptrs[var];
                    var_intersect_table[var.origin_row + letter][var.origin_col].rhs_index = letter;
                }
            }
        }
    }

    // TODO: find the valid constraints in var_intersect_table (ones with 2 variables) to add to constraints

    // TODO: build arc table to figure out what variables affect other variables
}