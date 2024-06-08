// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   12/28/2023
// Description: trie and related data structure implementations for cw_csp
// ==================================================================

#include "cw_trie.h"

using namespace cw_trie_ns;

/**
 * @brief constructor for cw_trie with no filepath
 * 
 * @param name the name of this object
*/
cw_trie::cw_trie(string name) : cw_trie(name, std::nullopt) {
    // do nothing, delegated to constructor does everything needed
}

/**
 * @brief constructor for cw_trie with filepath
 * 
 * @param name the name of this object
 * @param filepath path to .txt or .json file containing word data
 * @param print_progress_bar displays progress bar iff true, default: false
*/
cw_trie::cw_trie(string name, string filepath, bool print_progress_bar) : cw_trie(name, optional<string>(filepath), print_progress_bar) {
    // do nothing, delegated to constructor does everything needed
}

/**
 * @brief constructor for cw_trie with filepath optional
 * 
 * @param name the name of this object
 * @param filepath_opt optional, may contain path to .txt or .json file containing word data
 * @param print_progress_bar displays progress bar iff true, default: false
*/
cw_trie::cw_trie(string name, optional<string> filepath_opt, bool print_progress_bar) : common_parent(name) {
    trie = make_shared<trie_node>(false, '_', nullptr);
    this->filepath_opt = filepath_opt;
    unassigned_domain_size = 0;
    assigned = false;

    // if filepath was provided
    if(filepath_opt.has_value()) {
        string filepath = filepath_opt.value();
        string word;
        optional<string> parsed_word;

        // initialize progress bar fields
        unique_ptr<progress_bar> bar = nullptr;
        double prev_progress = 0.0;
        uint words_added = 0;
        if(print_progress_bar) {
            bar = make_unique<progress_bar>(cout, PROGRESS_BAR_WIDTH, "Building Dictionary", PROGRESS_BAR_SYMBOL_FULL, PROGRESS_BAR_SYMBOL_EMPTY);
        }
        
        // TODO: should .txt file support be removed?
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

                // update progress bar if 1% more of progress made
                if(print_progress_bar && (double)words_added/num_lines >= prev_progress + 0.01) {
                    prev_progress += 0.01;
                    bar->write(prev_progress);
                }

                // another word added
                words_added++;
            }
            word_file.close();

        } else if(has_suffix(filepath, ".json")) {
            // open word file, parse data
            ifstream word_file(filepath);
            assert_m(word_file.is_open(), "could not open json file " + filepath);
            json j = json::parse(word_file);

            for(const auto& [item, data] : j.items()) {
                // incoming json is guarenteed to be clean, besides for word length (all lowercase and alphabetical)
                parsed_word = parse_word(item);
                assert_m(parsed_word.has_value() && parsed_word.value() == item, ".json file input word not clean: " + item);
                word = item;

                // add word
                if(word.size() >= MIN_WORD_LEN && word.size() <= MAX_WORD_LEN) {
                    add_word(word_t(word, data["Score"], data["Frequency"]));
                } 

                // update progress bar if 1% more of progress made
                if(print_progress_bar && (double)words_added/j.size() >= prev_progress + 0.01) {
                    prev_progress += 0.01;
                    bar->write(prev_progress);
                }

                // another word added
                words_added++;
            }
            word_file.close();

        } else {
            ss << "cw_trie got file of invalid type: " << filepath;
            utils->print_msg(&ss, FATAL);
            return;
        }
    } 
}

