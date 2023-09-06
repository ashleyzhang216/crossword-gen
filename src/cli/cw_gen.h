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
#include "cxxopts.hpp"

using namespace common_parent_ns;
using namespace common_data_types_ns;
using namespace crossword_ns;
using namespace cw_csp_ns;
using namespace word_finder_ns;

namespace cw_gen_ns {
    /**
     * @brief representation of cw generation tool
    */
    class cw_gen : public common_parent {
        public:
            // base constructor, use all defaults
            cw_gen(string name);

        private:
            // TODO: implement
    }; // cw_gen
} // cw_gen_ns

#endif // CW_GEN_H