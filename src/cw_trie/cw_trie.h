// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structures for cw_csp
// ==================================================================

#ifndef CW_TRIE_H
#define CW_TRIE_H

#include "cw_trie_data_types.h"
#include "../common/common_parent.h"

using namespace cw_trie_data_types_ns;
using namespace common_parent_ns;

namespace cw_trie_ns {
    /**
     * @brief class representation of a word trie for variable domain initialization and tracking
    */
    class cw_trie : public common_parent {
        public:
            // base constructor
            cw_trie(string name);
            
            // constructor with filepath
            cw_trie(string name, string filepath);

            // constructor with filepath opt
            cw_trie(string name, optional<string> filepath_opt);

            // add word to trie, TODO: should this be by reference?
            void add_word(word_t w); 

            // word check
            bool is_word(string& word) const;

            // find all words that match a pattern
            // TODO: add const correctness
            unordered_set<word_t> find_matches(const string& pattern);

            // read function for entries in letters_at_indices
            uint num_letters_at_index(uint index, char letter) const;

            // deletion function for words with letters at an index
            void remove_matching_words(unordered_set<word_t>& pruned_words, uint index, char letter);

            // start new AC-3 algorithm call, i.e. add new blank layer to  

            // undo previous AC-3 algorithm call, i.e. pop top layer of 

            // assign domain to a single value, repeat calls overwrite assigned value
            void assign_domain(word_t new_value) { assigned = true; assigned_value = new_value; }

            // unassign domain, restoring the preivous trie, used when all values fail in backtracking
            void unassign_domain() { assigned = false; assigned_value.reset(); }

            // get size of domain remaining, for ac3 validity checking
            size_t domain_size() const;

            // expose letters_at_indicies for testing
            array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> get_letters_at_indices() { return letters_at_indices; }

            // expose word_map for testing, undefined if domain assigned
            unordered_map<string, word_t>& get_word_map() { return word_map; }

            // TODO: maybe add something to undo the previous call to remove_matching_words()
        
        private:
            // opt file that this object may have read from
            optional<string> filepath_opt;

            // trie of all words
            shared_ptr<trie_node> trie;

            // map of all words to word structs (with heuristics) for O(1) validity checking & struct lookup for find_matches()
            // contents undefined if assigned (contains data for previous trie)
            unordered_map<string, word_t> word_map;

            // stores # of words with letters at each index
            // contents undefined if domain assigned
            array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> letters_at_indices;

            // each layer corresponds to one call to AC-3 algorithm, and contains nodes pruned from each element in letters_at_indices
            // top layer corresponds to most recent AC-3 call
            // stack<array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > ac3_pruned_stack;

            // true iff domain has been assigned to a single value --> ignore trie
            bool assigned;

            // meaningful iff assigned true, if doesn't have value, then domain is empty
            optional<word_t> assigned_value;

            // helper for filepath constructor to detect file type
            bool has_suffix(const string& str, const string& suffix);

            // helper for filepath constructor to check if word is legal
            optional<string> parse_word(const string& word);

            // helper function for add_word()
            void add_word_to_trie(shared_ptr<trie_node> node, string& word, uint pos);

            // helper function for find_matches()
            // TODO: add const correctness
            void traverse_to_find_matches(unordered_set<word_t>& matches, const string& pattern, uint pos, shared_ptr<trie_node> node, string fragment);

            // upwards recursive deletion helper func for remove_matching_words()
            void remove_from_parents(shared_ptr<trie_node> node, uint& num_leafs, int index, bool letters_at_indices_updated);

            // downwards recursive deletion helper func for remove_matching_words()
            uint remove_children(shared_ptr<trie_node> node, unordered_set<word_t>& pruned_words, uint index, string fragment);

            // helper for remove_matching_words(), gets preceding word fragment from a node targeted for removal
            string get_fragment(shared_ptr<trie_node> node);
    }; // cw_trie
}; // cw_trie_ns

#endif // CW_TRIE_H