// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class declaration for crossword constraint satisfaction problem 
// ==================================================================

#ifndef CW_CSP_H
#define CW_CSP_H

#include "cw_csp_data_types.h"
#include "../common/common_parent.h"
#include "../crossword/crossword_data_types.h"
#include "../crossword/crossword.h"
#include "../word_finder/word_finder_data_types.h"
#include "../word_finder/word_finder.h"

using namespace cw_csp_data_types_ns;
using namespace common_parent_ns;
using namespace crossword_data_types_ns;
using namespace crossword_ns;
using namespace word_finder_data_types_ns;
using namespace word_finder_ns;

namespace cw_csp_ns {
    /**
     * @brief class representation of a crossword constraint satisfaction problem
    */
    class cw_csp : public common_parent {
        public:
            // constructor w/o puzzle contents
            cw_csp(string name, uint length, uint height, string filepath);

            // constructor with puzzle contents
            cw_csp(string name, uint length, uint height, string contents, string filepath);
        
        protected:
            // helper func to populate variables & constraints
            void initialize_csp();

        private:
            // crossword to be solved
            unique_ptr<crossword> cw = nullptr;

            // word_finder to populate domains
            shared_ptr<word_finder> finder = nullptr;

            // csp structures
            unordered_set<cw_variable>     variables;
            unordered_set<cw_constraint>   constraints;

            // ptrs to csp structures
            unordered_map<cw_variable,   shared_ptr<cw_variable> >   variable_ptrs;
            unordered_map<cw_constraint, shared_ptr<cw_constraint> > constraint_ptrs;

            // arc_dependencies[var_i] contains ptrs to all vars var_k in arcs of the form (var_k, var_i) 
            unordered_map<shared_ptr<cw_variable>, unordered_set<shared_ptr<cw_variable> > > arc_dependencies;

    }; // cw_csp
} // cw_csp_ns

#endif // CW_CSP_H