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
#include "../cw_csp/cw_csp.h"
#include "../word_finder/word_finder.h"
#include "cw_gen_data_types.h"
#include "cxxopts.hpp"

using namespace common_parent_ns;
using namespace common_data_types_ns;
using namespace crossword_ns;
using namespace cw_csp_ns;
using namespace word_finder_ns;
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
            void set_dimensions(uint length, uint height) { this->length = length; this->height = height; }
            void set_dict(string dict) { this->dict = dict; }
            void set_contents(string contents) { this->contents = contents; has_grid_contents = true; }

            // for checking legal length of contents
            uint num_tiles() { return this->length * this->height; }

            // util function for squashing param options into a single string
            static string squash_options(vector<string> options);

            // build crossword constraint satisfaction problem
            void build();

            // attempt to solve crossword puzzle, return true iff successful
            bool solve() { assert(csp != nullptr); return csp->solve(BACKTRACKING, MIN_REMAINING_VALUES) && csp->solved(); }

            // return result after running solve()
            string result() { assert(csp->solved()); return csp->result(); }

        private:
            // crossword constraint satisfaction problem to solve
            unique_ptr<cw_csp> csp = nullptr;
            
            // method to populate contents field when instantiating cw_csp
            bool has_grid_contents = false;

            // cw puzzle dimensions
            uint length = 0;
            uint height = 0;

            // dictionary name
            string dict;

            // contents to populate crossword grid with
            string contents;
    }; // cw_gen
} // cw_gen_ns

#endif // CW_GEN_H