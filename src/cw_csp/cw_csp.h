// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: class declaration for crossword constraint satisfaction problem 
// ==================================================================

#ifndef CW_CSP_H
#define CW_CSP_H

#include "cw_csp_data_types.h"
#include "../common/common_parent.h"
#include "../utils/cw_utils.h"
#include "../crossword/crossword_data_types.h"
#include "../crossword/crossword.h"
#include "../cw_tracer/cw_tracer_data_types.h"
#include "../cw_tracer/cw_tracer.h"
#include "../word_domain/word_domain_data_types.h"
#include "../word_domain/word_domain.h"

using namespace cw_csp_data_types_ns;
using namespace common_parent_ns;
using namespace cw;
using namespace crossword_data_types_ns;
using namespace crossword_ns;
using namespace cw_tracer_data_types_ns;
using namespace cw_tracer_ns;
using namespace word_domain_data_types_ns;
using namespace word_domain_ns;

namespace cw_csp_ns {
    /**
     * @brief class representation of a crossword constraint satisfaction problem
    */
    class cw_csp : public common_parent {
        public:
            // base constructor, agnostic to grid layout
            cw_csp(const string& name, crossword&& grid, const string& dict_filepath, bool print_progress_bar, bool enable_tracer);

            // read-only getters for testing
            unordered_set<unique_ptr<cw_variable > >                                           get_variables()           const;
            unordered_set<unique_ptr<cw_constraint> >                                          get_constraints()         const;
            unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > get_constr_dependencies() const;

            // solve CSP
            bool solve(csp_solving_strategy csp_strategy, var_ordering var_order, val_ordering val_order);

            // execute AC-3 algorithm to reduce CSP
            bool ac3();

            // undo previous call of ac3() due to invalid CSP or backtracking
            void undo_ac3();

            // check if CSP is solved
            bool solved() const;

            // get string representation of solved cw for printing when solved() == true
            string result() const;

            // save trace results to instrumentation file
            void save_trace_result(string filepath) const;

            // returns permutations of csp, i.e. csp with a permutated crossword grid
            vector<cw_csp> permutations(unordered_set<string>& explored_grids) const;

            // copy disallowed, construct new cw_csp with same params
            cw_csp(const cw_csp& other) = delete;
            cw_csp& operator=(const cw_csp& other) = delete;

            // movable
            cw_csp(cw_csp&& other) noexcept = default;
            cw_csp& operator=(cw_csp&& other) noexcept = default;

            // default ok
            ~cw_csp() = default;
        
        protected:
            // helper func to populate variables & constraints
            void initialize_csp();

            // select next unassigned variable to explore
            size_t select_unassigned_var(var_ordering strategy);

            // use backtracking to solve CSP
            bool solve_backtracking(var_ordering var_order, val_ordering val_order, bool do_progress_bar, uint depth);

        private:
            // tracer object for analysis
            mutable cw_tracer tracker; // TODO: rename from tracker --> tracer

            // original word dictionary filepath
            string dict_filepath;

            // original tracker enabling input param
            bool enable_tracer;

            // crossword to be solved
            crossword cw;

            // domain of all valid words of all lengths, to populate variable domains
            word_domain total_domain;

            // csp structures
            id_obj_manager<cw_variable>   variables;
            id_obj_manager<cw_constraint> constraints;

            // constr_dependencies[id of var_i] contains all ids of constraints with var_i as a dependency
            // i.e. arcs of the form (var_k, var_i) and all cycles containing var_i
            // when a constraint is updated, constr_dependencies of all its dependents must be reevaluated
            unordered_map<size_t, unordered_set<size_t> > constr_dependencies;

            // words already assigned to the crossword, used to avoid duplicates
            unordered_set<word_t> assigned_words;

            // progress bar for searching and domain building
            bool print_progress_bar;

    }; // cw_csp
} // cw_csp_ns

#endif // CW_CSP_H