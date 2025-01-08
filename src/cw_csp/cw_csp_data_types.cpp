// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: constraint satisfaction problem class data type operators/functions
// ==================================================================

#include "cw_csp_data_types.h"

using namespace cw_csp_data_types_ns;

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
       << ", dir: " << crossword_data_types_ns::word_dir_name.at(var.dir) << ", pattern: " 
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
 * TODO: convert this to construct new object using copy constructor then std::swap all members
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
    cw_assert(intersections.size() > 0);

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
            cw_assert(vars[lhs]->domain.remove_matching_words(lhs_index, static_cast<char>(i + 'a')) > 0);
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
    cw_assert_m(lhs_word != rhs_word, "word equality between constrainted vars");
    cw_assert_m(lhs_word.at(lhs_index) == rhs_word.at(rhs_index), "letter inequality at constraint");

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
    cw_assert(rhs != id_obj_manager<cw_variable>::INVALID_ID);
    return {rhs};
}

/**
 * @brief get id of variable upon which other constraints may be dependent
*/
unordered_set<size_t> cw_arc::dependents() const {
    cw_assert(lhs != id_obj_manager<cw_variable>::INVALID_ID);
    return {lhs};
}

/**
 * @brief get pairs of indices at which adjacent variables in this constraint intersect
*/
vector<pair<uint, uint> > cw_arc::intersection_indices() const {
    return {{lhs_index, rhs_index}};  
}

/**
 * @brief get indices of tiles at which vars for this constraint intersect, for an arc, there is only 1
 *
 * @param vars ref to id_obj_manager to index into to get lhs/rhs vars
 * @return the single {row, col} tile at which vars intersect
*/
vector<pair<uint, uint> > cw_arc::intersection_tiles(const id_obj_manager<cw_variable>& vars) const {
    pair<uint, uint> lhs_tile = std::make_pair(
        vars[lhs]->origin_row + (vars[lhs]->dir == DOWN   ? lhs_index : 0u),
        vars[lhs]->origin_col + (vars[lhs]->dir == ACROSS ? lhs_index : 0u)
    );
    pair<uint, uint> rhs_tile = std::make_pair(
        vars[rhs]->origin_row + (vars[rhs]->dir == DOWN   ? rhs_index : 0u),
        vars[rhs]->origin_col + (vars[rhs]->dir == ACROSS ? rhs_index : 0u)
    );

    cw_assert(lhs_tile.first == rhs_tile.first && lhs_tile.second == rhs_tile.second);
    return {lhs_tile};
}

// ############### cw_cycle ###############

/**
 * @brief helper for == operator for cw_arc to use in hashset
 * @pre other_constr is instance of the same derived class
*/
bool cw_cycle::equals(const cw_constraint& other_constr) const {
    const cw_cycle& other = static_cast<const cw_cycle&>(other_constr);

    // different length cycle
    if(cycle_len != other.cycle_len) {
        return false;
    }

    cw_assert(var_cycle.size() == cycle_len);
    cw_assert(other.var_cycle.size() == cycle_len);
    cw_assert(intersections.size() == cycle_len);
    cw_assert(other.intersections.size() == cycle_len);

    // all elements line up
    auto exact_match = [this, &other](size_t shift, bool reverse_dir) -> bool {
        for(size_t i = 0; i < cycle_len; ++i) {
            const size_t i0 = i;
            const size_t i1 = reverse_dir ? (cycle_len - i + shift) % cycle_len : (i + shift) % cycle_len;

            if(intersections[i0] != other.intersections[i1]) return false;
        }

        return true;
    };

    // return true if one cycle can be rotated so all elements match up
    for(size_t i = 0; i < cycle_len; ++i) {
        if(exact_match(i, false) || exact_match(i, true)) return true;
    }

    return false;
}

