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
cw_csp::cw_csp(string name, uint length, uint height, string filepath) : common_parent(name), total_domain(name + " total_domain", filepath) {
    cw = std::make_unique<crossword>(name + " cw", length, height);
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
cw_csp::cw_csp(string name, uint length, uint height, string contents, string filepath) : common_parent(name), total_domain(name + " total_domain", filepath) {
    cw = std::make_unique<crossword>(name + " cw", length, height, contents);
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
                        shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), total_domain);
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
            shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), total_domain);
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
                        shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), total_domain);
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
            shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), total_domain);
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
 * @brief AC-3 algorithm to reduce CSP; calls undo_ac3() iff running AC-3 to completion would result in an invalid CSP automatically
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

    // map from variable --> words pruned from its domain
    // tracks pruned words to undo AC-3 if resulting CSP is invalid
    unordered_map<shared_ptr<cw_variable>, unordered_set<word_t> > pruned_domains;

    // initialize constraint_queue
    for(shared_ptr<cw_constraint> constraint_ptr : constraints) {
        constraint_queue.push(constraint_ptr);
        constraints_in_queue.insert(constraint_ptr);
    }

    // run AC-3 algo
    shared_ptr<cw_constraint> constr;
    unordered_set<word_t> pruned_words;
    while(!constraint_queue.empty()) {
        // pop top constraint
        constr = constraint_queue.front();
        constraint_queue.pop();
        assert(constraints_in_queue.count(constr) > 0);
        constraints_in_queue.erase(constr);

        // prune invalid words in domain
        pruned_words = constr->prune_domain();

        // track pruned words for each var in case undo is needed if CSP becomes invalid
        for(word_t pruned_word : pruned_words) {
            pruned_domains[constr->lhs].insert(pruned_word);
        }
        
        // if domain was changed while pruning, add dependent arcs to constraint queue
        if(pruned_words.size() > 0) {
            if(constr->lhs->domain.size() == 0) {
                // CSP is now invalid, i.e. var has empty domain
            
                ss << "CSP became invalid, undo-ing pruning";
                utils->print_msg(&ss, DEBUG);

                // save record of all domain values pruned
                prev_pruned_domains.push(pruned_domains);

                // undo pruning
                undo_ac3();
                return false;
            }

            // add dependent arcs to queue
            for(shared_ptr<cw_constraint> constr_ptr : arc_dependencies[constr->lhs]) {
                if(constraints_in_queue.count(constr_ptr) == 0) {
                    constraint_queue.push(constr_ptr);
                    constraints_in_queue.insert(constr_ptr);
                }
            }
        }
    }

    // save record of all domain values pruned
    prev_pruned_domains.push(pruned_domains);

    // running AC-3 to completion does not make CSP invalid
    return true;
}

/**
 * @brief undo domain pruning from previous call of AC-3 algorithm
*/
void cw_csp::undo_ac3() {
    assert(prev_pruned_domains.size() > 0);

    for(const auto& pair : prev_pruned_domains.top()) {
        // re-add pruned words
        for(word_t pruned_word : pair.second) {
            pair.first->domain.insert(pruned_word);
        }
    }
    prev_pruned_domains.pop();
}

/**
 * @brief checks if this CSP is solved, i.e. all variables have 1 value in domain & all constraints satisfied
 * 
 * @return true iff CSP is solved
*/
bool cw_csp::solved() const {

    unordered_set<word_t> used_words;

    // check that all vars have one remaining domain value & satisfied
    for(shared_ptr<cw_variable> var_ptr : variables) {
        // check that all vars satisifed w/ one domain value
        if(var_ptr->domain.size() != 1) return false;
        if(!var_ptr->assigned) return false;

        // check that domain value is unique
        if(used_words.count(*(var_ptr->domain.begin())) > 0) return false;
        used_words.insert(*(var_ptr->domain.begin()));
    }

    // check that all constraints satisfied
    for(shared_ptr<cw_constraint> constr_ptr : constraints) {
        if(!constr_ptr->satisfied()) return false;
    }

    return true;
}

