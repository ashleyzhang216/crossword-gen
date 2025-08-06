// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/3/2024
// Description: class declaration for tree of CSP permutations
// ==================================================================

#ifndef CW_TREE_H
#define CW_TREE_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"
#include "../common/common_parent.h"
#include "../cw_csp/cw_csp.h"

using namespace common_data_types_ns;
using namespace cw;
using namespace common_parent_ns;
using namespace cw_csp_ns;

namespace cw_tree_ns {
    /**
     * @brief class representation of owner of a tree of cw_csp_nodes
     */
    class cw_tree : public common_parent {
        public:
            // base constructor, agnostic to grid layout
            cw_tree(const string& name, crossword&& grid, const std::filesystem::path& filepath, bool print_progress_bar, const optional<string>& trace_header);
            
            // singlethreaded search for 1 solution per permutated grid, returning up to num_solutions results
            vector<string> solve(size_t num_solutions, bool allow_permutations);
        
        protected:
            // initial user input grid, undefined once solve() is called
            crossword init_grid;

            // original word dictionary filepath
            std::filesystem::path filepath;

            // original progress bar enabling for searching and domain building
            bool print_progress_bar;

            // enable tracer iff this has a value
            // does not include .json file extension
            optional<string> trace_header;
    }; // cw_tree
} // cw_tree_ns

#endif // CW_TREE_H