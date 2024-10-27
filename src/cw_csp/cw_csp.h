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
#include "../cw_timetracker/cw_timetracker_data_types.h"
#include "../cw_timetracker/cw_timetracker.h"
#include "../word_domain/word_domain_data_types.h"
#include "../word_domain/word_domain.h"

using namespace cw_csp_data_types_ns;
using namespace common_parent_ns;
using namespace crossword_data_types_ns;
using namespace crossword_ns;
using namespace cw_timetracker_data_types_ns;
using namespace cw_timetracker_ns;
using namespace word_domain_data_types_ns;
using namespace word_domain_ns;

namespace cw_csp_ns {
    /**
     * @brief class representation of a crossword constraint satisfaction problem
    */
    class cw_csp : public common_parent {
        public:
            // constructor w/o puzzle contents
            cw_csp(string name, uint length, uint height, string filepath, bool print_progress_bar, bool use_timetracker);

            // constructor with puzzle contents
            cw_csp(string name, uint length, uint height, string contents, string filepath, bool print_progress_bar, bool use_timetracker);

            // read-only getters for testing
            unordered_set<unique_ptr<cw_variable > >                                           get_variables()        const;
            unordered_set<unique_ptr<cw_constraint> >                                          get_constraints()      const;
            unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > get_arc_dependencies() const;

            // solve CSP
            bool solve(csp_solving_strategy csp_strategy, var_selection_method var_strategy);

            // execute AC-3 algorithm to reduce CSP
            bool ac3();

            // check if CSP is solved
            bool solved() const;

            // get string representation of solved cw for printing when solved() == true
            string result() const;

            // save timetracker results for analysis
            void save_timetracker_result(string filepath) const { tracker.save_results(filepath); }
        
        protected:
            // helper func to populate variables & constraints
            void initialize_csp();

            // select next unassigned variable to explore
            size_t select_unassigned_var(var_selection_method strategy);

            // undo previous call of ac3() due to invalid CSP or backtracking
            void undo_ac3();

            // function to overwrite cw with progress
            void overwrite_cw();

            // function to undo previous call to overwrite_cw();
            void undo_overwrite_cw();

            // use backtracking to solve CSP
            bool solve_backtracking(var_selection_method var_strategy, bool do_progress_bar, uint depth);

        private:
            // timetracker object for analysis
            mutable cw_timetracker tracker;

            // crossword to be solved
            crossword cw;

            // domain of all valid words of all lengths, to populate variable domains
            word_domain total_domain;

            // csp structures
            id_obj_manager<cw_variable>   variables;
            id_obj_manager<cw_constraint> constraints;

            // arc_dependencies[id of var_i] contains all ids of arcs of the form (var_k, var_i) 
            unordered_map<size_t, unordered_set<size_t> > arc_dependencies;

            // previous cw tile values overwritten during call to overwrite_cw(), used for undo_overwrite_cw()
            stack<vector<tuple<char, uint, uint> > > prev_overwritten_tiles; 

            // words already assigned to the crossword, used to avoid duplicates
            unordered_set<word_t> assigned_words;

            // progress bar for searching and domain building
            bool print_progress_bar;

    }; // cw_csp
} // cw_csp_ns

#endif // CW_CSP_H