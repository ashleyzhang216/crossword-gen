// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/3/2024
// Description: class implementation for tree of CSP permutations
// ==================================================================

#include "cw_tree.h"

using namespace cw_tree_ns;

/**
 * @brief base constructor for cw_tree 
 */
cw_tree::cw_tree(const string& name, size_t num_solutions, crossword&& grid, const string& filepath, bool print_progress_bar, bool use_timetracker)
    : common_parent(name, VERBOSITY) {
    (void)num_solutions;
    (void)grid;
    (void)filepath;
    (void)print_progress_bar;
    (void)use_timetracker;
}
