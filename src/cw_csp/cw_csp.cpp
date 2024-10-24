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
 * @param print_progress_bar displays progress bar iff true
 * @param use_timetracker enables cw_timetracker iff true
*/
cw_csp::cw_csp(string name, uint length, uint height, string filepath, bool print_progress_bar, bool use_timetracker) 
        : common_parent(name, VERBOSITY), 
          tracker("cw_csp", use_timetracker), 
          cw(name + " cw", length, height), 
          total_domain(name + " total_domain", filepath, print_progress_bar), 
          print_progress_bar(print_progress_bar) {
    initialize_csp();
}

/**
 * @brief constructor for constraint satisfaction problem with puzzle contents
 * 
 * @param length the length of puzzle to be created
 * @param height the height of puzzle to be created
 * @param contents the contents to populate puzzle with
 * @param filepath relative filepath to dictionary of words file
 * @param print_progress_bar displays progress bar iff true
 * @param use_timetracker enables cw_timetracker iff true
*/
cw_csp::cw_csp(string name, uint length, uint height, string contents, string filepath, bool print_progress_bar, bool use_timetracker) 
        : common_parent(name, VERBOSITY), 
          tracker("cw_csp", use_timetracker), 
          cw(name + " cw", length, height, contents), 
          total_domain(name + " total_domain", filepath, print_progress_bar), 
          print_progress_bar(print_progress_bar) {
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
    cw_timestamper stamper(tracker, TS_CSP_INITIALIZE, "");

    utils.log(DEBUG, "cw_csp starting csp initialization");

    // local vars for finding horizontal & vertical cw_variable
    bool traversing_word; // currently iterating through variable
    stringstream word_pattern; // pattern formed by word so far
    uint cur_var_row, cur_var_col, cur_var_len; // valid iff traversing_word

    utils.log(DEBUG, "cw_csp searching for horizontal variables");

    // find horizontal variables
    for(uint row = 0; row < cw.rows(); row++) {

        // reset
        traversing_word = false;
        word_pattern.str("");

        for(uint col = 0; col < cw.cols(); col++) {
            if(cw.read_at(row, col) != BLACK) {
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
                word_pattern << cw.read_at(row, col);
                cur_var_len++;

            } else {
                // black tile, not in a word

                if(traversing_word) {
                    // was previously iterating word
                    // save progress as new word

                    // single letters are not full words
                    if(cur_var_len >= MIN_WORD_LEN) {
                        // save new variable
                        shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), total_domain.find_matches(word_pattern.str()));
                        utils.log(DEBUG, "adding new variable: ", *new_var);
                        variables.push_back(new_var);
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
            shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, HORIZONTAL, word_pattern.str(), total_domain.find_matches(word_pattern.str()));
            utils.log(DEBUG, "adding new variable: ", *new_var);
            variables.push_back(new_var);
        }
    }

    utils.log(DEBUG, "cw_csp searching for vertical variables");

    // find vertical variables
    for(uint col = 0; col < cw.cols(); col++) {

        // reset
        traversing_word = false;
        word_pattern.str("");

        for(uint row = 0; row < cw.rows(); row++) {
            if(cw.read_at(row, col) != BLACK) {
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
                word_pattern << cw.read_at(row, col);
                cur_var_len++;

            } else {
                // black tile, not in a word

                if(traversing_word) {
                    // was previously iterating word
                    // save progress as new word

                    // single letters are not full words
                    if(cur_var_len >= MIN_WORD_LEN) {
                        // save new variable
                        shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), total_domain.find_matches(word_pattern.str()));
                        utils.log(DEBUG, "adding new variable: ", *new_var);
                        variables.push_back(new_var);
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
            shared_ptr<cw_variable> new_var = make_shared<cw_variable>(cur_var_row, cur_var_col, cur_var_len, VERTICAL, word_pattern.str(), total_domain.find_matches(word_pattern.str()));
            utils.log(DEBUG, "adding new variable: ", *new_var);
            variables.push_back(new_var);
        }
    }

    // helper table to build valid constraints
    // var_intersect_table[i][j] corresponds to cw[i][j] 
    vector<vector<cw_constraint> > var_intersect_table(cw.rows(), vector<cw_constraint>(cw.cols()));

    utils.log(DEBUG, "cw_csp building var_intersect_table");

    // iterate across each variable to populate var_intersect_table
    for(shared_ptr<cw_variable> var_ptr : variables) {
        if(var_ptr->dir == HORIZONTAL) {
            for(uint letter = 0; letter < var_ptr->length; letter++) {
                if(var_intersect_table[var_ptr->origin_row][var_ptr->origin_col + letter].lhs == nullptr) {
                    // first variable for this tile
                    var_intersect_table[var_ptr->origin_row][var_ptr->origin_col + letter].lhs = var_ptr;
                    var_intersect_table[var_ptr->origin_row][var_ptr->origin_col + letter].lhs_index = letter;
                } else {
                    // second variable for this tile
                    var_intersect_table[var_ptr->origin_row][var_ptr->origin_col + letter].rhs = var_ptr;
                    var_intersect_table[var_ptr->origin_row][var_ptr->origin_col + letter].rhs_index = letter;
                }
            }
        } else if(var_ptr->dir == VERTICAL) { 
            for(uint letter = 0; letter < var_ptr->length; letter++) {
                if(var_intersect_table[var_ptr->origin_row + letter][var_ptr->origin_col].lhs == nullptr) {
                    // first variable for this tile
                    var_intersect_table[var_ptr->origin_row + letter][var_ptr->origin_col].lhs = var_ptr;
                    var_intersect_table[var_ptr->origin_row + letter][var_ptr->origin_col].lhs_index = letter;
                } else {
                    // second variable for this tile
                    var_intersect_table[var_ptr->origin_row + letter][var_ptr->origin_col].rhs = var_ptr;
                    var_intersect_table[var_ptr->origin_row + letter][var_ptr->origin_col].rhs_index = letter;
                }
            }
        } else {
            utils.log(ERROR, "got unknown direction type: ", var_ptr->dir);
        }
    }

    utils.log(DEBUG, "cw_csp populating constraints");

    // find the valid constraints in var_intersect_table (ones with 2 variables) to add to constraints
    for(uint row = 0; row < cw.rows(); row++) {
        for(uint col = 0; col < cw.cols(); col++) {
            if(var_intersect_table[row][col].rhs != nullptr) {
                // assert that 2 variables exist in constraint
                if(var_intersect_table[row][col].lhs == nullptr) {
                    utils.log(ERROR, "var_intersect_table has nullptr lhs with non-nullptr rhs");
                }

                shared_ptr<cw_constraint> forward_arc = make_shared<cw_constraint>(
                    var_intersect_table[row][col].lhs_index,
                    var_intersect_table[row][col].rhs_index,
                    var_intersect_table[row][col].lhs,
                    var_intersect_table[row][col].rhs
                );
                utils.log(DEBUG, "adding forward arc: ", *forward_arc);
                
                shared_ptr<cw_constraint> backwards_arc = make_shared<cw_constraint>(
                    var_intersect_table[row][col].rhs_index,
                    var_intersect_table[row][col].lhs_index,
                    var_intersect_table[row][col].rhs,
                    var_intersect_table[row][col].lhs
                );
                utils.log(DEBUG, "adding backwards arc: ", *backwards_arc);

                // add arcs
                constraints.push_back(forward_arc);
                constraints.push_back(backwards_arc);
            }
        }
    }

    utils.log(DEBUG, "cw_csp building arc_dependencies");

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
    cw_timestamper stamper(tracker, TS_CSP_AC3, "");

    utils.log(DEBUG, "starting AC-3 algorithm");

    // constraints to be checked
    queue<shared_ptr<cw_constraint> > constraint_queue;

    // for O(1) lookup of queue contents to avoid duplicate constraints in queue
    // a constraint c is in constraints_in_queue iff c also in constraint_queue
    unordered_set<shared_ptr<cw_constraint> > constraints_in_queue;

    // notify var domains of new AC-3 call
    for(shared_ptr<cw_variable> var_ptr : variables) {
        var_ptr->domain.start_new_ac3_call();
    }

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

        // prune invalid words in domain, and if domain changed, add dependent arcs to constraint queue
        if(constr->prune_domain()) {
            if(constr->lhs->domain.size() == 0) {
                // CSP is now invalid, i.e. var has empty domain
                utils.log(DEBUG, "CSP became invalid, undo-ing pruning");

                // undo pruning
                undo_ac3();
                stamper.add_result("fail");
                return false;
            }

            // add dependent arcs to queue
            for(shared_ptr<cw_constraint> constr_ptr : arc_dependencies.at(constr->lhs)) {
                if(constraints_in_queue.count(constr_ptr) == 0) {
                    constraint_queue.push(constr_ptr);
                    constraints_in_queue.insert(constr_ptr);
                }
            }
        }
    }

    // running AC-3 to completion does not make CSP invalid
    stamper.add_result("pass");
    return true;
}

