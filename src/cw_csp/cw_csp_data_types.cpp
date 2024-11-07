// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: constraint satisfaction problem class data type operators/functions
// ==================================================================

#include "cw_csp_data_types.h"

using namespace cw_csp_data_types_ns;

// ############### set_contents_equal() ###############

template bool set_contents_equal<unique_ptr<cw_variable> >(
    const unordered_set<unique_ptr<cw_variable> >& lhs, 
    const unordered_set<unique_ptr<cw_variable> >& rhs, 
    bool debug_prints
);
template bool set_contents_equal<unique_ptr<cw_constraint> >(
    const unordered_set<unique_ptr<cw_constraint> >& lhs, 
    const unordered_set<unique_ptr<cw_constraint> >& rhs, 
    bool debug_prints
);

// ############### map_to_set_contents_equal() ###############

template bool map_to_set_contents_equal(
    const unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& lhs, 
    const unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > >& rhs, 
    bool debug_prints
);

// ############### cw_variable ###############

/**
 * @brief == operator for cw_variable to use in hashset
*/
bool cw_variable::operator==(const cw_variable& rhs) const {
    return origin_row == rhs.origin_row
        && origin_col == rhs.origin_col
        && length == rhs.length  
        && dir == rhs.dir;
}

/**
 * @brief == operator for ptr to cw_variable to use for test drivers
*/
bool cw_csp_data_types_ns::operator==(const unique_ptr<cw_variable>& lhs, const unique_ptr<cw_variable>& rhs) {
    return *lhs == *rhs;
}

/**
 * @brief hash function for cw_variable
*/
size_t std::hash<unique_ptr<cw_variable> >::operator()(const unique_ptr<cw_variable>& var) const {
    size_t hash_row = hash<uint>{}(var->origin_row);
    size_t hash_col = hash<uint>{}(var->origin_col);
    return hash_row ^ hash_col; 
}

/**
 * @brief operator to print out cw_variable for debug
*/
ostream& cw_csp_data_types_ns::operator<<(ostream& os, const cw_variable& var) {
    os << "cw_variable(row: " << var.origin_row << ", col: " << var.origin_col << ", len: " << var.length
       << ", dir: " << cw_csp_data_types_ns::word_dir_name.at(var.dir) << ", pattern: " 
       << var.pattern << ", domain: {";
    for(word_t w : var.domain.get_cur_domain()) {
        os << w.word << ", ";
    }
    os << "})";
    return os;
}

/**
 * @brief copy constructor for cw_variable
*/
cw_variable::cw_variable(const cw_variable& other)
    : id(other.id),
      origin_row(other.origin_row),
      origin_col(other.origin_col),
      length(other.length),
      dir(other.dir),
      pattern(other.pattern),
      domain(other.domain) {
    // do nothing else
}

/**
 * @brief copy assignment for cw_variable
*/
cw_variable& cw_variable::operator=(const cw_variable& other) {
    if (this != &other) {
        id = other.id;
        origin_row = other.origin_row;
        origin_col = other.origin_col;
        length = other.length;
        dir = other.dir;
        pattern = other.pattern;
        domain = other.domain;
    }
    return *this;
}

/**
 * @brief constructor for cw_variable to be used by cw_csp
 * 
 * @param id index of this cw_variable in an id_obj_manager
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param pattern word pattern to find matches for to populate domain
 * @param domain contents of domain of this var
*/
cw_variable::cw_variable(size_t id, uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, unordered_set<word_t>&& domain) 
    : id(id),
      origin_row(origin_row),
      origin_col(origin_col),
      length(length),
      dir(dir),
      pattern(pattern),
      domain("cw_variable domain", std::move(domain)) {
    // do nothing else
}

/**
 * @brief testing-only constructor for cw_variable to be used by test driver
 * 
 * @param id index of this cw_variable in an id_obj_manager
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param domain contents of domain of this var
*/
cw_variable::cw_variable(size_t id, uint origin_row, uint origin_col, uint length, word_direction dir, unordered_set<word_t>&& domain) 
    : cw_variable(id, origin_row, origin_col, length, dir, "(created w/ testing constructor)", std::move(domain)) {
    // do nothing, delegated to constructor that does everything needed
}

// ############### cw_constraint ###############

