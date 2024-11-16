// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structure implementations for cw_csp and cw_variable
// ==================================================================

#include "word_domain.h"

using namespace word_domain_ns;

/**
 * @brief constructor for word_domain with no filepath
 * 
 * @param name the name of this object
*/
word_domain::word_domain(string name) : word_domain(name, std::nullopt) {
    // do nothing, delegated to constructor does everything needed
}

/**
 * @brief constructor for word_domain with filepath
 * 
 * @param name the name of this object
 * @param filepath path to .txt or .json file containing word data
 * @param print_progress_bar displays progress bar iff true, default: false
*/
word_domain::word_domain(string name, string filepath, bool print_progress_bar)
        : word_domain(name, optional<string>(filepath), print_progress_bar) {
    // do nothing, delegated to constructor does everything needed
}

/**
 * @brief constructor for word_domain with filepath optional
 * 
 * @param name the name of this object
 * @param filepath_opt optional, may contain path to .txt or .json file containing word data
 * @param print_progress_bar displays progress bar iff true, default: false
*/
word_domain::word_domain(string name, optional<string> filepath_opt, bool print_progress_bar)
        : common_parent(name, VERBOSITY),
        filepath_opt(filepath_opt),
        unassigned_domain_size(0),
        letters_at_indices(make_unique<array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> >()),
        assigned(false) {
    // init root node of trie
    nodes.push_back(
        make_unique<trie_node>(TRIE_ROOT_NODE_IDX, false, '_', id_obj_manager<trie_node>::INVALID_ID)
    );

    // if filepath was provided
    if(filepath_opt.has_value()) {
        string filepath = filepath_opt.value();
        string word;
        optional<string> parsed_word;
        unique_ptr<progress_bar> bar = nullptr;

        if(has_suffix(filepath, ".txt")) {
            // open file
            ifstream word_file;
            word_file.open(filepath);
            assert_m(word_file.is_open(), "could not open txt file " + filepath);

            // count total number of lines in file for progress bar
            uint num_lines = 0;
            if(print_progress_bar) {
                while(getline(word_file, word)) num_lines++;
                word_file.clear(); // clear EOF flag
                word_file.seekg(0, std::ios::beg); // rewind file indicator
            }
            
            // create progress bar, now that denominator (num_lines) is known
            if(print_progress_bar) {
                bar = make_unique<progress_bar>(utils, num_lines, 0.01, PROGRESS_BAR_WIDTH, "Building", PROGRESS_BAR_SYMBOL_FULL, PROGRESS_BAR_SYMBOL_EMPTY);
            }

            // parse word file
            while(getline(word_file, word)) {
                // check for validity & convert uppercase, remove dashes, etc.
                parsed_word = parse_word(word);

                // add to word set & tree if valid and of valid size
                if(parsed_word.has_value()) {
                    word = parsed_word.value();
                    
                    if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN) {
                        add_word(word_t(word));
                    } 
                }

                // another word added
                if(bar) bar->incr_numerator();
            }
            word_file.close();

        } else if(has_suffix(filepath, ".json")) {
            // open word file, parse data
            ifstream word_file(filepath);
            assert_m(word_file.is_open(), "could not open json file " + filepath);
            json j = json::parse(word_file);
            
            // create progress bar, now that denominator (j.size()) is known
            if(print_progress_bar) {
                bar = make_unique<progress_bar>(utils, j.size(), 0.01, PROGRESS_BAR_WIDTH, "Building", PROGRESS_BAR_SYMBOL_FULL, PROGRESS_BAR_SYMBOL_EMPTY);
            }

            for(const auto& [item, data] : j.items()) {
                // incoming json is guarenteed to be clean, besides for word length (all lowercase and alphabetical)
                parsed_word = parse_word(item);
                assert_m(parsed_word.has_value() && parsed_word.value() == item, ".json file input word not clean: " + item);
                word = item;

                // add word
                if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN) {
                    add_word(word_t(word, data["Score"], data["Frequency"]));
                } 

                // another word added
                if(bar) bar->incr_numerator();
            }
            word_file.close();

        } else {
            utils.log(FATAL, "word_domain got file of invalid type: ", filepath);
            return;
        }
    }
}