/**
 * @brief getter for string representation of solved crossword to print
*/
string cw_csp::result() const {
    assert(solved());
    stringstream cw_ss;
    cw_ss << *cw;
    return cw_ss.str();
}

/**
 * @brief overwrite wildcards in cw puzzle with progress so far
 */
void cw_csp::overwrite_cw() {

    // used to track overwritten chars for undo-ing later
    vector<tuple<char, uint, uint> > overwritten_tiles;

    // iterate across each variable to write onto cw
    for(shared_ptr<cw_variable> var_ptr : variables) {
        // only write assigned variables
        if(var_ptr->assigned) {
            cw_variable var = *var_ptr;
            if(var.dir == HORIZONTAL) {
                for(uint letter = 0; letter < var.length; letter++) {
                    // this square must be wildcard or the same letter about to be written
                    assert(
                        cw->read_at(var.origin_row, var.origin_col + letter) == WILDCARD ||
                        cw->read_at(var.origin_row, var.origin_col + letter) == var.domain.begin()->word.at(letter)
                    );

                    // if this will actually overwrite a wildcard
                    if(cw->read_at(var.origin_row, var.origin_col + letter) == WILDCARD) {
                        // record overwritting
                        overwritten_tiles.push_back(std::make_tuple(WILDCARD, var.origin_row, var.origin_col + letter));

                        // overwrite cw
                        cw->write_at(var.domain.begin()->word.at(letter), var.origin_row, var.origin_col + letter);
                    }
                }
            } else { // var.dir == VERTICAL
                for(uint letter = 0; letter < var.length; letter++) {

                    // this square must be wildcard or the same letter about to be written
                    assert(
                        cw->read_at(var.origin_row + letter, var.origin_col) == WILDCARD ||
                        cw->read_at(var.origin_row + letter, var.origin_col) == var.domain.begin()->word.at(letter)
                    );

                    // if this will actually overwrite a wildcard
                    if(cw->read_at(var.origin_row + letter, var.origin_col) == WILDCARD) {
                        // record overwriting
                        overwritten_tiles.push_back(std::make_tuple(WILDCARD, var.origin_row + letter, var.origin_col));

                        // overwrite cw
                        cw->write_at(var.domain.begin()->word.at(letter), var.origin_row + letter, var.origin_col);
                    }
                }
            }
        }
    }

    prev_overwritten_tiles.push(overwritten_tiles);
}