/**
 * @brief == operator for cw_constraint to use in hashset
*/
bool cw_csp_data_types_ns::operator==(const cw_constraint& lhs, const cw_constraint& rhs) {
    if(typeid(lhs) != typeid(rhs)) return false;
    return lhs.equals(rhs);
}

/**
 * @brief == operator for ptr to cw_constraint to use for test drivers
*/
bool cw_csp_data_types_ns::operator==(const unique_ptr<cw_constraint>& lhs, const unique_ptr<cw_constraint>& rhs) {
    return *lhs == *rhs;
}

/**
 * @brief call child implementation of operator<< when printing for debug
*/
ostream& cw_csp_data_types_ns::operator<<(ostream& os, const cw_constraint& var) {
    var.serialize(os);
    return os;
}

/**
 * @brief hash function for cw_constraint
*/
size_t std::hash<unique_ptr<cw_constraint> >::operator()(const unique_ptr<cw_constraint>& constr) const {
    auto intersections = constr->intersection_indices();
    assert(intersections.size() > 0);

    size_t h = 0ul;
    for(const auto& intersection : intersections) {
        h ^= hash<size_t>{}(static_cast<size_t>(intersection.first << 16 | intersection.second));
    }
    return h;
}

// ############### cw_arc ###############

/**
 * @brief helper for == operator for cw_arc to use in hashset
 * @pre other_constr is instance of the same derived class
*/
bool cw_arc::equals(const cw_constraint& other_constr) const {
    const cw_arc& other = static_cast<const cw_arc&>(other_constr);

    // cannot compare lhs or rhs since ids can be ordered differently, and we can't check underlying object
    return lhs_index == other.lhs_index
        && rhs_index == other.rhs_index;
}

/**
 * @brief operator to print out cw_arc for debug
*/
void cw_arc::serialize(ostream& os) const {
    os << "cw_arc(id: " << id << ", lhs: " << lhs << " @ index " << lhs_index
       << ", rhs: " << rhs << " @ index " << rhs_index << ")";
}

/**
 * @brief value constructor for cw_arc
 * 
 * @param id index of this cw_arc in an id_obj_manager
 * @param lhs_index value of lhs_index to populate
 * @param rhs_index value of rhs_index to populate
 * @param lhs value of lhs index in an id_obj_manager to populate
 * @param rhs value of rhs index in an id_obj_manager to populate
*/
cw_arc::cw_arc(size_t id, uint lhs_index, uint rhs_index, size_t lhs, size_t rhs) 
    : cw_constraint(id), lhs_index(lhs_index), rhs_index(rhs_index), lhs(lhs), rhs(rhs)
{
    // do nothing, initialization is sufficient
}

/**
 * @brief AC-3 step to prune words in lhs domain without valid rhs words
 * 
 * @param  vars ref to id_obj_manager to index into to get lhs/rhs vars
 * @return set of vars whose domains changed
*/
unordered_set<size_t> cw_arc::prune_domain(id_obj_manager<cw_variable>& vars) {
    unordered_set<size_t> result;

    letter_bitset_t lhs_letters = vars[lhs]->domain.get_all_letters_at_index(lhs_index);
    letter_bitset_t rhs_letters = vars[rhs]->domain.get_all_letters_at_index(rhs_index);
    for(uint i = 0; i < NUM_ENGLISH_LETTERS; ++i) {
        if(lhs_letters[i] && !rhs_letters[i]) {
            // cannot satisfy constraint for this letter
            assert(vars[lhs]->domain.remove_matching_words(lhs_index, static_cast<char>(i + 'a')) > 0);
            result.insert(lhs);
        }
    }

    return result;
}

/**
 * @brief checks that constraint is satisfied, used by solved() in cw_csp
 * 
 * @param  vars ref to id_obj_manager to index into to get lhs/rhs vars 
 * @return true iff constraint is satisfied
*/
bool cw_arc::satisfied(const id_obj_manager<cw_variable>& vars) const {
    if(vars[lhs]->domain.size() != 1) return false;
    if(vars[rhs]->domain.size() != 1) return false;

    // since ac3() undoes invalid assignments, this should always be true
    const string lhs_word = vars[lhs]->domain.get_cur_domain().at(0).word;
    const string rhs_word = vars[rhs]->domain.get_cur_domain().at(0).word;
    assert_m(lhs_word != rhs_word, "word equality between constrainted vars");
    assert_m(lhs_word.at(lhs_index) == rhs_word.at(rhs_index), "letter inequality at constraint");

    return true;
}