/**
 * @brief undo domain pruning from previous call of AC-3 algorithm
*/
void cw_csp::undo_ac3() {
    cw_timestamper stamper(tracker, TS_CSP_UNDO_AC3, "");

    for(shared_ptr<cw_variable> var_ptr : variables) {
        var_ptr->domain.undo_prev_ac3_call();
    }
}

/**
 * @brief checks if this CSP is solved, i.e. all variables have 1 value in domain & all constraints satisfied
 * 
 * @return true iff CSP is solved
*/
bool cw_csp::solved() const {
    cw_timestamper stamper(tracker, TS_CSP_SOLVED, "");

    unordered_set<word_t> used_words;

    // check that all vars have one remaining domain value & satisfied
    for(shared_ptr<cw_variable> var_ptr : variables) {
        // check that all vars satisifed w/ one domain value
        if(var_ptr->domain.size() != 1) { stamper.add_result("no"); return false; }
        if(!var_ptr->domain.is_assigned()) { stamper.add_result("no"); return false; }

        // check that domain value is unique
        if(used_words.count(var_ptr->domain.get_cur_domain().at(0)) > 0) { stamper.add_result("no"); return false; }
        used_words.insert(var_ptr->domain.get_cur_domain().at(0));
    }

    // check that all constraints satisfied
    for(shared_ptr<cw_constraint> constr_ptr : constraints) {
        if(!constr_ptr->satisfied()) { stamper.add_result("no"); return false; }
    }

    stamper.add_result("yes");
    return true;
}

