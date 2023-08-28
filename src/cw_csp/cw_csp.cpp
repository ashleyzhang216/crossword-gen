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
unordered_map<cw_variable, unordered_set<cw_constraint> > cw_csp::get_arc_dependencies() const {
    unordered_map<cw_variable, unordered_set<cw_constraint> > result;
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
        arc_dependencies[constraint_ptr->rhs].insert(constraint_ptr);
    }
}

/**
 * @brief AC-3 algorithm to reduce CSP; does not changed CSP if running AC-3 to completion would result in an invalid CSP
 * 
 * @return true iff resulting CSP is valid, i.e. all resulting variables have a non-empty domain
*/
bool cw_csp::ac3() {

    ss << "starting AC-3 algorithm";
    utils->print_msg(&ss, DEBUG);

    // constraints to be checked
    queue<shared_ptr<cw_constraint> > constraint_queue;

    // for O(1) lookup of queue contents to avoid duplicate constraints in queue
    // a constraint c is in constraints_in_queue iff c also in constraint_queue
    unordered_set<shared_ptr<cw_constraint> > constraints_in_queue;

    ss << "initializing queue of " << constraints.size() << " constraints";
    utils->print_msg(&ss, DEBUG);

    // initialize constraint_queue
    for(shared_ptr<cw_constraint> constraint_ptr : constraints) {
        constraint_queue.push(constraint_ptr);
        constraints_in_queue.insert(constraint_ptr);
    }

    /**
     * while constraint queue not empty
     *      constr(Xi, Xj) = queue.pop()
     *      bool changed = Xi.revise(Xj)
     *      if changed
     *          for dep_constraint in arc_dependencies(Xi)
     *              if dep_constraint not in constraint queue
     *                  add dep_constraint to constraint queue
    */

    // map from variable --> words pruned from its domain
    // tracks pruned words to undo AC-3 if resulting CSP is invalid
    unordered_map<shared_ptr<cw_variable>, unordered_set<string> > pruned_domains;

    // run AC-3 algo
    shared_ptr<cw_constraint> constr;
    unordered_set<string> pruned_words;
    while(!constraint_queue.empty()) {
        // pop top constraint
        constr = constraint_queue.front();
        constraint_queue.pop();
        assert(constraints_in_queue.count(constr) > 0);
        constraints_in_queue.erase(constr);

        ss << "considering constraint: " << *constr;
        utils->print_msg(&ss, DEBUG);

        // prune invalid words in domain
        pruned_words = constr->prune_domain();

        // track pruned words for each var in case undo is needed if CSP becomes invalid
        for(string pruned_word : pruned_words) {
            ss << "pruned word from domain: " << pruned_word;
            utils->print_msg(&ss, DEBUG);

            pruned_domains[constr->lhs].insert(pruned_word);
        }

        // if domain was changed while pruning, add dependent arcs to constraint queue
        if(pruned_words.size() > 0) {
            // check that CSP is still valid, i.e. var has non-empty domain
            if(constr->lhs->domain.size() == 0) {
                
                ss << "CSP became invalid, undo-ing pruning";
                utils->print_msg(&ss, DEBUG);

                // "undo" to re-add all pruned words to domain of each var if CSP becomes invalid
                for(const auto& pair : pruned_domains) {
                    for(string pruned_word : pair.second) {
                        pair.first->domain.insert(pruned_word);
                    }
                }
                return false;
            }

            // add dependent arcs to queue
            for(shared_ptr<cw_constraint> constr_ptr : arc_dependencies[constr->lhs]) {
                if(constraints_in_queue.count(constr_ptr) == 0) {
                    ss << "adding dependent constraint to queue: " << *constr_ptr;
                    utils->print_msg(&ss, DEBUG);

                    constraint_queue.push(constr_ptr);
                    constraints_in_queue.insert(constr_ptr);
                }
            }
        }
    }

    // double-check to verify CSP is still valid
    // TODO: remove once robust tests are written to validate this function
    for(shared_ptr<cw_variable> var_ptr : variables) {
        assert(var_ptr->domain.size() > 0);
    }

    return true;
}