/**
 * @brief checks if this constraint is now invalid as a result of an ac3 step, i.e. lhs domain empty
 * 
 * @param  vars ref to id_obj_manager to index into to get lhs var
 * @return true iff lhs domain empty
*/
bool cw_arc::invalid(const id_obj_manager<cw_variable>& vars) const {
    return vars[lhs]->domain.size() == 0;
}

/**
 * @brief get ids of variables upon which this constraint is dependent 
*/
unordered_set<size_t> cw_arc::dependencies() const {
    assert(rhs != id_obj_manager<cw_variable>::INVALID_ID);
    return {rhs};
}

/**
 * @brief get id of variable upon which other constraints may be dependent
*/
unordered_set<size_t> cw_arc::dependents() const {
    assert(lhs != id_obj_manager<cw_variable>::INVALID_ID);
    return {lhs};
}

/**
 * @brief get pairs of indices at which adjacent variables in this constraint intersect
*/
vector<pair<uint, uint> > cw_arc::intersection_indices() const {
    return {{lhs_index, rhs_index}};  
}

// ############### cw_cycle ###############

/**
 * @brief helper for == operator for cw_arc to use in hashset
 * @pre other_constr is instance of the same derived class
*/
bool cw_cycle::equals(const cw_constraint& other_constr) const {
    const cw_cycle& other = static_cast<const cw_cycle&>(other_constr);

    // size check
    if(var_cycle.size() != other.var_cycle.size() || intersections.size() != other.intersections.size()) {
        return false;
    }

    // all elements line up
    auto exact_match = [this, &other](size_t shift) -> bool {
        for(size_t i = 0; i < var_cycle.size(); ++i) {
            if(var_cycle[i] != other.var_cycle[(i + shift) % other.var_cycle.size()]) return false;
        }

        for(size_t i = 0; i < intersections.size(); ++i) {
            if(intersections[i] != other.intersections[(i + shift) % other.intersections.size()]) return false;
        }

        return true;
    };

    // return true if one cycle can be rotated so all elements match up
    for(size_t i = 0; i < var_cycle.size(); ++i) {
        if(exact_match(i)) return true;
    }

    return false;
}

/**
 * @brief operator to print out cw_cycle for debug
*/
void cw_cycle::serialize(ostream& os) const {
    for(size_t i = 0; i < intersections.size(); ++i) {
        os << '(' << var_cycle[i] << " @ " << intersections[i].first << " == " << var_cycle[(i+1) % var_cycle.size()] << " @ " << intersections[i].second << ", ";
    }
}

/**
 * @brief constructor for cw_cycle, constructs using existing arc constraints
 * @pre all constraints arcs indexes to in constrs are of derived class cw_arc
 * @pre length of arcs is exactly 4 arcs and to complete a cycle
 * @note dynamic_cast is used here, but for good reason- but we are guaranteed that all cw_constraint we index to are of type cw_arc
 * 
 * @param id index of this cw_cycle in constrs once constructed
 * @param constrs ref to id_obj_manager holding cw_arc to construct this cw_cycle from
 * @param arcs nonempty vec of arc indices in constrs to construct this cw_cycle from, in reverse order
*/
cw_cycle::cw_cycle(size_t id, const id_obj_manager<cw_constraint>& constrs, const vector<size_t>& arcs) : cw_constraint(id) {
    assert(arcs.size() == 4);

    // for checking invariant
    unordered_set<size_t> visited;

    // iterate in reverse order, matching order of arcs
    for(int i = static_cast<int>(arcs.size() - 1); i >= 0; --i) {
        // overlapping arcs
        const size_t curr_idx = static_cast<size_t>(i);
        const size_t next_idx = (i == 0) ? (arcs.size() - 1) : static_cast<size_t>(i - 1);

        // enforce arcs overlap adjacent vars
        // arcs is assumed to be ordered so L/R adjacent arcs share their lhs/rhs vars respectively
        const cw_constraint * const curr_constr = constrs[arcs[curr_idx]].get();
        const cw_constraint * const next_constr = constrs[arcs[next_idx]].get();
        const cw_arc * const curr_arc = dynamic_cast<const cw_arc* const>(curr_constr);
        const cw_arc * const next_arc = dynamic_cast<const cw_arc* const>(next_constr);
        assert(curr_arc && next_arc && curr_arc->rhs == next_arc->lhs);

        // enforce no double-visiting
        assert(visited.count(curr_arc->lhs) == 0);
        visited.insert(curr_arc->lhs);

        // add to cycle path
        var_cycle.push_back(curr_arc->lhs);
        intersections.push_back({curr_arc->lhs_index, curr_arc->rhs_index});
    }
}

