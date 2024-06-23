// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module declaration
// ==================================================================

#ifndef CW_UTILS_H
#define CW_UTILS_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

// verbosity levels
enum verbosity_t {
    FATAL = 0,
    ERROR = 1,
    WARNING = 2, 
    INFO = 3,
    DEBUG = 4
};

// mapping from verbosity value to name
extern unordered_map<verbosity_t, string> verbosity_type_to_name;

// global verbosity
extern verbosity_t VERBOSITY;

// class cw_utils {
//     public:
//         string name;
//         verbosity_t min_verbosity;

//         // base constructor
//         cw_utils(string name, verbosity_t verbosity);

//         // unused destructor
//         ~cw_utils(){};

//         // general message print
//         bool print_msg(string s, verbosity_t verbosity = INFO);
//         bool print_msg(stringstream *s, verbosity_t verbosity = INFO);
// };

/**
 * @brief object to handle message logging, and other functionalities in the future
*/
class cw_utils {
    public:
        // base constructor
        cw_utils(const string_view& name, const verbosity_t& min_verbosity);

        // try to log
        template <typename... Types>
        void log(const verbosity_t& verbosity, const Types&... args) const;

    private:
        const string name;
        const verbosity_t min_verbosity;
        
        /**
         * @brief allowed types for printing
         * @note the types are restricted so that with -O3 optimizations enabled, the arguments 
         *       don't have to be actually loaded from memory if the verbosity check fails. with 
         *       this optimization, the assembly for each call to log() is a compare instruction 
         *       followed by a conditional jump. since these arguments have no external effect, 
         *       they can safely be only loaded after the jump if it occurs, meaning that in 
         *       prod, a call to log() only costs 2 instructions of no op 
        */
        template <typename T>
        static constexpr bool is_printable = 
            std::disjunction_v<
                std::is_convertible<T, string_view>,
                std::is_arithmetic<T>
            >;
        
        // print to std::cout after verbosity check passed
        template <class T, typename = std::enable_if_t<is_printable<T>>, typename... Types>
        void log_std_cout(const T& msg, const Types&... args) const;

        // print to std::cerr after verbosity check passed
        template <class T, typename = std::enable_if_t<is_printable<T>>, typename... Types>
        void log_std_cerr(const T& msg, const Types&... args) const;
};

struct assertion_failure_exception : public exception {
    const char *what() const throw() {
        return "crossword-gen failed assertion";
    }
};

#undef assert
#define assert(x)                                                                                       \
    if(!(x)) {                                                                                          \
        cout << "\nAssertion failed\n" << "File: " << __FILE__ << ": " << std::dec << __LINE__ << endl; \
        throw assertion_failure_exception();                                                            \
    }                                                                                                   \

#undef assert_m
#define assert_m(x, msg)                                                                                \
    if(!(x)) {                                                                                          \
        cout << "\nAssertion failed\n" << "File: " << __FILE__ << ": " << std::dec << __LINE__ << endl  \
            << msg << endl;                                                                             \
        throw assertion_failure_exception();                                                            \
    } 

/**
 * @brief template function to compare contents of hashsets for testing, T must have << operator defined
 * TODO: switch to using shared_ptr or refs
 * 
 * @param lhs ptr to lhs set
 * @param rhs ptr to rhs set
 * @param debug_prints prints debug messages iff debug_prints == true
 * @returns true iff contents of lhs & rhs are identical
*/
template <typename T> 
inline bool set_contents_equal(const unordered_set<T>* lhs, const unordered_set<T>* rhs, bool debug_prints) {

    stringstream ss;
    cw_utils* utils = new cw_utils("set_contents_equal()", VERBOSITY);
    bool result = true;

    if(lhs->size() != rhs->size()) {
        if(debug_prints) {
            ss << "mismatched size: " << lhs->size() << " & " << rhs->size();
            utils->print_msg(&ss, WARNING);
        }
        result = false;
    }

    for(const T& t : *lhs) {
        if(rhs->count(t) == 0) {
            if(debug_prints) {
                ss << "missing from rhs: " << t;
                utils->print_msg(&ss, WARNING);
            }
            result = false;
        }
    }

    // not necessary for correctivity checking, only for debug
    for(const T& t : *rhs) {
        if(lhs->count(t) == 0) {
            if(debug_prints) {
                ss << "missing from lhs: " << t;
                utils->print_msg(&ss, WARNING);
            }
            result = false;
        }
    }

    return result;
}

/**
 * @brief template function to compare contents of hashmap to hashset for testing
 * 
 * @param lhs ptr to lhs map
 * @param rhs ptr to rhs map
 * @param debug_prints prints debug messages iff debug_prints == true
 * @returns true iff contents of lhs & rhs are identical
*/
template <typename K, typename V>
inline bool map_to_set_contents_equal(const unordered_map<K, unordered_set<V> >* lhs, const unordered_map<K, unordered_set<V> >* rhs, bool debug_prints) {
    
    stringstream ss;
    cw_utils* utils = new cw_utils("map_to_set_contents_equal()", VERBOSITY);
    bool result = true;

    if(lhs->size() != rhs->size()) {
        if(debug_prints) {
            ss << "mismatched size: " << lhs->size() << " & " << rhs->size();
            utils->print_msg(&ss, WARNING);
        }
        result = false;
    }
    
    for(const auto& pair : *lhs) {
        if(rhs->find(pair.first) == rhs->end()) {
            if(debug_prints) {
                ss << "key missing from rhs: " << pair.first;
                utils->print_msg(&ss, WARNING);
            }
            result = false;
            continue;
        }
        if(!set_contents_equal(&(rhs->at(pair.first)), &(pair.second), debug_prints)) {
            if(debug_prints) {
                ss << "set contents for key " << pair.first << " not equal";
                utils->print_msg(&ss, WARNING);
            }
            result = false;
        }
    }

    return result;
}

#endif // CW_UTILS_H