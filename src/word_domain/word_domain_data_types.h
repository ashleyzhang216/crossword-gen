// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: data types, functions for trie and related data structures
// ==================================================================

#ifndef WORD_DOMAIN_DATA_TYPES_H
#define WORD_DOMAIN_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../lib/src/json.hpp"

using namespace common_data_types_ns;
using json = nlohmann::json;

namespace word_domain_data_types_ns {
    /**
     * @brief node for word_domain
    */
    struct trie_node {
        bool valid; // true iff this node terminates a valid word
        char letter;

        // ptr to parent, nullptr if is_root
        weak_ptr<trie_node> parent;

        // ptr to child words
        unordered_map<char, shared_ptr<trie_node> > children;

        // constructor to initialize new words/head
        trie_node(bool v, char l, shared_ptr<trie_node> p) : valid(v), letter(l), parent(p) {}
    };

    /**
     * @brief entry in letters_at_indices
    */
    struct letters_table_entry {
        // number of words with a specific letter at a specific index
        uint num_words;

        // all nodes that match this specific letter and index
        // uses shared_ptr to support easier undos of calls to remove_matching_words() in the future 
        // after remove_matching_words() is called, ptrs to nodes are still kept here as a backup and can be restored if needed
        unordered_set<shared_ptr<trie_node> > nodes;

        /**
         * for these two functions below, each layer corresponds to one call to AC-3 algorithm, top layer corresponds to most recent AC-3 call
         * 
         * @note why only ac3_pruned_nodes used to restore trie structure:
         * ---
         * after calls to remove_matching_words(), all the nodes removed from the trie are the nodes of 0 or more trees which 
         * are branches chopped off of the original trie. thus, if a node has been removed from the main trie, it is guaranteed 
         * that its connection, an edge from its parent node to itself, has been removed. vice versa, if a node's connection to its 
         * child node has been removed, by the description of the behavior of remove_matching_words(), it is guaranteed that the 
         * child node is no longer part of the trie. thus, the function from nodes removed from the trie to their parents from which 
         * their incoming edge has been removed is one-to-one. therefore it is sufficient to store only a set of removed nodes since the 
         * node datatype stores a weak_ptr to their parent which is not deleted in calls to remove_matching_words(). during the restore 
         * call, we can simply restore all the removed nodes in the top layer of ac3_pruned_nodes, and for each node, access their parent 
         * to restore the edge pointing to formerly removed node. 
         * 
         * note: overlap between the removed node set and parent node set is permitted iff all parent nodes (themselves removed or not) 
         * remove edges to all their removed children, which indeed is the case. also notice that under these conditions, all the removed 
         * nodes have no connections to one another
        */
        // contains nodes pruned during an AC-3 call
        stack<unordered_set<shared_ptr<trie_node> > > ac3_pruned_nodes; 
        // # of words pruned during an AC-3 call
        stack<size_t> ac3_pruned_words; 

        // base constructor
        letters_table_entry() : num_words(0) {}
    };
}; // word_domain_data_types_ns

#endif // WORD_DOMAIN_DATA_TYPES_H