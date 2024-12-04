// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   9/6/2023
// Description: class declaration for main crossword generation tool
// ==================================================================

#ifndef CW_GEN_H
#define CW_GEN_H

#include "../common/common_parent.h"
#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"
#include "../crossword/crossword.h"
// #include "../cw_csp/cw_csp.h"
#include "../cw_tree/cw_tree.h"
#include "cw_gen_data_types.h"
#include "../lib/src/cxxopts.hpp"

using namespace common_parent_ns;
using namespace common_data_types_ns;
using namespace crossword_ns;
// using namespace cw_csp_ns;
using namespace cw_tree_ns;
using namespace cw_gen_data_types_ns;

namespace cw_gen_ns {

    /**
     * @brief representation of cw generation tool
    */
    class cw_gen : public common_parent {
        public:
            // base constructor, use all defaults
            cw_gen(string name);

            // these all must be set before creation of cw_csp
            void set_dimensions     (uint l, uint h ) { length = l; height = h;                 }
            void set_dict           (const string& d) { dict   = d;                             }
            void set_contents       (const string& c) { contents = std::make_optional(c);       }
            void set_num_solutions  (size_t n)        { num_solutions = n;                      }
            void enable_progress_bar()                { display_progress_bar = true;            }
            void enable_profile     (const string& f) { profile_header = std::make_optional(f); }

            // for checking legal length of contents
            uint num_tiles() { return length * height; }

            // util function for squashing param options into a single string
            static string squash_options(const vector<string>& options);

            // build crossword constraint satisfaction problem
            // void build();

            // attempt to solve with given parameters
            vector<string> solve();

            // // attempt to solve crossword puzzle, return true iff successful
            // bool solve() { assert(csp != nullptr); return csp->solve(BACKTRACKING, MIN_REMAINING_VALUES) && csp->solved(); }

            // // return result after running solve()
            // string result() { assert(csp->solved()); return csp->result(); }

            // generate profile file, this object cannot be meaningfully used thereafter
            // void save_profile(string filepath) { csp->save_timetracker_result(filepath); }

        private:
            // tree of CSPs to solve
            unique_ptr<cw_tree> tree;
            
            // cw puzzle dimensions
            uint length = 0;
            uint height = 0;

            // dictionary name
            string dict;

            // contents to populate crossword grid with
            optional<string> contents;

            // number of grids to find solutions for
            size_t num_solutions;

            // whether to display progress bar or not
            bool display_progress_bar = false;

            // cw_timetracker enabled iff this has a value
            // does not include .json file extension
            optional<string> profile_header;
    }; // cw_gen
} // cw_gen_ns

#endif // CW_GEN_H