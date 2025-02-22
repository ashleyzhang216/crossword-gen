// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class implementation for crossword constraint satisfaction problem 
// ==================================================================

#include "cw_csp.h"

using namespace cw_csp_ns;

/**
 * @brief constructor for constraint satisfaction problem with or without puzzle contents
 * 
 * @param name name of this object
 * @param grid rvalue crossword grid to solve and assume ownership of, can have or not have contents
 * @param dict_filepath relative filepath to dictionary of words file
 * @param print_progress_bar displays progress bar iff true
 * @param use_timetracker enables cw_timetracker iff true
*/
cw_csp::cw_csp(const string& name, crossword&& grid, const string& dict_filepath, bool print_progress_bar, bool use_timetracker) 
        : common_parent(name, VERBOSITY),
          tracker("cw_csp", use_timetracker),
          dict_filepath(dict_filepath),
          use_timetracker(use_timetracker),
          cw(std::move(grid)),
          total_domain(name + " total_domain", dict_filepath, print_progress_bar),
          print_progress_bar(print_progress_bar) {
    initialize_csp();
}

/**
 * @brief getter function for variables by value
 * 
 * @return copy of variables
*/
unordered_set<unique_ptr<cw_variable> > cw_csp::get_variables() const {
    unordered_set<unique_ptr<cw_variable> > result;
    for(const unique_ptr<cw_variable>& var : variables) {
        result.insert(var->clone());
    }
    return result;
}

/**
 * @brief getter function for constraints by value
 * 
 * @return copy of constraints
*/
unordered_set<unique_ptr<cw_constraint> > cw_csp::get_constraints() const {
    unordered_set<unique_ptr<cw_constraint> > result;
    for(const unique_ptr<cw_constraint>& constr : constraints) {
        result.insert(constr->clone());
    }
    return result;
}

/**
 * @brief getter function for constr_depenencies by value
 * 
 * @return copy of all objects that constr_depenencies items index to
*/
unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > cw_csp::get_constr_dependencies() const {
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > result;
    for(const auto& pair : constr_dependencies) {
        for(const size_t constr : pair.second) {
            result[variables[pair.first]->clone()].insert(constraints[constr]->clone());
        }
    }
    return result;
}

/**
 * @brief save profiling result from cw_timetracker
 *
 * @param filepath the filepath to save the result to, as a json file
 */
