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

            // read-only getters for testing
            shared_ptr   <word_finder>                                get_finder()           const { return finder; }
            unordered_set<cw_variable>                                get_variables()        const;
            unordered_set<cw_constraint>                              get_constraints()      const;
            unordered_map<cw_variable, unordered_set<cw_constraint> > get_arc_dependencies() const;

            // execute AC-3 algorithm to reduce CSP
            bool ac3();

            // solve CSP
            bool solve(csp_solving_strategy csp_strategy, var_selection_method var_strategy);

            // check if CSP is solved
            bool solved() const;

            string result() const;
        
        protected:
            // helper func to populate variables & constraints
            void initialize_csp();

            // select next unassigned variable to explore
            shared_ptr<cw_variable> select_unassigned_var(var_selection_method strategy);

            // function to overwrite cw when fully solved
            void overwrite_cw();

            // use backtracking to solve CSP
            bool solve_backtracking(var_selection_method var_strategy);

        private:
            // crossword to be solved
            unique_ptr<crossword> cw = nullptr;

            // word_finder to populate domains
            shared_ptr<word_finder> finder = nullptr;

            // csp structures
            unordered_set<shared_ptr<cw_variable> >   variables;
            unordered_set<shared_ptr<cw_constraint> > constraints;

            // arc_dependencies[var_i] contains ptrs to all arcs of the form (var_k, var_i) 
            unordered_map<shared_ptr<cw_variable>, unordered_set<shared_ptr<cw_constraint> > > arc_dependencies;

            // words already assigned to the crossword, used to avoid duplicates
            unordered_set<string> assigned_words;

    }; // cw_csp
} // cw_csp_ns

#endif // CW_CSP_H