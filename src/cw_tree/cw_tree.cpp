// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/3/2024
// Description: class implementation for tree of CSP permutations
// ==================================================================

#include "cw_tree.h"

using namespace cw_tree_ns;

/**
 * @brief base constructor for cw_tree 
 *
 * @param name name for this tree
 * @param grid rvalue user input crossword grid to solve, assume ownership of, and generate csp permutations to solve with
 * @param filepath relative filepath to dictionary of words file
 * @param print_progress_bar csp displays progress bar iff true
 * @param profile_header csp enables cw_timetracker iff has a value
 */
cw_tree::cw_tree(const string& name, crossword&& grid, const string& filepath, bool print_progress_bar, const optional<string>& profile_header)
    : common_parent(name, VERBOSITY),
      init_grid(std::move(grid)),
      filepath(filepath),
      print_progress_bar(print_progress_bar),
      profile_header(profile_header) {
    // do nothing else
}

/**
 * @brief search for solutions with a single thread. cw_tree will continue to search until num_solutions are found or search space exhausted
 * 
 * @param num_solutions maximum number of solutions to return
 */
vector<string> cw_tree::solve(size_t num_solutions) {
    assert(num_solutions > 0);
    vector<string> result;
    unordered_set<string> explored_grids;
    size_t num_explored = 0ul;

    // init current layer with depth=0, i.e. csp on initial grid
    vector<cw_csp> cur_layer;
    size_t cur_idx = 0ul;
    cur_layer.push_back(cw_csp(name + " cw_csp", std::move(init_grid), filepath, print_progress_bar, profile_header.has_value()));

    // find solutions until no more needed or no more children nodes to explore
    // this prioritizes permutations with fewer additional black tiles
    while(num_solutions > 0 && cur_layer.size()) {
        if(cur_idx < cur_layer.size()) {
            // still have more csps to process in current layer
            if(cur_layer.at(cur_idx).solve(BACKTRACKING, MIN_REMAINING_VALUES)) {
                assert(cur_layer.at(cur_idx).solved());
                result.push_back(cur_layer.at(cur_idx).result());
                --num_solutions;
            }

            // save profile result
            if(profile_header.has_value()) {
                cur_layer.at(cur_idx).save_timetracker_result(profile_header.value() + std::to_string(num_explored++) + ".json");
            }

            ++cur_idx;
        } else {
            // current layer is exhausted, build out next layer and discard current one
            vector<cw_csp> next_layer;
            for(const cw_csp& csp : cur_layer) {
                vector<cw_csp> next = csp.permutations(explored_grids);
                next_layer.insert(next_layer.end(), make_move_iterator(next.begin()), make_move_iterator(next.end()));
            }
            std::swap(cur_layer, next_layer);

            cur_idx = 0ul;
        }
    }

    return result;
}