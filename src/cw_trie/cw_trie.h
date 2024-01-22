// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structures for cw_csp and word_finder
// ==================================================================

#ifndef CW_TRIE_H
#define CW_TRIE_H

#include "cw_trie_data_types.h"
#include "../common/common_parent.h"

using namespace cw_trie_data_types_ns;
using namespace common_parent_ns;

namespace cw_trie_ns {
    /**
     * @brief class representation of a word trie and related contents tracking data structures
    */
    class cw_trie : public common_parent {
        public:
            // base constructor
            cw_trie(string name);

            // add word to trie, TODO: should this be by reference?
            void add_word(word_t w); 

            // find all words that match a pattern
            // TODO: add const correctness
            void find_matches(shared_ptr<unordered_set<word_t> > matches, string& pattern);

            // read function for entries in letters_at_indices
            uint num_letters_at_index(uint index, char letter) const;

            // deletion function for words with letters at an index
            void remove_matching_words(shared_ptr<unordered_set<word_t> > pruned_words, uint index, char letter);

            // expose letters_at_indicies for testing
            array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> get_letters_at_indices() { return letters_at_indices; }

            // expose word_map for testing
            unordered_map<string, word_t> get_word_map() { return word_map; }

            // TODO: maybe add something to undo the previous call to remove_matching_words()

            // TODO: maybe add something to represent setting the domain to a single value
        
        private:
            // trie of all words
            shared_ptr<trie_node> trie;

            // map of all words to word structs (with heuristics) for O(1) validity checking & struct lookup for find_matches()
            unordered_map<string, word_t> word_map;

            // stores # of words with letters at each index
            array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> letters_at_indices;

            // helper function for add_word()
            void add_word_to_trie(shared_ptr<trie_node> node, string& word, uint pos);

            // helper function for find_matches()
            // TODO: add const correctness
            void traverse_to_find_matches(shared_ptr<unordered_set<word_t> > matches, string& pattern, uint pos, shared_ptr<trie_node> node, string fragment);

            // upwards recursive deletion helper func for remove_matching_words()
            void remove_from_parents(shared_ptr<trie_node> node, uint& num_leafs, uint index, char child);

            // downwards recursive deletion helper func for remove_matching_words()
            uint remove_children(shared_ptr<trie_node> node, shared_ptr<unordered_set<word_t> > pruned_words, uint index, string fragment);

            // helper for remove_matching_words(), gets preceding word fragment from a node targeted for removal
            string get_fragment(shared_ptr<trie_node> node);
    }; // cw_trie
}; // cw_trie_ns

#endif // CW_TRIE_H