/**
 * @brief getter for string representation of solved crossword to print
*/
string cw_csp::result() const {
    assert(solved());
    stringstream cw_ss;
    cw_ss << cw;
    return cw_ss.str();
}

/**
 * @brief overwrite wildcards in cw puzzle with progress so far
 */
void cw_csp::overwrite_cw() {
    cw_timestamper stamper(tracker, TS_CSP_OVERWRITE_CW, "");

    // used to track overwritten chars for undo-ing later
    vector<tuple<char, uint, uint> > overwritten_tiles;

    // iterate across each variable to write onto cw
    for(shared_ptr<cw_variable> var_ptr : variables) {
        // only write assigned variables
        if(var_ptr->domain.is_assigned()) {
            if(var_ptr->dir == HORIZONTAL) {
                for(uint letter = 0; letter < var_ptr->length; letter++) {
                    // this square must be wildcard or the same letter about to be written
                    assert(
                        cw.read_at(var_ptr->origin_row, var_ptr->origin_col + letter) == WILDCARD ||
                        cw.read_at(var_ptr->origin_row, var_ptr->origin_col + letter) == var_ptr->domain.get_cur_domain().at(0).word.at(letter)
                    );

                    // if this will actually overwrite a wildcard
                    if(cw.read_at(var_ptr->origin_row, var_ptr->origin_col + letter) == WILDCARD) {
                        // record overwritting
                        overwritten_tiles.push_back(std::make_tuple(WILDCARD, var_ptr->origin_row, var_ptr->origin_col + letter));

                        // overwrite cw
                        cw.write_at(var_ptr->domain.get_cur_domain().at(0).word.at(letter), var_ptr->origin_row, var_ptr->origin_col + letter);
                    }
                }
            } else if(var_ptr->dir == VERTICAL) { 
                for(uint letter = 0; letter < var_ptr->length; letter++) {

                    // this square must be wildcard or the same letter about to be written
                    assert(
                        cw.read_at(var_ptr->origin_row + letter, var_ptr->origin_col) == WILDCARD ||
                        cw.read_at(var_ptr->origin_row + letter, var_ptr->origin_col) == var_ptr->domain.get_cur_domain().at(0).word.at(letter)
                    );

                    // if this will actually overwrite a wildcard
                    if(cw.read_at(var_ptr->origin_row + letter, var_ptr->origin_col) == WILDCARD) {
                        // record overwriting
                        overwritten_tiles.push_back(std::make_tuple(WILDCARD, var_ptr->origin_row + letter, var_ptr->origin_col));

                        // overwrite cw
                        cw.write_at(var_ptr->domain.get_cur_domain().at(0).word.at(letter), var_ptr->origin_row + letter, var_ptr->origin_col);
                    }
                }
            } else {
                utils.log(ERROR, "got unknown direction type: ", var_ptr->dir);
            }
        }
    }

    prev_overwritten_tiles.push(overwritten_tiles);
}

/**
 * @brief undo previous call to overwrite_cw()
*/
void cw_csp::undo_overwrite_cw() { 
    cw_timestamper stamper(tracker, TS_CSP_UNDO_OVERWRITE_CW, "");

    assert(prev_overwritten_tiles.size() > 0);

    for(const auto& tuple : prev_overwritten_tiles.top()) {
        cw.write_at(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
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
                    if(!var_ptr->domain.is_assigned() && (min_num_values == ULONG_MAX || var_ptr->domain.size() < min_num_values)) {
                        min_num_values = var_ptr->domain.size();
                        result = var_ptr;
                    }
                }
            } break;
        default: {
                utils.log(ERROR, "select_unassigned_var() got unknown var_selection_method: ", strategy);
            } break;
    }

    return result;
}