/**
 * @brief constructor for word_domain with hashset of words in domain
 * @warning behavior undefined unless called to initialize a domain for a cw_variable
 * 
 * @param name the name of this object
 * @param domain set of words to add to domain, whose words must all be equal length
*/
word_domain::word_domain(string name, unordered_set<word_t>&& domain)
        : common_parent(name, VERBOSITY),
          filepath_opt(std::nullopt),
          unassigned_domain_size(0),
          letters_at_indices(make_unique<array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> >()),
          assigned(false) {
    // init root node of trie
    nodes.push_back(
        make_unique<trie_node>(TRIE_ROOT_NODE_IDX, false, '_', id_obj_manager<trie_node>::INVALID_ID)
    );

    size_t word_len = 0;
    for(word_t word : domain) {
        assert_m(word_len == 0 || word_len == word.word.size(), "word_domain set constructor includes words of unequal length");
        word_len = word.word.size();
        add_word(word);
    }
}

/**
 * @brief helper for filepath constructor to detect file type
 * 
 * @param str string to check the suffix of
 * @param suffix the suffix to check for
 * @return true iff str ends with suffix
*/
bool word_domain::has_suffix(const string& str, const string& suffix) {
    if(str.size() < suffix.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/**
 * @brief helper for filepath constructor to test validity for and parse words
 * 
 * @param word the word to test
 * @return optional containing word iff word only contains lowercase letters, uppercase letters, dashes, apostrophes, semicolons, numbers, spaces
*/
optional<string> word_domain::parse_word(const string& word) {
    stringstream word_ss;
    for(char c : word) {
        if(c >= 'a' && c <= 'z') {
            // valid lowercase letters, do nothing
            word_ss << c;
        } else if(c >= 'A' && c <= 'Z') {
            // valid uppercase letters, convert to lowercase
            word_ss << static_cast<char>(c + 'a' - 'A');
        } else if(c == '-' || c == '\'' || c == ' ' || c == ';' || (c >= '0' && c <= '9')) {
            // remove dashes/apostrophes/semicolons/numbers/spaces, do nothing
        } else {
            // invalid word, contains unknown character
            return std::nullopt;
        }
    }
    return std::make_optional<string>(word_ss.str());
}

/**
 * @brief adds word to word tree, updates letters_at_indices
 * 
 * @param word the word to add, duplicates do nothing
 * @note if domain assigned, this will overwrite the domain value
*/
void word_domain::add_word(word_t w) {
    if(assigned) {
        assigned_value = w;

        // letters_at_indices not updated since it's contents are undefined if assigned
    } else {
        if(word_map.count(w.word) == 0) {

            word_map.insert({w.word, w});
            unassigned_domain_size++;

            // update word counts in letters_at_indices
            for(uint i = 0; i < w.word.size(); ++i) {
                assert('a' <= w.word.at(i) && w.word.at(i) <= 'z');
                (*letters_at_indices)[i][static_cast<size_t>(w.word.at(i) - 'a')].num_words++;
            }

            /**
             * @brief private helper for add_word(), adds word to trie
             * 
             * @param node_idx idx in nodes id_obj_manager of node to branch off of, if applicable
             * @param word the word to add
             * @param pos index of next letter to add to tree
            */
            std::function<void(const size_t, const string&, uint pos)> add_word_to_trie;
            add_word_to_trie = [this, &add_word_to_trie](const size_t node_idx, const string& word, uint pos) {
                assert(node_idx < nodes.size());

                // all letters added to tree
                if(pos >= word.size()) {
                    // mark node as terminating valid word
                    nodes[node_idx]->valid = true;

                    // update word counts in lai_subsets
                    assert(update_lai_subsets<true, false>(node_idx) == word.size());
                    return;
                }

                // create child node if it doesn't exist yet
                if(!nodes[node_idx]->children.count(word.at(pos))) {
                    // create and add node to children of current node, entry in letters_at_indices, and nodes
                    nodes[node_idx]->children.insert({word.at(pos), nodes.size()});
                    (*letters_at_indices)[pos][static_cast<size_t>(word.at(pos) - 'a')].nodes.insert(nodes.size());
                    nodes.push_back(make_unique<trie_node>(
                        nodes.size(), false, word.at(pos), node_idx
                    ));
                } 

                // recurse to next letter
                add_word_to_trie(nodes[node_idx]->children.at(word.at(pos)), word, pos + 1);
            };

            add_word_to_trie(TRIE_ROOT_NODE_IDX, w.word, 0);
        }
    }
}

/**
 * @brief check if a string is a valid word
 * 
 * @param word string to check
 * @return true iff word is a valid word
 * @note behavior undefined if domain assigned, only intended to be called in cw_variable initialization
*/
bool word_domain::is_word(string& word) const {
    return word_map.count(word) > 0;
}

/**
 * @brief adds all words that match pattern with WILDCARD ('?') as placeholder
 * 
 * @param pattern the pattern to compare against
 * @note behavior undefined if domain assigned, only intended to be called in cw_variable initialization
*/
unordered_set<word_t> word_domain::find_matches(const string& pattern) const {
    /**
     * @brief private helper to find_matches() to recursively traverse tree to find matches
     * 
     * @param matches ptr to set to add matches to
     * @param pattern the pattern to compare against
     * @param pos index of next char in pattern to check
     * @param node_idx idx in id_obj_manager of current node traversing in word_tree
     * @param fragment part of word matched already
    */
    std::function<void(unordered_set<word_t>&, const string&, uint pos, const size_t, string)> traverse_to_find_matches;
    traverse_to_find_matches = [this, &traverse_to_find_matches](unordered_set<word_t>& matches, const string& pattern, uint pos, const size_t node_idx, string fragment) {
        utils.log(DEBUG, "entering traverse_to_find_matches() w/ pattern ", pattern, " at pos ", pos, " @ node ", nodes[node_idx]->letter);

        // pattern fully matched
        if(pos >= pattern.size()) {
            // AND this is a valid word
            utils.log(DEBUG, "pattern fully matched, valid check: ", nodes[node_idx]->valid);

            if(nodes[node_idx]->valid) { 
                matches.insert(word_map.at(fragment));
            }
            return;
        }

        if(pattern.at(pos) == WILDCARD) {
            // wildcard at this index, add all possible matches
            utils.log(DEBUG, "traversing for wild card");

            for(auto& pair : nodes[node_idx]->children) {
                traverse_to_find_matches(matches, pattern, pos + 1, pair.second, fragment + pair.first);
            }

        } else if(nodes[node_idx]->children.count(pattern.at(pos)) > 0) {
            // next letter progresses towards a valid word, continue
            utils.log(DEBUG, "traversing for letter ", pattern.at(pos));
            traverse_to_find_matches(matches, pattern, pos + 1, nodes[node_idx]->children[pattern.at(pos)], fragment + pattern.at(pos));

        } else {
            // this is a dead end, do nothing
        }
    };

    unordered_set<word_t> matches;
    traverse_to_find_matches(matches, pattern, 0, TRIE_ROOT_NODE_IDX, "");
    return matches;
}

/**
 * @brief checks number of words in trie with a specific letter at a specific index
 * 
 * @param index the index to check
 * @param letter the letter to check, 'a' <= letter <= 'z'
 * @returns letters_at_indices[index][letter].num_words
*/
uint word_domain::num_letters_at_index(uint index, char letter) const {
    assert(index < MAX_WORD_LEN);
    assert('a' <= letter && letter <= 'z');
    if(assigned) {
        return (assigned_value.has_value() && index < assigned_value.value().word.size() && assigned_value.value().word.at(index) == letter) ? 1u : 0u;
    }
    return (*letters_at_indices)[index][static_cast<size_t>(letter - 'a')].num_words;
}

/**
 * @brief checks which of the input letters at an input index exist in domain words among those also with a required letter at a required index
 * @pre not assigned -> index != required_index, equality doesn't make sense for intended use case of simple cycle constraints
 *
 * @param index index to search for letters along
 * @param letters 0-indexed bitset of letters to search for, i.e. index 0 == 'a'
 * @param required_index index at which required_letter must appear in words included in search
 * @param required_letter letter words included in search must have at index required_index
 *
 * @returns 0-indexed bitset where bit set --> contains word with that letter at specified index AND required_letter @ index of required_index
 * @note the above arrow is unidirectional, bit not set in input -> bit not set in output
*/
letter_bitset_t word_domain::has_letters_at_index_with_letter_assigned(uint index, const letter_bitset_t& letters, uint required_index, char required_letter) const {
    letter_bitset_t res;

    if(assigned) {
        if(assigned_value.has_value()) {
            assert(required_index < assigned_value.value().word.size());
            assert(index < assigned_value.value().word.size());

            if(assigned_value.value().word.at(required_index) == required_letter) {
                res |= 1 << static_cast<uint>(assigned_value.value().word.at(index) - 'a');
            }
        }
    } else {
        assert(index != required_index);

        auto& target = (*letters_at_indices)[required_index][static_cast<size_t>(required_letter - 'a')].lai_subset;
        for(size_t i = 0; i < NUM_ENGLISH_LETTERS; ++i) {
            if(letters[i] && (*target)[index][i] > 0) {
                res |= 1 << i;
            }
        }
    }

    return res;

    // letter_bitset_t res;

    // if(assigned) {
    //     if(assigned_value.has_value()) {
    //         assert(required_index < assigned_value.value().word.size());
    //         assert(index < assigned_value.value().word.size());

    //         if(assigned_value.value().word.at(required_index) == required_letter) {
    //             res |= 1 << static_cast<uint>(assigned_value.value().word.at(index) - 'a');
    //         }
    //     }
    // } else {
    //     assert(index != required_index);
        
    //     /**
    //      * @brief private helper
    //      * @pre cur_index > target_index
    //      *
    //      * @param node_idx node to traverse from
    //      * @param cur_index initial index of node_idx
    //      * @param target_index index of parent to read letter from
    //      * @returns letter of the parent node of node_idx at a index of target_index
    //     */
    //     auto char_of_parent = [this](size_t node_idx, uint cur_index, const uint target_index) -> char {
    //         assert(cur_index > 0 && cur_index < MAX_WORD_LEN);
    //         assert(target_index < MAX_WORD_LEN - 1);
    //         assert(cur_index > target_index);

    //         while(cur_index != target_index) {
    //             node_idx = nodes[node_idx]->parent;
    //             cur_index--;
    //         }
    //         return nodes[node_idx]->letter;
    //     };

    //     // traverse upwards from whichever is the higher depth to only have one path via parent instead of exponential growth of search paths
    //     if(index > required_index) {
    //         for(uint j = 0; j < NUM_ENGLISH_LETTERS; ++j) {
    //             if(letters[j]) {
    //                 for(const size_t node_idx : (*letters_at_indices)[index][j].nodes) {
    //                     if(char_of_parent(node_idx, index, required_index) == required_letter) {
    //                         res |= 1 << j;
    //                         break;
    //                     }
    //                 }
    //             }
    //         }
    //     } else {
    //         for(const size_t node_idx : (*letters_at_indices)[required_index][static_cast<size_t>(required_letter - 'a')].nodes) {
    //             res |= 1 << static_cast<uint>(char_of_parent(node_idx, required_index, index) - 'a');

    //             // early exit iff all letters found
    //             if(res == letters) break;
    //         }
    //     }
    // }

    // return res;
}

/**
 * @brief removes words with a specific letter at a specific index from trie
 * @warning behavior undefined if called in cw_variable initialization
 * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
 * 
 * @param index the index to remove in the word(s)
 * @param letter the letter to remove, 'a' <= letter <= 'z'
 * @returns total # number of words/leaf nodes removed
*/
size_t word_domain::remove_matching_words(uint index, char letter) {
    // check precondition and invariant
    size_t stack_depth = ac3_pruned_assigned_val.size();
    assert_m(stack_depth > 0, "ac3_pruned_assigned_val depth is 0 upon call to remove_matching_words()");
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            assert_m((*letters_at_indices)[i][j].ac3_pruned_nodes.size() == stack_depth, "stack depth invariant violated for ac3_pruned_nodes");
            assert_m((*letters_at_indices)[i][j].ac3_pruned_words.size() == stack_depth, "stack depth invariant violated for ac3_pruned_words");
        }
    }

    if(assigned) { // assigned value
        if(assigned_value.has_value() && assigned_value.value().word.at(index) == letter) {
            ac3_pruned_assigned_val.top() = optional<word_t>(assigned_value.value()); // prune, add to ac3 layer
            assigned_value.reset();

            // letters_at_indices not updated since it's contents are undefined if assigned
            // unassigned_domain_size also not updated since it is undefined if assigned
            return 1;
        } else {
            return 0;
        }
    } else { // regular case

        /**
         * @brief downwards private helper for remove_matching_words(), records and removes all children of this node recursively and updates letters_at_indices
         * @warning behavior undefined if called in cw_variable initialization
         * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
         * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
         * 
         * @param node_idx idx of current node whose children (not itself) will be removed
         * @param index depth of this parent node in trie or letter index in the word, root trie node defined as index/depth -1
         * @returns number of words/leaf nodes removed
        */
       std::function<uint(const size_t, uint)> remove_children;
       remove_children = [this, &remove_children](const size_t node_idx, uint index) -> uint {
            assert((*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].nodes.count(node_idx) > 0);
            (*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].ac3_pruned_nodes.top().insert(node_idx); // prune, add to ac3 layer
            (*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].nodes.erase(node_idx); 

            // base case for leaf nodes
            if(nodes[node_idx]->valid) {
                // terminates valid word, assumed to be a leaf node since all domain values in cw_variable are equal length
                assert(nodes[node_idx]->children.size() == 0);

                // update letters_at_indices word count values
                (*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].num_words--;
                (*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].ac3_pruned_words.top() += 1; // prune, add to ac3 layer

                // update lai_subset word count values
                assert(update_lai_subsets<false, true>(node_idx) == index + 1);

                // leaf node represents 1 word
                return 1u;
            }

            // recursive calls to children
            uint num_leafs = 0;
            for(const auto& pair : nodes[node_idx]->children) {
                num_leafs += remove_children(pair.second, index + 1);
            }

            // update num_words
            (*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].num_words -= num_leafs;
            (*letters_at_indices)[index][static_cast<size_t>(nodes[node_idx]->letter - 'a')].ac3_pruned_words.top() += num_leafs; // prune, add to ac3 layer

            // for details on why these links aren't saved, see the proof in word_domain_data_types.h
            nodes[node_idx]->children.clear();

            return num_leafs;
       };

        /**
         * @brief upwards private helper for remove_matching_words(), updates letters_at_indices and removes nodes without remaining valid leafs
         * @warning behavior undefined if called in cw_variable initialization
         * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
         * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
         * 
         * @param node_idx idx of this node which may be removed from its parent 
         * @param num_leafs number of valid words/leafs removed from the original call to remove_matching_words()
         * @param index depth of this node in trie or letter index in the word, root trie node defined as index/depth -1
         * @param letters_at_indices_updated true iff node is a node remove_children() was already called on, to avoid double updating
         * @note letters_at_indices_updated should only be true on the first recursive call for each node iterated on in remove_matching_words()
        */
        std::function<void(const size_t, uint&, int, bool)> remove_from_parents;
        remove_from_parents = [this, &remove_from_parents](const size_t node_idx, uint& num_leafs, int index, bool letters_at_indices_updated) {
            // check for base case of reached root node of trie
            const size_t parent = nodes[node_idx]->parent;
            if(parent != id_obj_manager<trie_node>::INVALID_ID) {

                // check if node has no valid leafs of its own and thus should be removed from parent
                if(nodes[node_idx]->children.size() == 0) {
                    // if this is the first recursive call, remove_children() already updated letters_at_indices for this node
                    if(!letters_at_indices_updated) {
                        (*letters_at_indices)[static_cast<size_t>(index)][static_cast<size_t>(nodes[node_idx]->letter - 'a')].ac3_pruned_nodes.top().insert(node_idx); // prune, add to ac3 layer
                        (*letters_at_indices)[static_cast<size_t>(index)][static_cast<size_t>(nodes[node_idx]->letter - 'a')].nodes.erase(node_idx);
                    }

                    // remove node as child from parent
                    // for details on why this link isn't saved, see the proof in word_domain_data_types.h
                    nodes[parent]->children.erase(nodes[node_idx]->letter);
                }

                // if this is the first recursive call, remove_children() already updated letters_at_indices for this node
                if(!letters_at_indices_updated) { 
                    (*letters_at_indices)[static_cast<size_t>(index)][static_cast<size_t>(nodes[node_idx]->letter - 'a')].num_words -= num_leafs;
                    (*letters_at_indices)[static_cast<size_t>(index)][static_cast<size_t>(nodes[node_idx]->letter - 'a')].ac3_pruned_words.top() += num_leafs; // prune, add to ac3 layer
                }

                // recurse upwards
                remove_from_parents(parent, num_leafs, index - 1, false);
            }
        };

        uint num_leafs;
        size_t total_leafs = 0;

        // need to make copy since removing from set being iterated on, should deallocate when out of scope
        unordered_set<size_t> nodes_copy = (*letters_at_indices)[index][static_cast<size_t>(letter - 'a')].nodes;
        for(size_t node_idx : nodes_copy) {
            if(nodes[node_idx]->parent != id_obj_manager<trie_node>::INVALID_ID) {
                // downwards removal in trie
                num_leafs = remove_children(node_idx, index);
                assert(num_leafs > 0);
                total_leafs += num_leafs;

                // upwards removal in trie
                remove_from_parents(node_idx, num_leafs, static_cast<int>(index), true);
            } else {
                utils.log(ERROR, "parent of node index ", index, ", letter ", letter, " deleted early");
            }
        }

        unassigned_domain_size -= total_leafs;
        return total_leafs;
    }
}

