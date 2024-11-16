// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: data type, function implementations for trie and related data structures
// ==================================================================

#include "word_domain_data_types.h"

using namespace word_domain_data_types_ns;

/**
 * @brief explicitly copy underlying node object
 * 
 * @return ptr to deep copy of node
*/
unique_ptr<trie_node> trie_node::clone() const { 
    return make_unique<trie_node>(*this);
}

/**
 * @brief copy constructor for letters_table_entry
*/
letters_table_entry::letters_table_entry(const letters_table_entry& other) 
    : num_words(other.num_words),
      nodes(other.nodes),
      ac3_pruned_nodes(other.ac3_pruned_nodes),
      ac3_pruned_words(other.ac3_pruned_words),
      lai_subset(make_unique<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN>>(*other.lai_subset)) {
    // do nothing else
}

/**
 * @brief copy assignment for letters_table_entry
*/
letters_table_entry& letters_table_entry::operator=(const letters_table_entry& other) {
    if(this != &other) {
        num_words = other.num_words;
        nodes = other.nodes;
        ac3_pruned_nodes = other.ac3_pruned_nodes;
        ac3_pruned_words = other.ac3_pruned_words;
        lai_subset = make_unique<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN>>(*other.lai_subset);
    }
    return *this;
}

/**
 * @brief move constructor for letters_table_entry
*/
letters_table_entry::letters_table_entry(letters_table_entry&& other) noexcept 
    : num_words(other.num_words),
      nodes(std::move(other.nodes)),
      ac3_pruned_nodes(std::move(other.ac3_pruned_nodes)),
      ac3_pruned_words(std::move(other.ac3_pruned_words)),
      lai_subset(std::move(other.lai_subset)) {
    // do nothing else
}

/**
 * @brief move assignment for letters_table_entry
*/
letters_table_entry& letters_table_entry::operator=(letters_table_entry&& other) noexcept {
    letters_table_entry temp(std::move(other));

    std::swap(num_words, temp.num_words);
    std::swap(nodes, temp.nodes);
    std::swap(ac3_pruned_nodes, temp.ac3_pruned_nodes);
    std::swap(ac3_pruned_words, temp.ac3_pruned_words);
    std::swap(lai_subset, temp.lai_subset);

    return *this;
}
