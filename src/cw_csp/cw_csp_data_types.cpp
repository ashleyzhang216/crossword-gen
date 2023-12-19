// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: constraint satisfaction problem class data type operators/functions
// ==================================================================

#include "cw_csp_data_types.h"

using namespace cw_csp_data_types_ns;

// ############### set_contents_equal() ###############

template bool set_contents_equal<cw_variable>(
    const unordered_set<cw_variable>* lhs, 
    const unordered_set<cw_variable>* rhs, 
    bool debug_prints
);
template bool set_contents_equal<cw_constraint>(
    const unordered_set<cw_constraint>* lhs, 
    const unordered_set<cw_constraint>* rhs, 
    bool debug_prints
);

// ############### map_to_set_contents_equal() ###############

template bool map_to_set_contents_equal(
    const unordered_map<cw_variable, unordered_set<cw_constraint> >* lhs, 
    const unordered_map<cw_variable, unordered_set<cw_constraint> >* rhs, 
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
        && dir == rhs.dir
        && set_contents_equal(&domain, &(rhs.domain), false);
}

/**
 * @brief hash function for cw_variable
*/
size_t hash<cw_variable>::operator()(const cw_variable& var) const {
    size_t hash_row = hash<uint>{}(var.origin_row);
    size_t hash_col = hash<uint>{}(var.origin_col);
    return hash_row ^ hash_col; 
}

/**
 * @brief operator to print out cw_variable for debug
*/
ostream& cw_csp_data_types_ns::operator<<(ostream& os, const cw_variable& var) {
    os << "row: " << var.origin_row << ", col: " << var.origin_col << ", len: " << var.length
       << ", dir: " << cw_csp_data_types_ns::word_dir_name.at(var.dir) << ", pattern: " 
       << var.pattern << ", assigned: " << var.assigned << ", domain: {";
    for(string word : var.domain) {
        os << word << ", ";
    }
    os << "}";
    return os;
}

/**
 * @brief constructor for cw_variable to be used by cw_csp
 * 
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param pattern word pattern to find matches for to populate domain
 * @param finder ptr to word_finder to populate domain
*/
cw_variable::cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, string pattern, shared_ptr<word_finder> finder) {
    this->origin_row = origin_row;
    this->origin_col = origin_col;
    this->length = length;
    this->dir = dir;
    this->pattern = pattern;
    domain.clear();

    // populate domain
    finder->find_matches(&domain, pattern);
}

/**
 * @brief testing-only constructor for cw_variable to be used by test driver
 * 
 * @param origin_row row of top-left square in var
 * @param origin_col col of top-left square in var
 * @param length num of letters in this var
 * @param dir direction of this var
 * @param domain contents of domain of this var
*/
cw_variable::cw_variable(uint origin_row, uint origin_col, uint length, word_direction dir, unordered_set<string> domain) {
    this->origin_row = origin_row;
    this->origin_col = origin_col;
    this->length = length;
    this->dir = dir;
    this->pattern = "(created w/ testing constructor)";
    this->domain = domain;
}

/**
 * @brief tests if this var contains a word that satisifes one cw equality constraint and isn't the same word
 * @param param_word the other word to compare to
 * @param param_letter_pos index of the letter in param_word that this domain must contain
 * @param letter_pos the index at which the letter must appear in the word
 * @return true iff this var contains 1+ word in its domain that contains letter at position letter_pos
*/
bool cw_variable::can_satisfy_constraint(const string& param_word, const uint& param_letter_pos, const uint& letter_pos) const {

    stringstream ss;
    cw_utils* utils = new cw_utils("satisfies_constraint()", VERBOSITY);
    
    if(param_word.at(param_letter_pos) < 'a' || param_word.at(param_letter_pos) > 'z') {
        ss << "unknown target letter: " << param_word.at(param_letter_pos);
        utils->print_msg(&ss, ERROR);
    }

    for(string word : domain) {
        if(word != param_word && word.at(letter_pos) == param_word.at(param_letter_pos)) return true;
    }

    return false;
}

// ############### cw_constraint ###############

/**
 * @brief == operator for cw_constraint to use in hashset
*/
bool cw_constraint::operator==(const cw_constraint& other) const {
    return lhs_index == other.lhs_index
        && rhs_index == other.rhs_index
        && *lhs == (*other.lhs)
        && *rhs == (*other.rhs);
}

/**
 * @brief hash function for cw_constraint
*/
size_t hash<cw_constraint>::operator()(const cw_constraint& var) const {
    return hash<uint>{}(var.lhs_index) ^ hash<uint>{}(var.rhs_index) ^ hash<cw_variable>{}(*(var.lhs)) ^ hash<cw_variable>{}(*(var.rhs));
}

/**
 * @brief operator to print out cw_constraint for debug
*/
ostream& cw_csp_data_types_ns::operator<<(ostream& os, const cw_constraint& var) {
    os << "lhs: " << *(var.lhs) << " @ index " << var.lhs_index
       << ", rhs: " << *(var.rhs) << " @ index " << var.rhs_index;
    return os;
}

/**
 * @brief testing-only constructor for cw_constraint to be used by test driver
 * 
 * @param lhs_index value of lhs_index to populate
 * @param rhs_index value of rhs_index to populate
 * @param lhs value of lhs ptr to populate
 * @param rhs value of rhs ptr to populate
*/
cw_constraint::cw_constraint(uint lhs_index, uint rhs_index, shared_ptr<cw_variable> lhs, shared_ptr<cw_variable> rhs) {
    this->lhs_index = lhs_index;
    this->rhs_index = rhs_index;
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * @brief AC-3 step to prune words in lhs domain without valid rhs words
 * 
 * @return set of words pruned. i.e. size() == 0 iff domain not change
*/
unordered_set<string> cw_constraint::prune_domain() {

    // find words to prune from domain
    unordered_set<string> pruned_words;
    for(string word : lhs->domain) {
        if(!rhs->can_satisfy_constraint(word, lhs_index, rhs_index)) {
            // queue word to be removed from lhs domain
            pruned_words.insert(word);
        }
    }

    // remove words
    for(string word : pruned_words) lhs->domain.erase(word);

    return pruned_words;
}

/**
 * @brief checks that constraint is satisfied, used by solved() in cw_csp
 * 
 * @return true iff constraint is satisfied
*/
bool cw_constraint::satisfied() const {
    if(lhs->domain.size() != 1) return false;
    if(rhs->domain.size() != 1) return false;

    // since ac3() undoes invalid assignments, this should always be true
    assert(*(lhs->domain.begin()) != *(rhs->domain.begin())); // word inequality
    assert(lhs->domain.begin()->at(lhs_index) == rhs->domain.begin()->at(rhs_index)); // letter equality

    return true;
}