/**
 * @brief start new AC-3 call, during which remove_matching_words() may be called 0 or more times
 * @note causes new layer to be added to ac3_pruned_assigned_val, and ac3_pruned_nodes in each element of letters_at_indices
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
*/
void word_domain::start_new_ac3_call() {
    // check invariant
    size_t stack_depth = ac3_pruned_assigned_val.size();
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            assert_m((*letters_at_indices)[i][j].ac3_pruned_nodes.size() == stack_depth, "stack depth invariant violated for ac3_pruned_nodes");
            assert_m((*letters_at_indices)[i][j].ac3_pruned_words.size() == stack_depth, "stack depth invariant violated for ac3_pruned_words");
        }
    }

    ac3_pruned_assigned_val.push(std::nullopt);
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            (*letters_at_indices)[i][j].ac3_pruned_nodes.push(unordered_set<size_t>());
            (*letters_at_indices)[i][j].ac3_pruned_words.push(0);
        }
    }
}

/**
 * @brief undo previous AC-3 call, restoring the nodes to letters_at_indices and assigned_value
 * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes in each element of letters_at_indices must all be equal
 * @returns number of words/leaf nodes and assigned values restored
*/
size_t word_domain::undo_prev_ac3_call() {
    // check precondition and invariant
    size_t stack_depth = ac3_pruned_assigned_val.size();
    assert_m(stack_depth > 0, "ac3_pruned_assigned_val depth is 0 upon call to undo_prev_ac3_call()");
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            assert_m((*letters_at_indices)[i][j].ac3_pruned_nodes.size() == stack_depth, "stack depth invariant violated for ac3_pruned_nodes");
            assert_m((*letters_at_indices)[i][j].ac3_pruned_words.size() == stack_depth, "stack depth invariant violated for ac3_pruned_words");
        }
    }
    
    // restore regular letters_at_indices entries
    size_t num_restored = 0;
    size_t num_restored_per_index = 0;
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        num_restored_per_index = 0;
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            // restore nodes
            (*letters_at_indices)[i][j].nodes.insert(
                (*letters_at_indices)[i][j].ac3_pruned_nodes.top().begin(), (*letters_at_indices)[i][j].ac3_pruned_nodes.top().end()
            );
            // don't pop ac3_pruned_nodes yet, need it to restore links from parents

            // restore edges from parent nodes
            for(const size_t node_idx : (*letters_at_indices)[i][j].ac3_pruned_nodes.top()) {
                const size_t parent = nodes[node_idx]->parent;
                if(parent != id_obj_manager<trie_node>::INVALID_ID) {
                    assert_m(i == 0 || (*letters_at_indices)[i-1][static_cast<size_t>(nodes[parent]->letter - 'a')].nodes.count(parent) > 0, "parent node not yet restored in undo_prev_ac3_call() call");
                    assert_m(nodes[parent]->children.count(nodes[node_idx]->letter) == 0, "parent node still contains edge to child in undo_prev_ac3_call() call");
                    nodes[parent]->children.insert({nodes[node_idx]->letter, node_idx});
                } else {
                    utils.log(ERROR, "parent of node index ", i, ", letter ", j, " deleted early during restoration");
                }

                // update lai_subsets word counts
                if(nodes[node_idx]->valid) {
                    assert(nodes[node_idx]->children.size() == 0);
                    assert(update_lai_subsets<true, true>(node_idx) == i + 1);
                }
            }
            (*letters_at_indices)[i][j].ac3_pruned_nodes.pop();

            // restore num_words to update letters_at_indices word counts
            (*letters_at_indices)[i][j].num_words += (*letters_at_indices)[i][j].ac3_pruned_words.top();
            num_restored_per_index += (*letters_at_indices)[i][j].ac3_pruned_words.top();
            (*letters_at_indices)[i][j].ac3_pruned_words.pop();
        }
        assert_m(i == 0 || num_restored_per_index == 0 || num_restored == num_restored_per_index, "restoring unequal # of words per index in undo_prev_ac3_call() call");
        if(num_restored_per_index != 0) num_restored = num_restored_per_index;
    }
    if(num_restored > 0) assert_m(!assigned, "assigned upon restoring nonzero values in undo_prev_ac3_call() call");
    
    // restore assigned value
    optional<word_t> popped_assignment = ac3_pruned_assigned_val.top();
    ac3_pruned_assigned_val.pop();
    if(popped_assignment.has_value()) {
        assert_m(num_restored == 0, "restoring to letters_at_indices and assigned_value in undo_prev_ac3_call() call");
        assert_m(assigned, "not assigned upon restoring assigned value in undo_prev_ac3_call() call");
        assigned_value = popped_assignment.value();
        return 1; // return early to bypass updating unassigned_domain_size since it's undefined if assigned
    }

    unassigned_domain_size += num_restored;
    return num_restored;
}

