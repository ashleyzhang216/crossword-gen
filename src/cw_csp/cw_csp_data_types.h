// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: data types for constraint satisfaction problem class
// ==================================================================

#ifndef CW_CSP_DATA_TYPES_H
#define CW_CSP_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../word_domain/word_domain_data_types.h"
#include "../word_domain/word_domain.h"

using namespace common_data_types_ns;
using namespace word_domain_data_types_ns;
using namespace word_domain_ns;

namespace cw_csp_data_types_ns {

    // direction a word can be placed in a crossword
    enum word_direction {
        HORIZONTAL = 0,
        VERTICAL   = 1,
    };

    // for more interpretable prints
    const unordered_map<word_direction, string> word_dir_name = {
        {HORIZONTAL, "HORIZONTAL"},
        {VERTICAL, "VERTICAL"}
    };

    // to choose between solving strategies, when more are added in the future
    enum csp_solving_strategy {
        BACKTRACKING = 0,
    };

    // to choose between variable selection method, when more are added in the future
    enum var_selection_method {
        MIN_REMAINING_VALUES = 0,
    };

    // a variable in a constraint satisfaction problem
    struct cw_variable {
        size_t id;             // for indexing in id_obj_manager
        uint origin_row;       // 0-indexed
        uint origin_col;       // 0-indexed
        uint length;           // >= MIN_WORD_LEN && <= MAX_WORD_LEN
        word_direction dir;    // direction of word of this var
        string pattern;        // original pattern used to populate domain
        word_domain domain;    // all possible words that fit

        // standard constructor for cw_csp
        cw_variable(size_t id, uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, unordered_set<word_t>&& domain);

        // testing-only constructor
        cw_variable(size_t id, uint origin_row, uint origin_col, uint length, word_direction dir, unordered_set<word_t>&& domain);

        // equality operators
        bool operator==(const cw_variable& rhs) const;
        friend bool operator==(const unique_ptr<cw_variable>& lhs, const unique_ptr<cw_variable>& rhs);

        // operator to print out cw_variable for debug
        friend ostream& operator<<(ostream& os, const cw_variable& var);

        // for copying, must deep copy/clone other domain
        cw_variable(const cw_variable& other);
        cw_variable& operator=(const cw_variable& other);

        // moving ok with default since id_obj_manager has move semantics
        cw_variable(cw_variable&& other) = default;
        cw_variable& operator=(cw_variable&& other) = default;
        
        ~cw_variable() = default;

        unique_ptr<cw_variable> clone() const { 
            return make_unique<cw_variable>(*this);
        }
    };

    // base struct to represent a constraint
    struct cw_constraint {
        public:
            // for indexing in id_obj_manager
            size_t id;

            // constraint operations
            virtual bool prune_domain(id_obj_manager<cw_variable>& vars) = 0;
            virtual bool satisfied(const id_obj_manager<cw_variable>& vars) const = 0;
            virtual bool invalid(const id_obj_manager<cw_variable>& vars) const = 0;

            // ids of vars whose domain modification need re-evaluation of this constraint
            // used to build arc dependencies table in cw_csp
            virtual vector<size_t> dependencies() const = 0;

            // ids of var which may be modified in prune_domain() which can affect dependent arcs
            virtual size_t dependent() const = 0;

            // for each pair of intersecting variables, their lhs/rhs index values
            virtual vector<pair<uint, uint> > intersection_indices() const = 0;

            // equality operator
            friend bool operator==(const cw_constraint& lhs, const cw_constraint& rhs);
            friend bool operator==(const unique_ptr<cw_constraint>& lhs, const unique_ptr<cw_constraint>& rhs);
            virtual bool equals(const cw_constraint&) const = 0;

            // operator to print out cw_constraint for debug
            friend ostream& operator<<(ostream& os, const cw_constraint& var);
            virtual void serialize(ostream& os) const = 0;
            
            // prototype pattern for copying in id_obj_manager
            virtual unique_ptr<cw_constraint> clone() const = 0;

            virtual ~cw_constraint() = default;

        protected:
            cw_constraint(size_t id) : id(id) {}
    };