/**
 * @brief constructor for cw_trie with hashset of words in domain
 * @warning behavior undefined unless called to initialize a domain for a cw_variable
 * 
 * @param name the name of this object
 * @param domain set of words to add to domain, whose words must all be equal length
*/
cw_trie::cw_trie(string name, unordered_set<word_t> domain) : common_parent(name) {
    trie = make_shared<trie_node>(false, '_', nullptr);
    filepath_opt = std::nullopt;
    unassigned_domain_size = 0;
    assigned = false;

    size_t word_len = 0;
    for(word_t word : domain) {
        assert_m(word_len == 0 || word_len == word.word.size(), "cw_trie set constructor includes words of unequal length");
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
bool cw_trie::has_suffix(const string& str, const string& suffix) {
    if(str.size() < suffix.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/**
 * @brief helper for filepath constructor to test validity for and parse words
 * 
 * @param word the word to test
 * @return optional containing word iff word only contains lowercase letters, uppercase letters, dashes, apostrophes, semicolons, numbers, spaces
*/
optional<string> cw_trie::parse_word(const string& word) {
    stringstream word_ss;
    for(char c : word) {
        if(c >= 'a' && c <= 'z') {
            // valid lowercase letters, do nothing
            word_ss << c;
        } else if(c >= 'A' && c <= 'Z') {
            // valid uppercase letters, convert to lowercase
            word_ss << (char)(c + 'a' - 'A'); // TODO: should this be static_cast<char> ?
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
void cw_trie::add_word(word_t w) {
    if(assigned) {
        assigned_value = w;

        // letters_at_indices not updated since it's contents are undefined if assigned
    } else {
        if(word_map.count(w.word) == 0) {

            word_map.insert({w.word, w});
            unassigned_domain_size++;

            // update word counts in letters_at_indices
            for(uint i = 0; i < w.word.size(); i++) {
                assert('a' <= w.word.at(i) && w.word.at(i) <= 'z');
                letters_at_indices[i][static_cast<size_t>(w.word.at(i) - 'a')].num_words++;
            }

            add_word_to_trie(trie, w.word, 0);

            ss << "num_words: " << endl;
            for(uint i = 0; i < MAX_WORD_LEN; i++) {
                for(char c = 'a'; c <= 'z'; c++) {
                    ss << letters_at_indices[i][static_cast<size_t>(c - 'a')].num_words << " ";
                }
                ss << endl;
            }
            utils->print_msg(&ss, DEBUG);

            ss << "num nodes: " << endl;
            for(uint i = 0; i < MAX_WORD_LEN; i++) {
                for(char c = 'a'; c <= 'z'; c++) {
                    ss << letters_at_indices[i][static_cast<size_t>(c - 'a')].nodes.size() << " ";
                }
                ss << endl;
            }
            utils->print_msg(&ss, DEBUG);
        }
    }
}

/**
 * @brief private helper for add_word(), adds word to trie
 * 
 * @param node ptr to node to branch off of, if applicable
 * @param word the word to add
 * @param pos index of next letter to add to tree
*/
void cw_trie::add_word_to_trie(shared_ptr<trie_node> node, string& word, uint pos) {
    assert(node);

    // all letters added to tree
    if(pos >= word.size()) {
        node->valid = true;
        return;
    }

    // create child node if it doesn't exist yet
    if(node->children.find(word.at(pos)) == node->children.end()) {
        node->children.insert({word.at(pos), make_shared<trie_node>(false, word.at(pos), node)});

        // update nodes in letters_at_indices
        letters_at_indices[pos][static_cast<size_t>(word.at(pos) - 'a')].nodes.insert(node->children.at(word.at(pos)));
    } 

    // recurse to next letter
    add_word_to_trie(node->children.at(word.at(pos)), word, pos + 1);
}

/**
 * @brief check if a string is a valid word
 * 
 * @param word string to check
 * @return true iff word is a valid word
 * @note behavior undefined if domain assigned, only intended to be called in cw_variable initialization
*/
bool cw_trie::is_word(string& word) const {
    return word_map.count(word) > 0;
}

/**
 * @brief adds all words that match pattern with WILDCARD ('?') as placeholder
 * 
 * @param pattern the pattern to compare against
 * @note behavior undefined if domain assigned, only intended to be called in cw_variable initialization
*/
unordered_set<word_t> cw_trie::find_matches(const string& pattern) {
    unordered_set<word_t> matches;
    traverse_to_find_matches(matches, pattern, 0, trie, "");
    return matches;
}

/**
 * @brief helper to find_matches() to recursively traverse tree to find matches
 * 
 * @param matches ptr to set to add matches to
 * @param pattern the pattern to compare against
 * @param pos index of next char in pattern to check
 * @param node current node traversing in word_tree
 * @param fragment part of word matched already
*/
void cw_trie::traverse_to_find_matches(unordered_set<word_t>& matches, const string& pattern, uint pos, shared_ptr<trie_node> node, string fragment) {
    ss << "entering traverse_to_find_matches() w/ pattern " << pattern << " at pos " << pos 
       << " @ node " << node->letter;
    utils->print_msg(&ss, DEBUG);

    // pattern fully matched
    if(pos >= pattern.size()) {
        // AND this is a valid word
        ss << "pattern fully matched, valid check: " << node->valid;
        utils->print_msg(&ss, DEBUG);

        if(node->valid) { 
            matches.insert(word_map.at(fragment));
        }
        return;
    }

    if(pattern.at(pos) == WILDCARD) {
        // wildcard at this index, add all possible matches
        ss << "traversing for wild card";
        utils->print_msg(&ss, DEBUG);

        for(auto& pair : node->children) {
            traverse_to_find_matches(matches, pattern, pos + 1, pair.second, fragment + pair.first);
        }

    } else if(node->children.count(pattern.at(pos)) > 0) {
        // next letter progresses towards a valid word, continue
        ss << "traversing for letter " << pattern.at(pos);
        utils->print_msg(&ss, DEBUG);
        traverse_to_find_matches(matches, pattern, pos + 1, node->children[pattern.at(pos)], fragment + pattern.at(pos));

    } else {
        // this is a dead end, do nothing
    }
}

/**
 * @brief checks number of words in trie with a specific letter at a specific index
 * 
 * @param index the index to check
 * @param letter the letter to check, 'a' <= letter <= 'z'
 * @returns letters_at_indices[index][letter].num_words
*/
uint cw_trie::num_letters_at_index(uint index, char letter) const {
    assert('a' <= letter && letter <= 'z');
    if(assigned) {
        return (assigned_value.has_value() && index < assigned_value.value().word.size() && assigned_value.value().word.at(index) == letter) ? 1l : 0l;
    }
    return letters_at_indices[index][static_cast<size_t>(letter - 'a')].num_words;
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
size_t cw_trie::remove_matching_words(uint index, char letter) {
    // check precondition and invariant
    size_t stack_depth = ac3_pruned_assigned_val.size();
    assert_m(stack_depth > 0, "ac3_pruned_assigned_val depth is 0 upon call to remove_matching_words()");
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            assert_m(letters_at_indices[i][j].ac3_pruned_nodes.size() == stack_depth, "stack depth invariant violated for ac3_pruned_nodes");
            assert_m(letters_at_indices[i][j].ac3_pruned_words.size() == stack_depth, "stack depth invariant violated for ac3_pruned_words");
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
        uint num_leafs;
        size_t total_leafs = 0;

        // need to make copy since removing from set being iterated on, should deallocate when out of scope
        unordered_set<shared_ptr<trie_node> > nodes_copy = letters_at_indices[index][static_cast<size_t>(letter - 'a')].nodes;
        for(shared_ptr<trie_node> node : nodes_copy) {
            if(shared_ptr<trie_node> parent = node->parent.lock()) {
                // downwards removal in trie
                num_leafs = remove_children(node, index);
                total_leafs += num_leafs;

                // upwards removal in trie
                remove_from_parents(node, num_leafs, static_cast<int>(index), true);
            } else {
                ss << "parent of node index " << index << ", letter " << letter << " deleted early";
                utils->print_msg(&ss, ERROR);
            }
        }

        unassigned_domain_size -= total_leafs;
        return total_leafs;
    }
}

/**
 * @brief upwards helper for remove_matching_words(), updates letters_at_indices and removes nodes without remaining valid leafs
 * @warning behavior undefined if called in cw_variable initialization
 * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
 * 
 * @param node this node which may be removed from its parent 
 * @param num_leafs number of valid words/leafs removed from the original call to remove_matching_words()
 * @param index depth of this node in trie or letter index in the word, root trie node defined as index/depth -1
 * @param letters_at_indices_updated true iff node is a node remove_children() was already called on, to avoid double updating
 * @note letters_at_indices_updated should only be true on the first recursive call for each node iterated on in remove_matching_words()
*/
void cw_trie::remove_from_parents(shared_ptr<trie_node> node, uint& num_leafs, int index, bool letters_at_indices_updated) {

    // check for base case of reached root node of trie
    if(shared_ptr<trie_node> parent = node->parent.lock()) {

        // check if node has no valid leafs of its own and thus should be removed from parent
        if(node->children.size() == 0) {
            // if this is the first recursive call, remove_children() already updated letters_at_indices for this node
            if(!letters_at_indices_updated) {
                letters_at_indices[static_cast<size_t>(index)][static_cast<size_t>(node->letter - 'a')].ac3_pruned_nodes.top().insert(node); // prune, add to ac3 layer
                letters_at_indices[static_cast<size_t>(index)][static_cast<size_t>(node->letter - 'a')].nodes.erase(node);
            }

            // remove node as child from parent
            // for details on why this link isn't saved, see the proof in cw_trie_data_types.h
            parent->children.erase(node->letter);
        }

        // if this is the first recursive call, remove_children() already updated letters_at_indices for this node
        if(!letters_at_indices_updated) { 
            letters_at_indices[static_cast<size_t>(index)][static_cast<size_t>(node->letter - 'a')].num_words -= num_leafs;
            letters_at_indices[static_cast<size_t>(index)][static_cast<size_t>(node->letter - 'a')].ac3_pruned_words.top() += num_leafs; // prune, add to ac3 layer
        }

        // recurse upwards
        remove_from_parents(parent, num_leafs, index - 1, false);
    }
}

/**
 * @brief downwards helper for remove_matching_words(), records and removes all children of this node recursively and updates letters_at_indices
 * @warning behavior undefined if called in cw_variable initialization
 * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
 * 
 * @param node current node whose children (not itself) will be removed
 * @param index depth of this parent node in trie or letter index in the word
 * @returns number of words/leaf nodes removed
*/
uint cw_trie::remove_children(shared_ptr<trie_node> node, uint index) {
    assert(letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].nodes.count(node) > 0);
    letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].ac3_pruned_nodes.top().insert(node); // prune, add to ac3 layer
    letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].nodes.erase(node); 

    // base case for leaf nodes
    if(node->valid) {
        // terminates valid word, assumed to be a leaf node since all domain values in cw_variable are equal length
        assert(node->children.size() == 0);

        letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].num_words--;
        letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].ac3_pruned_words.top() += 1; // prune, add to ac3 layer
        return 1;
    }

    // recursive calls to children
    uint num_leafs = 0;
    for(const auto& pair : node->children) {
        num_leafs += remove_children(pair.second, index + 1);
    }

    // update num_words
    letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].num_words -= num_leafs;
    letters_at_indices[index][static_cast<size_t>(node->letter - 'a')].ac3_pruned_words.top() += num_leafs; // prune, add to ac3 layer

    // for details on why these links aren't saved, see the proof in cw_trie_data_types.h
    node->children.clear();

    return num_leafs;
}

/**
 * @brief start new AC-3 call, during which remove_matching_words() may be called 0 or more times
 * @note causes new layer to be added to ac3_pruned_assigned_val, and ac3_pruned_nodes in each element of letters_at_indices
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes/ac3_pruned_words in each element of letters_at_indices must all be equal
*/
void cw_trie::start_new_ac3_call() {
    // check invariant
    size_t stack_depth = ac3_pruned_assigned_val.size();
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            assert_m(letters_at_indices[i][j].ac3_pruned_nodes.size() == stack_depth, "stack depth invariant violated");
            assert_m(letters_at_indices[i][j].ac3_pruned_words.size() == stack_depth, "stack depth invariant violated for ac3_pruned_words");
        }
    }

    ac3_pruned_assigned_val.push(std::nullopt);
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            letters_at_indices[i][j].ac3_pruned_nodes.push(unordered_set<shared_ptr<trie_node> >());
            letters_at_indices[i][j].ac3_pruned_words.push(0);
        }
    }
}