/**
 * @brief get size of domain remaining for ac3 validity checking, whether or not domain is assigned
*/
size_t word_domain::size() const {
    if(assigned) {
        return assigned_value.has_value() ? 1l : 0l;
    }
    return unassigned_domain_size;
}

/**
 * @brief get letters at an index in the current domain, for AC-3 constraint satisfaction checking
 * @param index the index to get letters for
 * @returns bitset where true --> corresponding letter appears  at specified index in current domain (taking into account assignment) 
*/
letter_bitset_t word_domain::get_all_letters_at_index(uint index) const {
    assert_m(index < MAX_WORD_LEN, "index out of bounds of max word length");

    letter_bitset_t result;
    if(assigned) {
        if(assigned_value.has_value()) {
            assert_m(index < assigned_value.value().word.size(), "index out of bounds in letters_at_index() call");
            
            result |= 1 << static_cast<uint>(assigned_value.value().word.at(index) - 'a');
        }
    } else {
        for(uint i = 0; i < NUM_ENGLISH_LETTERS; ++i) {
            if(num_letters_at_index(index, static_cast<char>(i + 'a')) > 0) {
                result |= 1 << i;
            }
        }
    }
    
    return result;
}

/**
 * @brief get vector containing all words in the current domain
 * @returns unsorted vector of all words in the current domain
*/
vector<word_t> word_domain::get_cur_domain() const {
    if(assigned) {
        if(assigned_value.has_value()) return { assigned_value.value() };
        return {};
    }

    /**
     * @brief private helper for get_cur_domain(), traverses trie and adds words to accumulator
     * 
     * @param node_idx idx of current node in the traversal
     * @param fragment letters from traversal so far up to and not including the current node
     * @param acc accumulator vector to write back valid words to
    */
    std::function<void(const size_t, string, vector<word_t>&)> collect_cur_domain;
    collect_cur_domain = [this, &collect_cur_domain](const size_t node_idx, string fragment, vector<word_t>& acc) {
        string fragment_with_cur_node = (node_idx == TRIE_ROOT_NODE_IDX) ? fragment : fragment + nodes[node_idx]->letter;

        // base case for leaf nodes
        if(nodes[node_idx]->valid) {
            // terminates valid word, assumed to be a leaf node since all domain values in cw_variable are equal length
            assert(nodes[node_idx]->children.size() == 0);

            acc.push_back(word_map.at(fragment_with_cur_node));
            return;
        }

        // recursive calls to children
        for(const auto& pair : nodes[node_idx]->children) {
            collect_cur_domain(pair.second, fragment_with_cur_node, acc);
        }
    };

    vector<word_t> acc;
    collect_cur_domain(TRIE_ROOT_NODE_IDX, "", acc);
    return acc;
}