/**
 * @brief solves this CSP using the given strategy
 * 
 * @param csp_strategy strategy to solve this CSP
 * @param var_strategy strategy to select next unassigned variable
 * @return true iff successful
*/
bool cw_csp::solve(csp_solving_strategy csp_strategy, var_selection_method var_strategy) {
    cw_timestamper stamper(tracker, TS_CSP_SOLVE, "");

    // base case for initially invalid crosswords
    if(!ac3()) { stamper.add_result("fail initial ac3"); return false; }

    switch(csp_strategy) {
        case BACKTRACKING: {
                if(solve_backtracking(var_strategy, print_progress_bar, 0)) {
                    stamper.add_result("success");
                    return true;
                } else {
                    stamper.add_result("fail");
                    return false;
                }
            } break;
        default: {
                utils.log(ERROR, "solve() got unknown strategy: ", var_strategy);
                return false;
            } break;
    }

    return false;
}

/**
 * @brief use backtracking strategy to solve CSP
 * 
 * @param var_strategy strategy to use to select next unassigned variable 
 * @param do_progress_bar true for top level call in prod to avoid printing during testing
 * @param depth the depth of recursive calls, for analysis
 * @return true iff successful
*/
bool cw_csp::solve_backtracking(var_selection_method var_strategy, bool do_progress_bar, uint depth) {
    cw_timestamper stamper(tracker, TS_CSP_BACKTRACK_STEP, "depth: " + std::to_string(depth));

    utils.log(DEBUG, "entering solve_backtracking() with depth ", depth);

    // base case
    if(solved()) {
        stamper.add_result("solved");
        return true;
    }

    // select next variable
    shared_ptr<cw_variable> next_var = select_unassigned_var(var_strategy);

    utils.log(DEBUG, "selected next var: ", *next_var);

    // invalid base case: var is invalid, i.e. not assigned AND has domain of one already-assigned word
    if(!next_var->domain.is_assigned() && (next_var->domain.size() == 1 && assigned_words.count(next_var->domain.get_cur_domain().at(0)) > 0)) {
        stamper.add_result("invalid base case");
        return false;
    }
    
    // get domain of next variable as list of candidates to try to assign
    vector<word_t> domain_copy;
    {  
        cw_timestamper gather_stamper(tracker, TS_CSP_GATHER_DOMAIN, "");
        
        // search all possible values, sorted by word score, tiebroken by frequency
        domain_copy = next_var->domain.get_cur_domain();
        auto compare = [](const word_t& lhs, const word_t& rhs) {
            if(lhs.score != rhs.score) return lhs.score > rhs.score;
            if(lhs.freq != rhs.freq) return lhs.freq > rhs.freq;
            return lhs.word > rhs.word;
        };
        sort(domain_copy.begin(), domain_copy.end(), compare);
    }

    // only initialize if this is top level solve_backtracking() call in prod
    unique_ptr<progress_bar> bar = nullptr;
    if(do_progress_bar) {
        bar = make_unique<progress_bar>(utils, domain_copy.size(), 0.01, PROGRESS_BAR_WIDTH, "Searching", PROGRESS_BAR_SYMBOL_FULL, PROGRESS_BAR_SYMBOL_EMPTY);
    }

    // iterate through search space for this variable
    for(word_t word : domain_copy) {
        cw_timestamper word_stamper(tracker, TS_CSP_TRY_ASSIGN, word.word);

        // avoid duplicate words
        if(assigned_words.count(word) == 0) {
            // assignment
            next_var->domain.assign_domain(word);
            assigned_words.insert(word);

            utils.log(DEBUG, "trying new word: ", word);

            // if does not result in invalid CSP, recurse
            if(ac3()) {
                utils.log(DEBUG, "adding new word: ", word, " to var: ", *next_var);

                // add to crossword assignment
                overwrite_cw();
                if(solve_backtracking(var_strategy, false, depth + 1)) return true;
                undo_overwrite_cw();
                undo_ac3(); // AC-3 undo automatic iff ac3() returns false
                word_stamper.add_result("recursive fail");
            } else {
                word_stamper.add_result("fail ac3");
            }

            utils.log(DEBUG, "word failed: ", word);

            // undo assignment
            next_var->domain.unassign_domain();
            assigned_words.erase(word);
        } else {
            word_stamper.add_result("duplicate");
            utils.log(DEBUG, "avoided duplicate word: ", word);
        }

        // another word searched
        if(bar) bar->incr_numerator();
    }

    // after returning here or if solution, progress bar goes out of scope and finishes printing in destructor
    stamper.add_result("fail, exhausted domain");
    return false;
}