void cw_csp::save_timetracker_result(string filepath) const {
    tracker.save_results(filepath, basic_json::object({
        {"success", solved()},
        {"result", solved() ? result() : ""},
        {"grid", basic_json::object({
            {"rows", cw.rows()},
            {"cols", cw.cols()},
            {"dict", dict_filepath},
            {"contents", cw.init_contents()}
        })}
    }));
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
    uint cur_var_row = 0ul, cur_var_col = 0ul, cur_var_len = 0ul; // valid iff traversing_word

    // track frequency of each length of variable
    unordered_map<size_t, size_t> var_len_freqs;

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
                        variables.push_back(make_unique<cw_variable>(
                            variables.size(), cur_var_row, cur_var_col, cur_var_len, DOWN, word_pattern.str(), total_domain.find_matches(word_pattern.str())
                        ));

                        // record variable length
                        ++var_len_freqs[cur_var_len];
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
            // applicable if the last MIN_WORD_LEN+ spaces in a row are blank
            variables.push_back(make_unique<cw_variable>(
                variables.size(), cur_var_row, cur_var_col, cur_var_len, DOWN, word_pattern.str(), total_domain.find_matches(word_pattern.str())
            ));

            // record variable length
            ++var_len_freqs[cur_var_len];
        }
    }

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
                        variables.push_back(make_unique<cw_variable>(
                            variables.size(), cur_var_row, cur_var_col, cur_var_len, ACROSS, word_pattern.str(), total_domain.find_matches(word_pattern.str())
                        ));

                        // record variable length
                        ++var_len_freqs[cur_var_len];
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
            // applicable if the last MIN_WORD_LEN+ spaces in a row are blank
            variables.push_back(make_unique<cw_variable>(
                variables.size(), cur_var_row, cur_var_col, cur_var_len, ACROSS, word_pattern.str(), total_domain.find_matches(word_pattern.str())
            ));

            // record variable length
            ++var_len_freqs[cur_var_len];
        }
    }

    // helper table to build valid constraints
    // var_intersect_table[i][j] corresponds to cw[i][j] 
    vector<vector<cw_arc> > var_intersect_table(cw.rows(), vector<cw_arc>(cw.cols()));

    utils.log(DEBUG, "cw_csp building var_intersect_table");

    // iterate across each variable to populate var_intersect_table
    // lhs for horizontal variables, rhs for vertical
    for(size_t i = 0; i < variables.size(); ++i) {
        if(variables[i]->dir == ACROSS) {
            for(uint letter = 0; letter < variables[i]->length; ++letter) {
                cw_assert(var_intersect_table[variables[i]->origin_row][variables[i]->origin_col + letter].lhs == id_obj_manager<cw_variable>::INVALID_ID);
                var_intersect_table[variables[i]->origin_row][variables[i]->origin_col + letter].lhs = variables[i]->id;
                var_intersect_table[variables[i]->origin_row][variables[i]->origin_col + letter].lhs_index = letter;
            }
        } else if(variables[i]->dir == DOWN) { 
            for(uint letter = 0; letter < variables[i]->length; ++letter) {
                cw_assert(var_intersect_table[variables[i]->origin_row + letter][variables[i]->origin_col].rhs == id_obj_manager<cw_variable>::INVALID_ID);
                var_intersect_table[variables[i]->origin_row + letter][variables[i]->origin_col].rhs = variables[i]->id;
                var_intersect_table[variables[i]->origin_row + letter][variables[i]->origin_col].rhs_index = letter;
            }
        } else {
            utils.log(ERROR, "got unknown direction type: ", variables[i]->dir);
        }
    }

    utils.log(DEBUG, "cw_csp populating constraints");

    // track frequency of each length of constraint
    unordered_map<size_t, size_t> constr_len_freqs;

    // find the valid constraints in var_intersect_table (ones with 2 variables) to add to constraints
    for(uint row = 0; row < cw.rows(); row++) {
        for(uint col = 0; col < cw.cols(); col++) {
            // two variables intersect in this tile
            if(
                var_intersect_table[row][col].lhs != id_obj_manager<cw_variable>::INVALID_ID && 
                var_intersect_table[row][col].rhs != id_obj_manager<cw_variable>::INVALID_ID
            ) {
                // add arcs
                constraints.push_back(make_unique<cw_arc>(
                    constraints.size(),
                    var_intersect_table[row][col].lhs_index,
                    var_intersect_table[row][col].rhs_index,
                    var_intersect_table[row][col].lhs,
                    var_intersect_table[row][col].rhs
                ));
                constraints.push_back(make_unique<cw_arc>(
                    constraints.size(),
                    var_intersect_table[row][col].rhs_index,
                    var_intersect_table[row][col].lhs_index,
                    var_intersect_table[row][col].rhs,
                    var_intersect_table[row][col].lhs
                ));

                // record both arcs just created
                constr_len_freqs[2ul] += 2;
            }
        }
    }

    utils.log(DEBUG, "cw_csp building length 2 constr_dependencies");

    // build arc table to list out all dependencies for easy arc queueing in AC-3
    for(size_t i = 0; i < constraints.size(); ++i) {
        for(size_t var : constraints[i]->dependencies()) {
            constr_dependencies[var].insert(i);
        }
    }

    utils.log(DEBUG, "cw_csp searching for cycle constraints");

    /**
     * @brief find cycle constraints
     * @pre prev_arcs is nonempty, and size < MAX_CYCLE_LEN
     * @pre visited_vars is nonempty, size >= 2 (vars of first arc) and size <= MAX_CYCLE_LEN
     * 
     * @param prev_arcs vector of previous arcs in current cycle prototype
     * @param visited_vars vector of previously visited vars in current cycle prototype
    */
    unordered_set<rot_vector<size_t> > unique_cycles;
    std::function<void(vector<size_t>&, vector<size_t>&)> find_cycles;
    find_cycles = [this, &find_cycles, &unique_cycles, &constr_len_freqs](vector<size_t>& prev_arcs, vector<size_t>& visited_vars) {
        cw_assert(prev_arcs.size());
        cw_assert(prev_arcs.size() < cw_cycle::MAX_CYCLE_LEN);
        cw_assert(visited_vars.size());
        cw_assert(visited_vars.size() <= cw_cycle::MAX_CYCLE_LEN);
        cw_assert(prev_arcs.size() + 1 == visited_vars.size());

        // this casting is dirty but guaranteed to work since constraints must only contain cw_arc
        const cw_constraint * const cur_constr = constraints[prev_arcs[prev_arcs.size() - 1]].get();
        const cw_arc * const cur_arc = dynamic_cast<const cw_arc* const>(cur_constr);
        cw_assert(cur_arc);
        const size_t cur_var = cur_arc->lhs;
        
        for(const size_t dep : constr_dependencies.at(cur_var)) {
            // this messy casting also guaranteed to work for same reason as above
            const cw_constraint * const next_constr = constraints[dep].get();
            const cw_arc * const next_arc = dynamic_cast<const cw_arc* const>(next_constr);
            cw_assert(next_arc);
            const size_t next_var = next_arc->lhs;

            // if next var unvisited so far, excluding the very first var so cycles can form
            if(std::find(visited_vars.begin() + 1, visited_vars.end(), next_var) == visited_vars.end()) {
                // visit this arc. may not need to visit the next var if it completes a cycle
                prev_arcs.push_back(dep);

                // check if formed a valid simple cycle
                // second condition needed to avoid cycle of (var1 -> var2 -> ...)
                if(next_var == visited_vars.at(0) && visited_vars.size() >= cw_cycle::MIN_CYCLE_LEN) {
                    // started with 1 more var visited, first var isn't added again for cycles, so size should be equal
                    cw_assert(prev_arcs.size() == visited_vars.size());
                    cw_assert(visited_vars.size() <= cw_cycle::MAX_CYCLE_LEN);
                    
                    rot_vector<size_t> visited_cycle{visited_vars};
                    if(!unique_cycles.count(visited_cycle)) {
                        unique_cycles.insert(std::move(visited_cycle));
                        constraints.push_back(make_unique<cw_cycle>(
                            constraints.size(), constraints, prev_arcs
                        ));

                        // record one constraint of this length
                        ++constr_len_freqs[prev_arcs.size()];
                    }

                    // do not continue recursive search, since it wouldn't be a simple cycle anymore

                } else if(visited_vars.size() < cw_cycle::MAX_CYCLE_LEN) {
                    // visit this next var, now that we know it doesn't complete a cycle
                    visited_vars.push_back(next_var);

                    // disallow search to continue if too long
                    find_cycles(prev_arcs, visited_vars);

                    // backtrack from this var
                    cw_assert(visited_vars.back() == next_var);
                    visited_vars.pop_back();
                }

                // backtrack from this arc
                cw_assert(prev_arcs.back() == dep);
                prev_arcs.pop_back();
            }
        }
    };

    // to avoid double-adding dependencies of arcs
    size_t num_arcs = constraints.size();

    // add cycles starting with each variable
    for(const size_t id : variables.ids()) {
        // filter out edge case for variables that don't intersect with any others
        if(constr_dependencies.count(id)) {
            for(const size_t dep : constr_dependencies.at(id)) {
                const cw_constraint * const constr = constraints[dep].get();
                const cw_arc * const arc = dynamic_cast<const cw_arc* const>(constr);
                cw_assert(arc);

                vector<size_t> prev_arcs = {dep};
                vector<size_t> visited_vars = {arc->rhs, arc->lhs};
                find_cycles(prev_arcs, visited_vars);
            }
        }
    }

    utils.log(DEBUG, "cw_csp building cycle constr_dependencies");

    // add dependencies of newly created arc constraints
    for(size_t i = num_arcs; i < constraints.size(); ++i) {
        for(size_t var : constraints[i]->dependencies()) {
            constr_dependencies[var].insert(i);
        }
    }

    // record variable and constraint length histogram
    stamper.result()["var_len_freqs"]    = std::move(var_len_freqs);
    stamper.result()["constr_len_freqs"] = std::move(constr_len_freqs);
}