/**
 * @brief undo previous AC-3 call, restoring the nodes to letters_at_indices and assigned_value
 * @pre prior to this, start_new_ac3_call() must have been called more times than the # of prior calls to undo_prev_ac3_call()
 * @invariant # of layers in ac3_pruned_assigned_val and ac3_pruned_nodes in each element of letters_at_indices must all be equal
 * @returns number of words/leaf nodes and assigned values restored
*/
size_t cw_trie::undo_prev_ac3_call() {
    // check precondition and invariant
    size_t stack_depth = ac3_pruned_assigned_val.size();
    assert_m(stack_depth > 0, "ac3_pruned_assigned_val depth is 0 upon call to undo_prev_ac3_call()");
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            assert_m(letters_at_indices[i][j].ac3_pruned_nodes.size() == stack_depth, "stack depth invariant violated");
            assert_m(letters_at_indices[i][j].ac3_pruned_words.size() == stack_depth, "stack depth invariant violated for ac3_pruned_words");
        }
    }
    
    // restore regular letters_at_indices entries
    size_t num_restored = 0;
    size_t num_restored_per_index = 0;
    for(uint i = 0; i < MAX_WORD_LEN; i++) {
        num_restored_per_index = 0;
        for(uint j = 0; j < NUM_ENGLISH_LETTERS; j++) {
            // restore nodes
            letters_at_indices[i][j].nodes.insert(
                letters_at_indices[i][j].ac3_pruned_nodes.top().begin(), letters_at_indices[i][j].ac3_pruned_nodes.top().end()
            ); 
            // don't pop ac3_pruned_nodes yet, need it to restore links from parents

            // restore edges from parent nodes
            for(shared_ptr<trie_node> node : letters_at_indices[i][j].ac3_pruned_nodes.top()) {
                if(shared_ptr<trie_node> parent = node->parent.lock()) {
                    assert_m(i == 0 || letters_at_indices[i-1][static_cast<size_t>(parent->letter - 'a')].nodes.count(parent) > 0, "parent node not yet restored in undo_prev_ac3_call() call");
                    assert_m(parent->children.count(node->letter) == 0, "parent node still contains edge to child in undo_prev_ac3_call() call");
                    parent->children.insert({node->letter, node});
                } else {
                    ss << "parent of node index " << i << ", letter " << j << " deleted early during restoration";
                    utils->print_msg(&ss, ERROR);
                }
            }
            letters_at_indices[i][j].ac3_pruned_nodes.pop();

            // restore num_words
            letters_at_indices[i][j].num_words += letters_at_indices[i][j].ac3_pruned_words.top();
            num_restored_per_index += letters_at_indices[i][j].ac3_pruned_words.top();
            letters_at_indices[i][j].ac3_pruned_words.pop();
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
size_t cw_trie::domain_size() const {
    if(assigned) {
        return assigned_value.has_value() ? 1l : 0l;
    }
    return unassigned_domain_size;
}

/**
 * @brief get letters at an index in the current domain, for AC-3 constraint satisfaction checking
 * @param index the index to get letters for
 * @returns set of chars, >= 'a', and <= 'z', which appear at the specific index in the current domain (taking into account assignment) 
*/
unordered_set<char> cw_trie::get_all_letters_at_index(uint index) const {
    assert_m(index < MAX_WORD_LEN, "index out of bounds of max word length");

    unordered_set<char> result;
    if(assigned) {
        if(assigned_value.has_value()) {
            assert_m(index < assigned_value.value().word.size(), "index out of bounds in letters_at_index() call");
            
            result = { assigned_value.value().word.at(index) };
        }
    } else {
        for(char letter = 'a'; letter <= 'z'; letter++) {
            if(num_letters_at_index(index, letter) > 0) {
                result.insert(letter);
            }
        }
    } 
    
    return result;
}

/**
 * @brief get vector containing all words in the current domain
 * @returns unsorted vector of all words in the current domain
*/
vector<word_t> cw_trie::get_cur_domain() {
    vector<word_t> acc;
    collect_cur_domain(trie, "", acc);
    return acc;
}

/**
 * @brief helper for get_cur_domain(), traverses trie and adds words to accumulator
 * 
 * @param node current node in the traversal
 * @param fragment letters from traversal so far up to and not including the current node
 * @param acc accumulator vector to write back valid words to
*/
void cw_trie::collect_cur_domain(shared_ptr<trie_node> node, string fragment, vector<word_t>& acc) {
    string fragment_with_cur_node = (node == trie) ? fragment : fragment + node->letter;

    // base case for leaf nodes
    if(node->valid) {
        // terminates valid word, assumed to be a leaf node since all domain values in cw_variable are equal length
        assert(node->children.size() == 0);

        acc.push_back(word_map.at(fragment_with_cur_node));
        return;
    }

    // recursive calls to children
    for(const auto& pair : node->children) {
        collect_cur_domain(pair.second, fragment_with_cur_node, acc);
    }
}