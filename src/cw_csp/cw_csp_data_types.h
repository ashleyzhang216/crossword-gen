// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: data types for constraint satisfaction problem class
// ==================================================================

#ifndef CW_CSP_DATA_TYPES_H
#define CW_CSP_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"
#include "../word_domain/word_domain_data_types.h"
#include "../word_domain/word_domain.h"
#include "../crossword/crossword_data_types.h"

using namespace common_data_types_ns;
using namespace cw;
using namespace word_domain_data_types_ns;
using namespace word_domain_ns;
using namespace crossword_data_types_ns;

namespace cw_csp_data_types_ns {
    // to choose between solving strategies, when more are added in the future
    enum csp_solving_strategy {
        BACKTRACKING = 0,
    };

    // mapping from csp solve strategy to display name
    NLOHMANN_JSON_SERIALIZE_ENUM( csp_solving_strategy, {
        {BACKTRACKING, "Backtracking"},
    })

    // criteria for selecting next variable to assign a value to
    enum var_ordering {
        MRV = 0,
    };

    // mapping from variable strategy to display name
    NLOHMANN_JSON_SERIALIZE_ENUM( var_ordering, {
        {MRV, "Minimum Remaining Values"},
    })

    // criteria for selecting next word value to try to assign
    enum val_ordering {
        HIGH_SCORE_AND_FREQ = 0,
    };

    // mapping from word strategy to display name
    NLOHMANN_JSON_SERIALIZE_ENUM( val_ordering, {
        {HIGH_SCORE_AND_FREQ, "Highest Score, Frequency"},
    })

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

    // friend operator declarations
    bool operator==(const unique_ptr<cw_variable>& lhs, const unique_ptr<cw_variable>& rhs);
    ostream& operator<<(ostream& os, const cw_variable& var);

    // base struct to represent a constraint
    struct cw_constraint {
        public:
            // for indexing in id_obj_manager
            size_t id;

            // constraint operations
            virtual unordered_map<size_t, size_t> prune_domain(id_obj_manager<cw_variable>& vars) = 0;
            virtual bool satisfied(const id_obj_manager<cw_variable>& vars) const = 0;
            virtual bool invalid(const id_obj_manager<cw_variable>& vars) const = 0;

            // ids of vars whose domain modification need re-evaluation of this constraint
            // used to build constr_dependencies table in cw_csp
            virtual unordered_set<size_t> dependencies() const = 0;

            // ids of var which may be modified in prune_domain(), possibly affecting dependent constraints
            virtual unordered_set<size_t> dependents() const = 0;

            // number of variables traversed over
            virtual size_t size() const = 0;

            // for each pair of intersecting variables, their lhs/rhs index values
            virtual vector<pair<uint, uint> > intersection_indices() const = 0;

            // tiles at which this constraint intersect, in the form of {row, col}
            virtual vector<pair<uint, uint> > intersection_tiles(const id_obj_manager<cw_variable>& vars) const = 0;

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

    // friend operator declarations
    bool operator==(const cw_constraint& lhs, const cw_constraint& rhs);
    bool operator==(const unique_ptr<cw_constraint>& lhs, const unique_ptr<cw_constraint>& rhs);
    ostream& operator<<(ostream& os, const cw_constraint& var);

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
        virtual unordered_map<size_t, size_t> prune_domain(id_obj_manager<cw_variable>& vars) override;

        // used by solved() in cw_csp to check that this constraint is satisfied
        virtual bool satisfied(const id_obj_manager<cw_variable>& vars) const override;

        // returns true iff lhs domain is now empty
        virtual bool invalid(const id_obj_manager<cw_variable>& vars) const override;

        // only dependency is rhs
        virtual unordered_set<size_t> dependencies() const override;

        // only dependent is lhs
        virtual unordered_set<size_t> dependents() const override;

        // always traverses over exactly 2 variables
        virtual size_t size() const override;

        // only pair of intersections is (lhs_index, rhs_index)
        virtual vector<pair<uint, uint> > intersection_indices() const override;

        // tiles at which this constraint intersect, in the form of {row, col}
        virtual vector<pair<uint, uint> > intersection_tiles(const id_obj_manager<cw_variable>& vars) const override;
        
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

    // cycle constraint between 4 distinct variables
    // the last arc rhs variable is the lhs variable in the first arc
    // adjacent lhs and rhs variables in different adjacent arcs are the same
    struct cw_cycle : public cw_constraint {
        // for performance reasons, these are restricted
        static constexpr size_t MIN_CYCLE_LEN = 4ul;
        static constexpr size_t MAX_CYCLE_LEN = 4ul;

        // the length of var_cycle and intersections
        size_t cycle_len;

        // first element is implied to be repeated to complete a cycle
        // must have exact length of cycle_len
        vector<size_t> var_cycle;

        // must have same len of var_cycle, i.e. cycle_len, intersections[i] describes var_cycle[i] and var_cycle[i+1], with wraparound
        vector<pair<uint, uint> > intersections;

        // construct using existing arcs 
        cw_cycle(size_t id, const id_obj_manager<cw_constraint>& constrs, const vector<size_t>& arcs);

        // testing-only constructor
        cw_cycle(size_t id, const vector<size_t>& var_cycle, const vector<pair<uint, uint> >& intersections);

        // AC-N step; remove all words in first var's domain that don't have a path to the last var
        virtual unordered_map<size_t, size_t> prune_domain(id_obj_manager<cw_variable>& vars) override;

        // used by solved() in cw_csp to check that this constraint is satisfied
        virtual bool satisfied(const id_obj_manager<cw_variable>& vars) const override;

        // returns true iff any domain is now empty
        virtual bool invalid(const id_obj_manager<cw_variable>& vars) const override;

        // all elements in var_cycle are dependencies
        virtual unordered_set<size_t> dependencies() const override;

        // all elements in var_cycle are dependents
        virtual unordered_set<size_t> dependents() const override;

        // traverses over one variable per element in var_cycle
        virtual size_t size() const override;

        // equivalent to getter for intersections
        virtual vector<pair<uint, uint> > intersection_indices() const override;

        // tiles at which this constraint intersect, in the form of {row, col}
        virtual vector<pair<uint, uint> > intersection_tiles(const id_obj_manager<cw_variable>& vars) const override;

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