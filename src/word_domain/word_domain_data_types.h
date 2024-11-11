// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: data types, functions for trie and related data structures
// ==================================================================

#ifndef WORD_DOMAIN_DATA_TYPES_H
#define WORD_DOMAIN_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../lib/src/json.hpp"
#include "../utils/cw_utils.h"

using namespace common_data_types_ns;
using json = nlohmann::json;

namespace word_domain_data_types_ns {

    // 0-indexed bitset of letters, i.e. index 0/LSB == 'a'
    using letter_bitset_t = std::bitset<NUM_ENGLISH_LETTERS>;

    /**
     * @brief node for word_domain
    */
    struct trie_node {
        size_t id; // for id_obj_manager
        bool valid; // true iff this node terminates a valid word
        char letter;

        // idx in id_obj_manager of parent, id_obj_manager::INVALID_ID if is root
        size_t parent;

        // maps next letters -> idx of child trie node in id_obj_manager
        unordered_map<char, size_t> children;
        
        // for explicitly copying unique_ptr to a node by copying underlying object
        unique_ptr<trie_node> clone() const;

        // constructor to initialize new words/head
        trie_node(size_t id, bool v, char l, size_t p) : id(id), valid(v), letter(l), parent(p) {}
    };

    /**
     * @brief entry in letters_at_indices
    */
    struct letters_table_entry {
        // number of words with a specific letter at a specific index
        uint num_words;

        // idx for all nodes that currently match this specific letter and index
        unordered_set<size_t> nodes;

        /**
         * for these two functions below, each layer corresponds to one call to AC-3 algorithm, top layer corresponds to most recent AC-3 call
         * 
         * @note why only ac3_pruned_nodes used to restore trie structure:
         * ---
         * after calls to remove_matching_words(), all the nodes removed from the trie are the nodes of 0 or more subtrees which 
         * are branches chopped off of the original trie. thus, if a node has been removed from the main trie, it is guaranteed 
         * that its connection, an edge from its parent node to itself, has been removed. vice versa, if a node's connection to its 
         * child node has been removed, by the description of the behavior of remove_matching_words(), it is guaranteed that the 
         * child node is no longer part of the trie. thus, the function from nodes removed from the trie to their parents from which 
         * their incoming edge has been removed is one-to-one. therefore it is sufficient to store only a set of removed nodes since the 
         * node datatype stores an idx to their parent which is not deleted in calls to remove_matching_words(). during the restore 
         * call, we can simply restore all the removed nodes in the top layer of ac3_pruned_nodes, and for each node, access their parent 
         * to restore the edge pointing to formerly removed node. 
         * 
         * note: overlap between the removed node set and parent node set is permitted iff all parent nodes (themselves removed or not) 
         * remove edges to all their removed children, which indeed is the case. also notice that under these conditions, all the removed 
         * nodes have no connections to one another
        */
        // contains nodes pruned during an AC-3 call
        stack<unordered_set<size_t> > ac3_pruned_nodes;
        // number of words pruned during an AC-3 call
        stack<uint> ac3_pruned_words;

        // base constructor
        letters_table_entry() : num_words(0u) {}
    };
}; // word_domain_data_types_ns

#endif // WORD_DOMAIN_DATA_TYPES_H