/**
 * @brief undo previous call to overwrite_cw()
*/
void cw_csp::undo_overwrite_cw() {
    assert(prev_overwritten_tiles.size() > 0);

    for(const auto& tuple : prev_overwritten_tiles.top()) {
        cw->write_at(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
    }
    prev_overwritten_tiles.pop();
}

/**
 * @brief selects one unassigned var ptr in variables
 * 
 * @param strategy the selection strategy to use
 * @return next unassigned shared_ptr<cw_variable> in variables to explore, nullptr if none remaining
 * 
 * TODO: currently always selects empty/single-valued vars first due to MIN_REMAINING_VALUES being only strategy, 
 *       this helps backtracking detect invalid assignments asap but this method needs to change if new strategy added
*/
shared_ptr<cw_variable> cw_csp::select_unassigned_var(var_selection_method strategy) {
    shared_ptr<cw_variable> result = nullptr;

    switch(strategy) {
        case MIN_REMAINING_VALUES: {
                unsigned long min_num_values = ULONG_MAX;
                for(shared_ptr<cw_variable> var_ptr : variables) {
                    // if this variable is unassigned AND (no other variable selected OR has fewer remaining values in domain)
                    if(!var_ptr->assigned && (min_num_values == ULONG_MAX || var_ptr->domain.size() < min_num_values)) {
                        min_num_values = var_ptr->domain.size();
                        result = var_ptr;
                    }
                }
            } break;
        default: {
                cw_utils* utils = new cw_utils("select_unassigned_var()", VERBOSITY);
                stringstream ss;
                ss << "got unknown var_selection_method";
                utils->print_msg(&ss, ERROR);
            } break;
    }

    return result;
}

/**
 * @brief solves this CSP using the given strategy
 * 
 * @param csp_strategy strategy to solve this CSP
 * @param var_strategy strategy to select next unassigned variable
 * @param print_progress_bar true for prod to avoid printing during testing
 * @return true iff successful
*/
bool cw_csp::solve(csp_solving_strategy csp_strategy, var_selection_method var_strategy, bool print_progress_bar) {
    // base case for initially invalid crosswords
    if(!ac3()) return false;

    switch(csp_strategy) {
        case BACKTRACKING: {
                return solve_backtracking(var_strategy, print_progress_bar);
            } break;
        default: {
                ss << "solve() got unknown strategy";
                utils->print_msg(&ss, ERROR);
                return false;
            } break;
    }

    return false;
}

/**
 * @brief use backtracking strategy to solve CSP
 * 
 * @param var_strategy strategy to use to select next unassigned variable 
 * @param print_progress_bar true for top level call in prod to avoid printing during testing
 * @return true iff successful
*/
bool cw_csp::solve_backtracking(var_selection_method var_strategy, bool print_progress_bar) {

    ss << "entering solve_backtracking() with cw: " << *cw;
    utils->print_msg(&ss, DEBUG);

    // base case
    if(solved()) return true;

    // select next variable
    shared_ptr<cw_variable> next_var = select_unassigned_var(var_strategy);

    ss << "selected next var: " << *next_var;
    utils->print_msg(&ss, DEBUG);

    // invalid base case: var is invalid, i.e. not assigned AND has domain of one already-assigned word
    if(!next_var->assigned && (next_var->domain.size() == 1 && assigned_words.count(*(next_var->domain.begin())) > 0)) {
        return false;
    }

    // only initialize if this is top level solve_backtracking() call in prod
    unique_ptr<progress_bar> bar = nullptr;
    double prev_progress = 0.0;
    int words_searched = 0;
    if(print_progress_bar) {
        bar = make_unique<progress_bar>(cout, 100u, "Searching", '#', '.');
    }

    // search all possible values, sorted by word score, tiebroken by frequency
    vector<word_t> domain_copy(next_var->domain.begin(), next_var->domain.end());
    auto compare = [](const word_t& lhs, const word_t& rhs) {
        if(lhs.score != rhs.score) return lhs.score > rhs.score;
        return lhs.freq > rhs.freq;
    };
    sort(domain_copy.begin(), domain_copy.end(), compare);

    // iterate through search space for this variable
    for(word_t word : domain_copy) {
        
        // update progress bar if 1% more of progress made
        if(print_progress_bar && (double)words_searched/domain_copy.size() >= prev_progress + 0.01) {
            prev_progress += 0.01;
            bar->write(prev_progress);
        }

        // avoid duplicate words
        if(assigned_words.count(word) == 0) {
            // assignment
            next_var->domain = {word}; // TODO: perhaps use std::swap here
            next_var->assigned = true;
            assigned_words.insert(word);

            ss << "trying new word: " << word;
            utils->print_msg(&ss, DEBUG);

            // if does not result in invalid CSP, recurse
            if(ac3()) {
                ss << "adding new word: " << word << " to var: " << *next_var;
                utils->print_msg(&ss, DEBUG);

                // add to crossword assignment
                overwrite_cw();
                if(solve_backtracking(var_strategy, false)) return true;
                undo_overwrite_cw();
                undo_ac3(); // AC-3 undo automatic iff ac3() returns false
            } 

            ss << "word failed: " << word;
            utils->print_msg(&ss, DEBUG);

            // undo assignment
            next_var->assigned = false;
            assigned_words.erase(word);
        } else {
            ss << "avoided duplicate word: " << word;
            utils->print_msg(&ss, DEBUG);
        }

        // another word eliminated
        words_searched++;
    }

    // after returning here or if solution, progress bar goes out of scope and finishes printing in destructor
    return false;
}