/**
 * @brief testing-only constructor for cw_cycle, directly initialize all fields
 * @pre var_cycle completes a full cycle
 * @pre intersections has one valid intersection per step between vars
 *
 * @param id index of this cw_cycle in constrs once constructed
 * @param var_cycle value to initialize var_cycle to
 * @param intersections value to initialize intersections to
*/
cw_cycle::cw_cycle(size_t id, const vector<size_t>& var_cycle, const vector<pair<uint, uint> >& intersections)
    : cw_constraint(id),
      var_cycle(var_cycle),
      intersections(intersections) {
    assert(var_cycle.size() == 4); // complete cycle
    assert(var_cycle.size() == intersections.size()); // one intersection per step between vars
    assert(var_cycle.size() == unordered_set<size_t>(var_cycle.begin(), var_cycle.end()).size()); // no duplicate vars
}

/**
 * @brief AC-N step to prune words in first var domain without cyclical path back to first var
 * 
 * @param  vars ref to id_obj_manager to index into to get vars from
 * @return set of vars whose domains changed
*/
unordered_set<size_t> cw_cycle::prune_domain(id_obj_manager<cw_variable>& vars) {
    // TODO: implement
    (void)vars;

    /**
     * idea: track the letters that can be placed for each step between vars, each is a node
     * an edge from one node to another node in layer below it if has_letters_at_index_with_letter_assigned() finds it
     * node can only stay if it has a path back to itself
     *
     * see notebook where i drew a diagram example
    */

    return {};
}

/**
 * @brief checks that constraint is satisfied, used by solved() in cw_csp
 * 
 * @param  vars ref to id_obj_manager to index into to get vars 
 * @return true iff constraint is satisfied
*/
bool cw_cycle::satisfied(const id_obj_manager<cw_variable>& vars) const {
    for(size_t i = 0; i < var_cycle.size(); ++i) {
        if(vars[var_cycle[i]                         ]->domain.size() != 1) return false;
        if(vars[var_cycle[(i + 1) % var_cycle.size()]]->domain.size() != 1) return false;

        // invalid assignments are undone, so this should always be true
        const string lhs_word = vars[var_cycle[i]                         ]->domain.get_cur_domain().at(0).word;
        const string rhs_word = vars[var_cycle[(i + 1) % var_cycle.size()]]->domain.get_cur_domain().at(0).word;
        assert_m(lhs_word != rhs_word, "word equality between constrainted vars");
        assert_m(lhs_word.at(intersections[i].first) == rhs_word.at(intersections[i].second), "letter inequality at constraint");
    }

    return true;
}

/**
 * @brief checks if this constraint is now invalid as a result of an AC-N step, i.e. first var domain empty
 * 
 * @param  vars ref to id_obj_manager to index into to get var
 * @return true iff lhs domain empty
*/
bool cw_cycle::invalid(const id_obj_manager<cw_variable>& vars) const {
    for(size_t d : dependents()) {
        if(vars[d]->domain.size() == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief get ids of variables upon which this constraint is dependent 
*/
unordered_set<size_t> cw_cycle::dependencies() const {
    return {var_cycle.begin(), var_cycle.end()};
}

/**
 * @brief get id of variable upon which other constraints may be dependent
*/
unordered_set<size_t> cw_cycle::dependents() const {
    return {var_cycle.begin(), var_cycle.end()};
}

/**
 * @brief get pairs of indices at which adjacent variables in this constraint intersect
*/
vector<pair<uint, uint> > cw_cycle::intersection_indices() const {
    return intersections;
}
