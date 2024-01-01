// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal common data types
// ==================================================================

#ifndef COMMON_DATA_TYPES_H
#define COMMON_DATA_TYPES_H

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <array>

#define MIN_WORD_LEN 2         // max length for a single word
#define MAX_WORD_LEN 20        // max length for a single word
#define WILDCARD '?'           // tile/pattern where any letter can go
#define BLACK ' '              // tile where no letter can be placed
#define WCD WILDCARD           // shorthand
#define BLK BLACK              // shorthand
#define NUM_ENGLISH_LETTERS 26 // this shouldn't ever change, knock on wood :)

using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::string;
using std::stringstream;
using std::vector;
using std::queue;
using std::stack;
using std::tuple;
using std::ifstream;
using std::ostream;
using std::unordered_set;
using std::unordered_map;
using std::hash;
using std::array;

// RAII
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::make_shared;
using std::make_unique;

namespace common_data_types_ns {
    /**
     * @brief struct to represent possible cw words & extra heuristics for prioritization
    */
    struct word_t {
        // contents of this word
        string word;

        // # of times word appears, if provided
        long freq;

        // word score from curated crossword lists, if provided
        int score;

        // optional word hint/description to assist with writing clues, if provided
        string descr; 

        // basic constructors
        word_t(string w)                          : word(w), freq(0), score(0), descr("") {}
        word_t(string w, int s)                   : word(w), freq(0), score(s), descr("") {}
        word_t(string w, int s, long f)           : word(w), freq(f), score(s), descr("") {}
        word_t(string w, int s, long f, string d) : word(w), freq(f), score(s), descr(d)  {}

        // for printing
        friend ostream& operator<<(ostream& os, const word_t& cw);

        // equality operators
        bool operator==(const word_t& rhs) const { return word == rhs.word; }
        bool operator!=(const word_t& rhs) const { return word != rhs.word; }
    };

    /**
     * @brief progress bar to print cw search progress
     * implementation largely borrowed from https://codereview.stackexchange.com/a/186537
    */
    class progress_bar {
        public:
            // basic constructor, starts printing bar
            progress_bar(ostream& os, size_t line_width, string msg, const char symbol_full, const char symbol_empty);

            // write fraction, 0 <= fraction <= 1.0
            void write(double fraction);

            // not copyable
            progress_bar(const progress_bar&) = delete;
            progress_bar& operator=(const progress_bar&) = delete;

            // destructor, terminates bar and prints newline
            ~progress_bar();
        private:
            ostream& os;
            size_t bar_width;
            string msg;
            string full_bar;
    };
    
} // common_data_types_ns

/**
 * hash function declarations in global scope
*/
template <>
struct hash<common_data_types_ns::word_t> {
    size_t operator()(const common_data_types_ns::word_t& w) const;
};

#endif // COMMON_DATA_TYPES_H