// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structures for cw_csp and cw_variable
// ==================================================================

#ifndef WORD_DOMAIN_H
#define WORD_DOMAIN_H

#include "word_domain_data_types.h"
#include "../common/common_parent.h"

using namespace word_domain_data_types_ns;
using namespace common_parent_ns;

namespace word_domain_ns {
    /**
     * @brief class representation of a word trie for variable domain initialization and tracking
    */
    class word_domain : public common_parent {
        public:
            // base constructor
            word_domain(string name);
            
            // constructor with filepath
            word_domain(string name, string filepath, bool print_progress_bar = false);

            // constructor with filepath opt
            word_domain(string name, optional<string> filepath_opt, bool print_progress_bar = false);

            // constructor with set of domain, exclusively for cw_variable domain representation
            word_domain(string name, unordered_set<word_t>&& domain);

            // add word to trie
            void add_word(word_t w); 

            // word check
            bool is_word(string& word) const;

            // find all words that match a pattern
            unordered_set<word_t> find_matches(const string& pattern) const;

            // read function for entries in letters_at_indices
            uint num_letters_at_index(uint index, char letter) const;

            // read function for entries in letters_at_indices given a specific letter at a specific index
            letter_bitset_t has_letters_at_index_with_letter_assigned(uint index, const letter_bitset_t& letters, uint required_index, char required_letter) const;

            // deletion function for words with letters at an index
            size_t remove_matching_words(uint index, char letter);

            // start new AC-3 algorithm call
            // i.e. add new blank layer to letters_at_indices.ac3_pruned_nodes and ac3_pruned_assigned_val
            void start_new_ac3_call();

            // undo previous AC-3 algorithm call
            // i.e. pop top layers of letters_at_indices.ac3_pruned_nodes and ac3_pruned_assigned_val, and restore to trie and assigned_value
            size_t undo_prev_ac3_call();

            // assign domain to a single value, repeat calls overwrite assigned value
            void assign_domain(word_t new_value) { assigned = true; assigned_value = new_value; }

            // unassign domain, restoring the preivous trie, used when all values fail in backtracking
            void unassign_domain() { assigned = false; assigned_value.reset(); }

            // get assigned status
            bool is_assigned() const { return assigned; }

            // get size of domain remaining, for ac3 validity checking
            size_t size() const;

            // get letters at an index, for AC-3 constraint satisfaction checking
            letter_bitset_t get_all_letters_at_index(uint index) const;

            // get all words in current domain to try to assign for backtracking
            vector<word_t> get_cur_domain() const;

            // expose letters_at_indicies for testing
            array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> get_letters_at_indices() const { assert(letters_at_indices); return *letters_at_indices; }

            // expose word_map for testing, undefined if domain assigned
            unordered_map<string, word_t>& get_word_map() { return word_map; }

            // copyable
            word_domain(const word_domain& other);
            word_domain& operator=(const word_domain& other);

            // movable
            word_domain(word_domain&& other) noexcept;
            word_domain& operator=(word_domain&& other) noexcept;

            // specialize std::swap for copy/move assignment
            void swap(word_domain& other) noexcept;

            // expose std::swap specialization for friend use
            friend void swap(word_domain& lhs, word_domain& rhs) noexcept;
        
        private:
            // opt file that this object may have read from
            optional<string> filepath_opt;

            // all trie nodes
            id_obj_manager<trie_node> nodes;

            // idx 0 of nodes id_obj_manager defined as root node
            static constexpr size_t TRIE_ROOT_NODE_IDX = 0ul;

            // map of all words to word structs (with heuristics) for O(1) validity checking & struct lookup for find_matches()
            // contents include all words ever added, even if pruned during an AC-3 call or this domain is assigned a value
            unordered_map<string, word_t> word_map;

            // number of words currently in domain, ignoring any assigned value
            size_t unassigned_domain_size;

            // stores # of words with letters at each index
            // contents undefined if domain assigned
            unique_ptr<array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> > letters_at_indices;

            // each layer corresponds to one call to AC-3 algorithm, and possibly contains a pruned assigned value 
            // top layer corresponds to most recent AC-3 call
            stack<optional<word_t> > ac3_pruned_assigned_val;

            // true iff domain has been assigned to a single value --> ignore trie
            bool assigned;

            // meaningful iff assigned true, if doesn't have value, then domain is empty
            optional<word_t> assigned_value;

            // helper for filepath constructor to detect file type
            static bool has_suffix(const string& str, const string& suffix);

            // helper for filepath constructor to check if word is legal
            optional<string> parse_word(const string& word);
    }; // word_domain

    // friend declarations
    void swap(word_domain& lhs, word_domain& rhs) noexcept;
}; // word_domain_ns

#endif // WORD_DOMAIN_H