    // equality constraints between 2 letters in 2 cw vars
    // uni-directional, in constraint set both a constraint and its reverse must both exist
    struct cw_arc : public cw_constraint {
        uint lhs_index;                                      // index of shared letter in lhs
        uint rhs_index;                                      // index of shared letter in rhs
        size_t lhs{id_obj_manager<cw_variable>::INVALID_ID}; // index of lhs var in an id_obj_manager
        size_t rhs{id_obj_manager<cw_variable>::INVALID_ID}; // index of rhs var in an id_obj_manager

        // default constructor for initialization in var_intersect_table
        cw_arc() : cw_constraint(id_obj_manager<cw_constraint>::INVALID_ID) {}
        
        // value constructor
        cw_arc(size_t id, uint lhs_index, uint rhs_index, size_t lhs, size_t rhs);

        // AC-3 step; remove all words in lhs domain that don't have a corresponding rhs word in its domain
        virtual bool prune_domain(id_obj_manager<cw_variable>& vars) override;

        // used by solved() in cw_csp to check that this constraint is satisfied
        virtual bool satisfied(const id_obj_manager<cw_variable>& vars) const override;

        // returns true iff lhs domain is now empty
        virtual bool invalid(const id_obj_manager<cw_variable>& vars) const override;

        // only dependency is rhs
        virtual vector<size_t> dependencies() const override;

        // only dependent is lhs
        virtual size_t dependent() const override;

        // only pair of intersections is (lhs_index, rhs_index)
        virtual vector<pair<uint, uint> > intersection_indices() const override;
        
        // equality operator
        virtual bool equals(const cw_constraint& other_constr) const override;

        // helper for operator to print out cw_constraint for debug
        virtual void serialize(ostream& os) const override;
        
        // copy
        virtual unique_ptr<cw_constraint> clone() const override { 
            return make_unique<cw_arc>(*this);
        }

        virtual ~cw_arc() override = default;
    };

    // cycle constraint 
    // the last arc rhs variable is the lhs variable in the first arc
    // adjacent lhs and rhs variables in different adjacent arcs are the same
    // TODO: store data about previous calls to word_domain.has_letters_at_index_with_letter_assigned() to avoid recomputation
    struct cw_cycle : public cw_constraint {
        vector<size_t>            var_cycle;     // first and last elements must be the same
        vector<pair<uint, uint> > intersections; // must have same len of vars, minus 1

        // construct using existing arcs 
        cw_cycle(size_t id, const id_obj_manager<cw_constraint>& constrs, const vector<size_t>& arcs);

        // AC-N step; remove all words in first var's domain that don't have a path to the last var
        virtual bool prune_domain(id_obj_manager<cw_variable>& vars) override;

        // used by solved() in cw_csp to check that this constraint is satisfied
        virtual bool satisfied(const id_obj_manager<cw_variable>& vars) const override;

        // returns true iff lhs domain is now empty
        virtual bool invalid(const id_obj_manager<cw_variable>& vars) const override;

        // all elements in vars are dependencies
        virtual vector<size_t> dependencies() const override;

        // only dependent is first element in vars
        virtual size_t dependent() const override;

        // equivalent to getter for intersections
        virtual vector<pair<uint, uint> > intersection_indices() const override;

        // equality operator
        virtual bool equals(const cw_constraint& other_constr) const override;

        // helper for operator to print out cw_constraint for debug
        virtual void serialize(ostream& os) const override;

        // copy
        virtual unique_ptr<cw_constraint> clone() const override { 
            return make_unique<cw_cycle>(*this);
        }

        virtual ~cw_cycle() override = default;
    };
} // cw_csp_data_types_ns

/**
 * hash function specialization declarations
*/
namespace std {
    template <>
    struct hash<unique_ptr<cw_csp_data_types_ns::cw_variable> > {
        size_t operator()(const unique_ptr<cw_csp_data_types_ns::cw_variable>& var) const;
    };

    template <>
    struct hash<unique_ptr<cw_csp_data_types_ns::cw_constraint> > {
        size_t operator()(const unique_ptr<cw_csp_data_types_ns::cw_constraint>& var) const;
    };
} // std


#endif // CW_CSP_DATA_TYPES_H