/**
 * @brief AC-3 algorithm to reduce CSP; calls undo_ac3() iff running AC-3 to completion would result in an invalid CSP automatically
 * 
 * @return true iff resulting CSP is valid, i.e. all resulting variables have a non-empty domain
*/
bool cw_csp::ac3() {
    #ifdef TIMETRACKER_TRACK_AC3
    cw_timestamper stamper(tracker, TS_CSP_AC3, "");
    #endif // TIMETRACKER_TRACK_AC3

    utils.log(DEBUG, "starting AC-3 algorithm");

    // constraint ids to be checked
    queue<size_t> constraint_queue;

    // for O(1) lookup of queue contents to avoid duplicate constraints in queue
    // a constraint c's id is in constraints_in_queue iff c's id also in constraint_queue
    unordered_set<size_t> constraints_in_queue;

    // notify var domains of new AC-3 call
    for(unique_ptr<cw_variable>& var : variables) {
        var->domain.start_new_ac3_call();
    }

    // initialize constraint_queue
    for(size_t constr_id : constraints.ids()) {
        constraint_queue.push(constr_id);
        constraints_in_queue.insert(constr_id);
    }

    // run AC-3 algo
    size_t constr_id;
    while(!constraint_queue.empty()) {
        // pop top constraint
        constr_id = constraint_queue.front();
        constraint_queue.pop();
        cw_assert(constraints_in_queue.count(constr_id) > 0);
        constraints_in_queue.erase(constr_id);

        // prune invalid words in domain, and if domain changed, add dependent constraints to constraint queue
        unordered_set<size_t> modified = constraints[constr_id]->prune_domain(variables);
        if(modified.size() > 0) {
            if(constraints[constr_id]->invalid(variables)) {
                // CSP is now invalid, i.e. var has empty domain
                utils.log(DEBUG, "CSP became invalid, undo-ing pruning");

                // report violating tiles that caused this to become invalid to the crossword grid
                cw.report_invalidating_tiles(constraints[constr_id]->intersection_tiles(variables));

                // undo pruning
                undo_ac3();
                #ifdef TIMETRACKER_TRACK_AC3
                stamper.result()["success"] = false;
                #endif // TIMETRACKER_TRACK_AC3
                return false;
            }

            // add dependent constraints to queue
            for(size_t dep : modified) {
                for(size_t dep_constr : constr_dependencies.at(dep)) {
                    if(constraints_in_queue.count(dep_constr) == 0) {
                        constraint_queue.push(dep_constr);
                        constraints_in_queue.insert(dep_constr);
                    }
                }
            }
        }
    }

    // running AC-3 to completion does not make CSP invalid
    #ifdef TIMETRACKER_TRACK_AC3
    stamper.result()["success"] = true;
    #endif // TIMETRACKER_TRACK_AC3
    return true;
}