/**
 * @brief update values of lai_subset in letters_at_indices given addition/removal of a word from active domain
 * 
 * @param leaf idx of leaf node in id_obj_manager of word being added/removed
 * @param Add true --> word added, false --> word removed
 * @param AssumeFixedSizeWords set true if all domain values expected to be the same length, enables an assert statement
 * @return inferred length of word based on trie, for defensive programming purposes
*/
template <bool Add, bool AssumeFixedSizeWords>
size_t word_domain::update_lai_subsets(const size_t leaf) {
    assert(leaf != id_obj_manager<trie_node>::INVALID_ID);
    assert(nodes[leaf]->valid);
    if constexpr(AssumeFixedSizeWords) {
        assert(nodes[leaf]->children.size() == 0);
    }

    /**
     * @brief adds path to reach leaf in trie in forward order, including leaf itself and excluding root node
    */
    std::function<void(vector<size_t>&, const size_t)> get_leaf_path;
    get_leaf_path = [this, &get_leaf_path](vector<size_t>& path, const size_t cur) {
        if(cur != TRIE_ROOT_NODE_IDX) {
            assert(cur != id_obj_manager<trie_node>::INVALID_ID);
            get_leaf_path(path, nodes[cur]->parent);
            path.push_back(cur);
        }
    };

    // get path to leaf node
    vector<size_t> path;
    get_leaf_path(path, leaf);
    assert(path.size() >= MIN_WORD_LEN && path.size() <= MAX_WORD_LEN);
    
    // update lai_subset for each letter/index pair in path
    for(size_t i = 0; i < path.size(); ++i) {
        // shorthand
        const size_t i_idx = i;
        const size_t i_letter = static_cast<size_t>(nodes[path[i]]->letter - 'a');
        unique_ptr<array<array<uint, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> >& target = (*letters_at_indices)[i_idx][i_letter].lai_subset;

        // update entry in lai_subset for a letter/index pair
        for(size_t j = 0; j < path.size(); ++j) {
            // shorthand
            const size_t j_idx = j;
            const size_t j_letter = static_cast<size_t>(nodes[path[j]]->letter - 'a');

            if constexpr(Add) {
                (*target)[j_idx][j_letter]++;
            } else {
                assert((*target)[j_idx][j_letter] != 0);
                (*target)[j_idx][j_letter]--;
            }
        }
    }

    return path.size();
}

