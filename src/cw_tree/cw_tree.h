// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/3/2024
// Description: class declaration for tree of CSP permutations
// ==================================================================

#ifndef CW_TREE_H
#define CW_TREE_H

#include "cw_tree_data_types.h"
#include "../common/common_parent.h"
#include "../crossword/crossword.h"

using namespace cw_tree_data_types_ns;
using namespace common_parent_ns;
using namespace crossword_ns;

namespace cw_tree_ns {
    /**
     * @brief class representation of owner of a tree of cw_csp_nodes
     */
    class cw_tree : public common_parent {
        public:
            // base constructor, agnostic to grid layout
            cw_tree(const string& name, size_t num_solutions, crossword&& grid, const string& filepath, bool print_progress_bar, bool use_timetracker);
            
        protected:
            // TODO: implement

    }; // cw_tree
} // cw_tree_ns

#endif // CW_TREE_H