/**
 * @brief operator to print out cw_cycle for debug
*/
void cw_cycle::serialize(ostream& os) const {
    os << "vars{";
    for(size_t i = 0; i < cycle_len; ++i) {
        os << var_cycle[i] << ",";
    }
    os << "}, ";

    os << "intersections{";
    for(size_t i = 0; i < cycle_len; ++i) {
        os << '(' << intersections[i].first << ", " << intersections[i].second << "),";
    }
    os << '}';
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
cw_cycle::cw_cycle(size_t id, const id_obj_manager<cw_constraint>& constrs, const vector<size_t>& arcs)
    : cw_constraint(id),
      cycle_len(arcs.size()) {

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
        cw_assert(curr_arc && next_arc && curr_arc->rhs == next_arc->lhs);

        // enforce no double-visiting
        cw_assert(visited.count(curr_arc->lhs) == 0);
        visited.insert(curr_arc->lhs);

        // add to cycle path
        var_cycle.push_back(curr_arc->lhs);
        intersections.push_back({curr_arc->lhs_index, curr_arc->rhs_index});
    }

    cw_assert(var_cycle.size() == cycle_len);
    cw_assert(intersections.size() == cycle_len);
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
      cycle_len(var_cycle.size()),
      var_cycle(var_cycle),
      intersections(intersections) {
    cw_assert(var_cycle.size() == cycle_len); // complete cycle
    cw_assert(intersections.size() == cycle_len); // one intersection per step between vars
    cw_assert(var_cycle.size() == unordered_set<size_t>(var_cycle.begin(), var_cycle.end()).size()); // no duplicate vars
}

/**
 * @brief AC-N step to prune words in first var domain without cyclical path back to first var
 * 
 * @param  vars ref to id_obj_manager to index into to get vars from
 * @return set of vars whose domains changed
*/
unordered_set<size_t> cw_cycle::prune_domain(id_obj_manager<cw_variable>& vars) {
    // imagine a graph with N=4 layers, with each layer having up to 26 nodes
    // each layer is placed in between two variables that intersect
    // a node exists if the corresponding letter can be placed in the intersection 
    // an edge exists from a node to another node in the next layer iff allowed by has_letters_at_index_with_letter_assigned()

    // letter_nodes[i] represents nodes between var_cycle[i] and var_cycle[(i+1) % N]
    // letter_nodes[i][j] set ==> letter j can be placed between variables var_cycle[i] and var_cycle[(i+1) % N]
    vector<letter_bitset_t> letter_nodes(cycle_len);

    // letter_edges[i] represents edges between nodes in letter_nodes[i] and letter_nodes[(i+1) % N]
    // letter_edges[i][j][k] set ==> edges exists from letter j node in var_cycle[i], and letter k node in var_cycle[(i+1) % N]
    vector<array<letter_bitset_t, NUM_ENGLISH_LETTERS> > letter_edges(cycle_len);

    // letter_in_cycle[i] represents letters placed between variables var_cycle[i] and var_cycle[(i+1) % N] that are part of a cycle
    // letter_in_cycle[i][j] set && letter_nodes[i][j] set ==> letter j between variables var_cycle[i] and var_cycle[(i+1) % N] is part of a cycle
    vector<letter_bitset_t> letter_in_cycle(cycle_len);

    // populate letter_nodes
    for(size_t i = 0; i < cycle_len; ++i) {
        const size_t curr_idx = i;
        const size_t next_idx = (i + 1) % cycle_len;

        const letter_bitset_t curr_letters = vars[var_cycle[curr_idx]]->domain.get_all_letters_at_index(intersections[i].first);
        const letter_bitset_t next_letters = vars[var_cycle[next_idx]]->domain.get_all_letters_at_index(intersections[i].second);

        // if these don't equal, then the arc between these two vars will prune a domain later
        letter_nodes[i] = curr_letters & next_letters;
    }
    
    // populate letter_edges
    for(size_t i = 0; i < cycle_len; ++i) {
        const size_t curr_idx = i;
        const size_t next_idx = (i + 1) % cycle_len;

        // for all nodes in curr_idx layer
        for(size_t j = 0; j < NUM_ENGLISH_LETTERS; ++j) {
            // if node exists
            if(letter_nodes[curr_idx][j]) {
                // draw edges to nodes in next_idx layer
                letter_edges[curr_idx][j] = vars[var_cycle[next_idx]]->domain.has_letters_at_index_with_letter_assigned(
                    intersections[next_idx].first,
                    letter_nodes[next_idx],
                    intersections[curr_idx].second,
                    static_cast<char>(j + 'a')
                );
            }
        }
    }

    /**
     * @brief find all nodes part of any cycle_len cycle that contains a specific node
     * @param idx index of the cycle origin node
     * @param letter letter of the cycle origin node 
     * 
     * @return bitmap vector of length cycle_len representing all nodes in a cycle_len cycle that contains the node specified by idx/letter
     */
    auto get_nodes_of_cycles_containing = [this, &letter_nodes, &letter_edges](const size_t idx, const size_t letter) -> vector<letter_bitset_t> {
        /**
         * @brief finds all nodes reachable using forward edges from this node in cycle_len-1 steps
        */
        auto traverse_forward = [this, &letter_nodes, &letter_edges](const size_t idx, const size_t letter) -> vector<letter_bitset_t> {
            vector<letter_bitset_t> reachable(cycle_len);
            letter_bitset_t curr = letter_bitset_t(1 << letter);
            reachable[idx] = curr; // define current node as reachable

            // only allow N-1 steps to avoid marking nodes in current layer
            for(size_t step = 0; step < cycle_len - 1; ++step) {
                const size_t curr_idx = (idx + step) % cycle_len;
                const size_t next_idx = (idx + step + 1) % cycle_len;

                letter_bitset_t next_layer;
                for(size_t j = 0; j < NUM_ENGLISH_LETTERS; ++j) {
                    if(curr[j]) {
                        // OR to add to nodes we can reach in next layer from our current nodes in current layer
                        next_layer |= letter_edges[curr_idx][j];
                    }
                }

                // only if nodes in next layer exist
                next_layer &= letter_nodes[next_idx];

                // store result, then recurse
                reachable[next_idx] = next_layer;
                curr = next_layer;
            }

            return reachable;
        };

        /**
         * @brief finds all nodes reachable using reverse edges from this node in cycle_len-1 steps
        */
        auto traverse_reverse = [this, &letter_nodes, &letter_edges](const size_t idx, const size_t letter) -> vector<letter_bitset_t> {
            vector<letter_bitset_t> reachable(cycle_len);
            letter_bitset_t curr = letter_bitset_t(1 << letter);
            reachable[idx] = curr; // define current node as reachable

            // only allow N-1 steps to avoid marking nodes in current layer
            for(size_t step = 0; step < cycle_len - 1; ++step) {
                const size_t next_idx = (cycle_len + idx - step - 1) % cycle_len;

                letter_bitset_t next_layer;
                for(size_t j = 0; j < NUM_ENGLISH_LETTERS; ++j) {
                    // set true iff next node has edge to a reachable node in current layer
                    next_layer[j] = (letter_edges[next_idx][j] & curr).any();
                }

                // only if nodes in next layer exist
                next_layer &= letter_nodes[next_idx];

                // store result, then recurse
                reachable[next_idx] = next_layer;
                curr = next_layer;
            }

            return reachable;
        };
        
        vector<letter_bitset_t> reachable_forward = traverse_forward(idx, letter);
        vector<letter_bitset_t> reachable_reverse = traverse_reverse(idx, letter);

        assert(reachable_forward.size() == cycle_len);
        assert(reachable_reverse.size() == cycle_len);

        // all nodes reachable in both directions
        // on the layer of idx, we must have that only have the root letter node is reachable
        // all other layers must all be empty or all be nonempty
        // if they are all nonempty, then all nodes in reachable are part of a cycle_len cycle containing the target node
        vector<letter_bitset_t> reachable(cycle_len);

        // populate reachable
        for(size_t i = 0; i < cycle_len; ++i) {
            // search the graph to find all nodes reachable from this node in cycle_len-1 steps
            // also do the same for traversing the graph in reverse order
            // AND these to yield nodes reachable in both directions
            reachable[i] = (reachable_forward[i] & reachable_reverse[i]);

            // double check that no node that doesn't exist is marked as reachable
            letter_bitset_t no_node = ~letter_nodes[i];
            cw_assert(!(reachable[i] & no_node).any());
        }

        // check that all non root layers of reachable are all either reachable or unreachable together
        bool cycles_exist = reachable[(idx + 1) % cycle_len].any();
        for(size_t i = 2; i < cycle_len; ++i) {
            cw_assert(cycles_exist == reachable[(idx + i) % cycle_len].any());
        }

        // if cycles exist, then all nodes in reachable are part of a cycle_len cycle containing then target node
        if(cycles_exist) {
            return reachable;
        }

        // all relevant reachable layers are empty, thus no cycles containing the target node exist
        return vector<letter_bitset_t>(cycle_len);
    };

    // populate letter_in_cycle
    for(size_t j = 0; j < NUM_ENGLISH_LETTERS; ++j) {
        // layer from which cycles are searched
        // this arbitrary and can be anywhere in [0, cycle_len-1] without affecting the result
        constexpr size_t root_layer = 0ul;

        // explore node for j on root layer only if node exists
        if(letter_nodes[root_layer][j]) {
            // get nodes in cycles with exact length CYCLE_LEN that contain node j on first layer
            vector<letter_bitset_t> nodes_in_cycle = get_nodes_of_cycles_containing(root_layer, j);
            assert(nodes_in_cycle.size() == cycle_len);
            
            // add to letter_in_cycle which contains all cycle nodes
            for(size_t i = 0; i < cycle_len; ++i) {
                letter_in_cycle[i] |= nodes_in_cycle[i];
            }
        }
    }

    // remove letters from domains if node not part of a cycle
    unordered_set<size_t> modified;
    for(size_t i = 0; i < cycle_len; ++i) {
        for(size_t j = 0; j < NUM_ENGLISH_LETTERS; ++j) {
            // remove if node exists AND not part of a cycle
            if(letter_nodes[i][j] && !letter_in_cycle[i][j]) {
                const size_t lhs_idx = i;
                const size_t rhs_idx = (i + 1) % cycle_len;

                vars[var_cycle[lhs_idx]]->domain.remove_matching_words(intersections[i].first,  static_cast<char>(j + 'a'));
                vars[var_cycle[rhs_idx]]->domain.remove_matching_words(intersections[i].second, static_cast<char>(j + 'a'));

                modified.insert(var_cycle[lhs_idx]);
                modified.insert(var_cycle[rhs_idx]);
            }
        }
    }

    return modified;
}

/**
 * @brief checks that constraint is satisfied, used by solved() in cw_csp
 * 
 * @param  vars ref to id_obj_manager to index into to get vars 
 * @return true iff constraint is satisfied
*/
bool cw_cycle::satisfied(const id_obj_manager<cw_variable>& vars) const {
    for(size_t i = 0; i < cycle_len; ++i) {
        if(vars[var_cycle[i]                  ]->domain.size() != 1) return false;
        if(vars[var_cycle[(i + 1) % cycle_len]]->domain.size() != 1) return false;

        // invalid assignments are undone, so this should always be true
        const string lhs_word = vars[var_cycle[i]                  ]->domain.get_cur_domain().at(0).word;
        const string rhs_word = vars[var_cycle[(i + 1) % cycle_len]]->domain.get_cur_domain().at(0).word;
        cw_assert_m(lhs_word != rhs_word, "word equality between constrainted vars");
        cw_assert_m(lhs_word.at(intersections[i].first) == rhs_word.at(intersections[i].second), "letter inequality at constraint");
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

/**
 * @brief get indices of tiles at which vars for this constraint intersect, for a cycle, there is 4
 *
 * @param vars ref to id_obj_manager to index into to get vars
 * @return the four {row, col} tiles at which vars in this cycle intersect
*/
vector<pair<uint, uint> > cw_cycle::intersection_tiles(const id_obj_manager<cw_variable>& vars) const {
    vector<pair<uint, uint> > res;

    for(size_t i = 0; i < cycle_len; ++i) {
        const size_t lhs = var_cycle[i];
        const size_t rhs = var_cycle[(i + 1) % cycle_len];

        pair<uint, uint> lhs_tile = std::make_pair(
            vars[lhs]->origin_row + (vars[lhs]->dir == DOWN   ? intersections[i].first : 0u),
            vars[lhs]->origin_col + (vars[lhs]->dir == ACROSS ? intersections[i].first : 0u)
        );
        pair<uint, uint> rhs_tile = std::make_pair(
            vars[rhs]->origin_row + (vars[rhs]->dir == DOWN   ? intersections[i].second : 0u),
            vars[rhs]->origin_col + (vars[rhs]->dir == ACROSS ? intersections[i].second : 0u)
        );

        cw_assert(lhs_tile.first == rhs_tile.first && lhs_tile.second == rhs_tile.second);
        res.push_back(lhs_tile);
    }

    return res;
}