/**
 * @brief copy for word_domain
*/
word_domain::word_domain(const word_domain& other) 
    : common_parent(other.name, other.verbosity),
      filepath_opt(other.filepath_opt),
      nodes(other.nodes.clone()),
      word_map(other.word_map),
      unassigned_domain_size(other.unassigned_domain_size),
      letters_at_indices(other.letters_at_indices ? 
        make_unique<array<array<letters_table_entry, NUM_ENGLISH_LETTERS>, MAX_WORD_LEN> >(*other.letters_at_indices) : nullptr),
      ac3_pruned_assigned_val(other.ac3_pruned_assigned_val),
      assigned(other.assigned),
      assigned_value(other.assigned_value) {

    assert(letters_at_indices);
}
word_domain& word_domain::operator=(const word_domain& other) {
    if(this != &other) {
        word_domain temp(other);
        word_domain_ns::swap(*this, temp);
    }
    return *this;
}

/**
 * @brief move for word_domain
*/
word_domain::word_domain(word_domain&& other) noexcept 
    : common_parent(other.name, other.verbosity),
      filepath_opt(std::move(other.filepath_opt)),
      nodes(std::move(other.nodes)),
      word_map(std::move(other.word_map)),
      unassigned_domain_size(std::move(other.unassigned_domain_size)),
      letters_at_indices(std::move(other.letters_at_indices)),
      ac3_pruned_assigned_val(std::move(other.ac3_pruned_assigned_val)),
      assigned(other.assigned),
      assigned_value(std::move(other.assigned_value)) {
    // do nothing else
}
word_domain& word_domain::operator=(word_domain&& other) noexcept {
    word_domain temp(std::move(other));
    word_domain_ns::swap(*this, temp);
    return *this;
}

/**
 * @brief member std::swap specialization for word_domain
*/
void word_domain::swap(word_domain& other) noexcept {
    std::swap(utils, other.utils);
    std::swap(name, other.name);
    std::swap(verbosity, other.verbosity);
    std::swap(filepath_opt, other.filepath_opt);
    std::swap(nodes, other.nodes);
    std::swap(word_map, other.word_map);
    std::swap(unassigned_domain_size, other.unassigned_domain_size);
    std::swap(letters_at_indices, other.letters_at_indices);
    std::swap(ac3_pruned_assigned_val, other.ac3_pruned_assigned_val);
    std::swap(assigned, other.assigned);
    std::swap(assigned_value, other.assigned_value);
}

/**
 * @brief friend std::swap specialization for word_domain, calls member function
*/
void word_domain_ns::swap(word_domain& lhs, word_domain& rhs) noexcept {
    lhs.swap(rhs);
}