/**
 * @brief undo domain pruning from previous call of AC-3 algorithm
*/
void cw_csp::undo_ac3() {
    #ifdef TIMETRACKER_TRACK_AC3
    cw_timestamper stamper(tracker, TS_CSP_UNDO_AC3, "");
    #endif // TIMETRACKER_TRACK_AC3

    for(unique_ptr<cw_variable>& var : variables) {
        var->domain.undo_prev_ac3_call();
    }
}

/**
 * @brief checks if this CSP is solved, i.e. all variables have 1 value in domain & all constraints satisfied
 * 
 * @return true iff CSP is solved
*/
bool cw_csp::solved() const {
    unordered_set<word_t> used_words;

    // check that all vars have one remaining domain value & satisfied
    for(const unique_ptr<cw_variable>& var : variables) {
        // check that all vars satisifed w/ one domain value
        if(var->domain.size() != 1 || !var->domain.is_assigned()) {
            return false;
        }

        // check that domain value is unique
        word_t domain_val = var->domain.get_cur_domain().at(0);
        if(used_words.count(domain_val) > 0) {
            return false;
        }
        used_words.insert(domain_val);
    }

    // check that all constraints satisfied
    for(const unique_ptr<cw_constraint>& constr : constraints) {
        if(!constr->satisfied(variables)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief getter for string representation of solved crossword to print
*/
string cw_csp::result() const {
    cw_assert(solved());
    return cw.serialize_result();
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
size_t cw_csp::select_unassigned_var(var_selection_method strategy) {
    size_t result = id_obj_manager<cw_variable>::INVALID_ID;

    switch(strategy) {
        case MIN_REMAINING_VALUES: {
                size_t min_num_values = UINT_MAX;
                for(unique_ptr<cw_variable>& var : variables) {
                    // if this variable is unassigned AND has fewer remaining values in domain
                    if(!var->domain.is_assigned() && var->domain.size() < min_num_values) {
                        min_num_values = var->domain.size();
                        result = var->id;
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
    if(!ac3()) {
        stamper.result()["success"] = false;
        stamper.result()["reason"]  = "ac3";
        return false;
    }

    switch(csp_strategy) {
        case BACKTRACKING: {
                if(solve_backtracking(var_strategy, print_progress_bar, 0)) {
                    stamper.result()["success"] = true;
                    stamper.result()["reason"]  = "recursive";
                    return true;
                } else {
                    stamper.result()["success"] = false;
                    stamper.result()["reason"]  = "recursive";
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
        stamper.result()["success"]  = true;
        stamper.result()["reason"]   = "solved";
        stamper.result()["variable"] = nullptr;
        return true;
    }

    // select next variable
    size_t next_var = select_unassigned_var(var_strategy);
    cw_assert(next_var != id_obj_manager<cw_variable>::INVALID_ID);
    stamper.result()["variable"] = basic_json::object({
        {"origin_row", variables[next_var]->origin_row},
        {"origin_col", variables[next_var]->origin_col},
        {"direction",  word_dir_name.at(variables[next_var]->dir)},
        {"length",     variables[next_var]->length}
    });

    utils.log(DEBUG, "selected next var: ", *variables[next_var]);
    
    // get domain of next variable as list of candidates to try to assign
    vector<word_t> domain_copy{variables[next_var]->domain.get_cur_domain()};

    // sort candidates by word score, tiebroken by frequency
    auto compare = [](const word_t& lhs, const word_t& rhs) {
        if(lhs.score != rhs.score) return lhs.score > rhs.score;
        if(lhs.freq != rhs.freq) return lhs.freq > rhs.freq;
        return lhs.word > rhs.word;
    };
    sort(domain_copy.begin(), domain_copy.end(), compare);

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
            variables[next_var]->domain.assign_domain(word);
            assigned_words.insert(word);

            utils.log(DEBUG, "trying new word: ", word);

            // if does not result in invalid CSP, recurse
            if(ac3()) {
                utils.log(DEBUG, "adding new word: ", word, " to var: ", *variables[next_var]);

                // add to crossword assignment
                cw.write({
                    .origin_row = variables[next_var]->origin_row,
                    .origin_col = variables[next_var]->origin_col,
                    .word       = word.word,
                    .dir        = variables[next_var]->dir
                });

                // will be overwritten if fails recurisively
                word_stamper.result()["success"] = true;
                word_stamper.result()["reason"]  = "recursive";

                // recurse
                if(solve_backtracking(var_strategy, false, depth + 1)) {
                    stamper.result()["success"] = true;
                    stamper.result()["reason"]  = "recursive";
                    return true;
                }
                
                // undo adding to crossword asignment
                cw_assert(cw.undo_prev_write() == word.word);

                undo_ac3(); // AC-3 undo automatic iff ac3() returns false
                word_stamper.result()["success"] = false;
                word_stamper.result()["reason"]  = "recursive";
            } else {
                word_stamper.result()["success"] = false;
                word_stamper.result()["reason"]  = "ac3";
            }

            utils.log(DEBUG, "word failed: ", word);

            // undo assignment
            variables[next_var]->domain.unassign_domain();
            assigned_words.erase(word);
        } else {
            word_stamper.result()["success"] = false;
            word_stamper.result()["reason"]  = "duplicate";
            utils.log(DEBUG, "avoided duplicate word: ", word);
        }

        // another word searched
        if(bar) bar->incr_numerator();
    }

    // after returning here or if solution, progress bar goes out of scope and finishes printing in destructor
    stamper.result()["success"] = false;
    stamper.result()["reason"]  = "domain exhausted";
    return false;
}

/**
 * @brief get all permutations of this csp with a permutated crossword grid
 *
 * @param explored_grids ref to set of grids already being explored, to avoid duplicates
*/
vector<cw_csp> cw_csp::permutations(unordered_set<string>& explored_grids) const {
    vector<crossword> p = cw.permutations(explored_grids);
    vector<cw_csp> res;

    for(size_t i = 0; i < p.size(); ++i) {
        res.emplace_back(cw_csp(name + '-' + std::to_string(i), std::move(p[i]), dict_filepath, print_progress_bar, use_timetracker));
    }

    return res;
}
