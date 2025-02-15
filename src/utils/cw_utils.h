// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module declaration
// ==================================================================

#ifndef CW_UTILS_H
#define CW_UTILS_H

#include "../common/common_data_types.h"
#include "cw_assert.hpp"

using namespace common_data_types_ns;

// verbosity levels
enum verbosity_t {
    FATAL   = 0,
    ERROR   = 1,
    WARNING = 2, 
    INFO    = 3,
    DEBUG   = 4
};

// mapping from verbosity value to name
extern unordered_map<verbosity_t, string> verbosity_type_to_name;

// global verbosity
extern verbosity_t VERBOSITY;

namespace cw {
    /**
     * @brief object to handle message logging and progress bars
    */
    class cw_utils {
        public:
            // base constructor
            cw_utils(const string_view& name, const verbosity_t& max_verbosity);

            // register new bar always displayed on std::cout as the last line below log print statements
            void add_fixed_bar(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty);

            // update fixed bar on to std::cout, 0 <= fraction <= 1.0
            void request_write_bar(double fraction) const;

            // completes to std::cout fixed bar to 100%
            void end_bar();

            /**
             * @brief log messages to std::cout or std::cerr if verbosity is high enough
             *
             * @param verbosity the verbosity of this message, which is logged if <= max verbosity
             * @param args printable objects to log
            */
            template <typename... Types>
            void log(const verbosity_t& verbosity, const Types&... args) const {
                if(verbosity <= max_verbosity) [[unlikely]] {
                    lock_guard print_lg(print_mx);

                    // erase progress bar, if it exists
                    if(bar.has_value()) cout << bar.value().bar_flusher;

                    if(verbosity <= ERROR) {
                        log_to_ostream(cerr, verbosity_type_to_name.at(verbosity), ": ", name, ' ', args...);
                        throw cw::assertion_failure_exception("Log with verbosity: " + verbosity_type_to_name.at(verbosity));
                    } else {
                        log_to_ostream(cout, verbosity_type_to_name.at(verbosity), ": ", name, ' ', args...);
                    }

                    // restore progress bar, if it exists
                    if(bar.has_value()) write_bar(bar.value().filled_ratio);
                }
            }

        protected:
            // fields for printing
            string name;
            verbosity_t max_verbosity;

            // declaration of global mutex to protect printing
            static mutex print_mx;

            // settings for fixed bar
            struct bar_settings {
                // print settings
                const size_t bar_width;
                const string msg;

                // util helper objects
                const string full_bar;
                const string bar_flusher;

                // current fraction of bar filled, [0.0, 1.0]
                double filled_ratio;

                // base constructor
                bar_settings(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty);
            };
            
            // declaration of global mutex to protect bar creation access
            static mutex bar_mx;

            // settings for fixed bar, if it exists
            static optional<bar_settings> bar;

            // with print_max locked, update fixed bar, 0 <= fraction <= 1.0
            void write_bar(double fraction) const;

            /**
             * @brief preferred types for printing
             * @note the types are restricted so that with -O3 optimizations enabled, the arguments
             *       don't have to be actually loaded from memory if the verbosity check fails. with
             *       this optimization, the assembly for each call to log() is a compare instruction
             *       followed by a conditional jump. since these arguments have no external effect,
             *       they can safely be only loaded after the jump if it occurs, meaning that in
             *       prod, a call to log() only costs 2 instructions of no op
            */
            template <typename T>
            struct no_mov_on_verbosity_fail :
                std::disjunction<
                    std::is_convertible<T, string_view>,
                    std::is_arithmetic<T>
                > {};

            // allowed types for printing, is a superset of preferred print types
            template <typename T>
            static constexpr bool is_printable =
                std::disjunction_v<
                    no_mov_on_verbosity_fail<T>,
                    std::is_class<T>,
                    std::is_enum<T>,
                    std::is_pointer<T>
                >;
            static_assert(is_printable<const char*>);
            static_assert(is_printable<string>);

            /**
             * @brief log messages to ostream, after verbosity check has passed
             *
             * @param os the stream to print to
             * @param msg the next printable object to logged
             * @param args the other arguments to be logged
            */
            template <class T, typename... Types>
            requires is_printable<T>
            void log_to_ostream(ostream& os, const T& msg, const Types&... args) const {
                os << msg;
                if constexpr(sizeof...(args) > 0) {
                    log_to_ostream(os, args...);
                } else {
                    os << endl << std::flush;
                }
            }

    }; // class cw_utils

    /**
     * @brief object to manage a progress bar lifetime and printing with cw::cw_utils
     * @cite https://codereview.stackexchange.com/a/186537
    */
    class progress_bar {
        public:
            // base constructor
            template <typename... Types>
            progress_bar(cw::cw_utils& utils, uint denominator, double granularity, const Types&... args)
                    : utils(utils),
                    numerator(0),
                    progress_ratio(0.0),
                    denominator(denominator),
                    granularity(granularity) {
                utils.add_fixed_bar(args...);
            }

            // add one to numerator of progress
            void incr_numerator();

            // destructor, terminates bar and prints newline
            ~progress_bar();

        private:
            cw::cw_utils& utils;
            uint numerator;
            double progress_ratio; // [0.0, 1.0]
            const uint denominator;
            const double granularity;

    }; // class progress_bar
}

/**
 * @brief helpers to check if struct has 'id' field of a certain type
*/
template <typename T, typename F>
struct has_id : std::false_type {};

template <typename T>
struct has_id<T, decltype(std::declval<T>().id)>
    : std::true_type {};

/**
 * @brief helpers to check if struct has clone() function returning a unique_ptr to itself
*/
template <typename T, typename = void>
struct has_clone : std::false_type {};

template <typename T>
struct has_clone<T, std::void_t<decltype(std::declval<T>().clone())> > : std::is_same<decltype(std::declval<T>().clone()), std::unique_ptr<T> > {};

/**
 * @brief holds vector of elements containing a unique size_t 'id' field equal to its index and clone() function
*/
template <class T>
requires std::conjunction_v<has_id<T, size_t>, has_clone<T> >
class id_obj_manager {
    public:
        id_obj_manager () = default;
        ~id_obj_manager() = default;

        // for id placeholders
        static constexpr size_t INVALID_ID = ULONG_MAX;

        // add element
        void push_back(unique_ptr<T>&& ptr) {
            if(!vec.has_value()) {
                vec = vector<unique_ptr<T> >();
            }

            cw_assert(vec.value().size() == ptr->id);
            vec.value().push_back(std::move(ptr));
        }

        // access size
        size_t size() const {
            if(!vec.has_value()) return 0ul;
            return vec.value().size();
        }

        // access vector element
        unique_ptr<T>& operator[](size_t n) {
            cw_assert(vec.has_value());
            cw_assert(vec.value()[n]->id == n);
            return vec.value()[n];
        }
        const unique_ptr<T>& operator[](size_t n) const {
            return const_cast<const unique_ptr<T>&>(const_cast<id_obj_manager*>(this)->operator[](n));
        }
        
        // ids of all managed elements
        vector<size_t> ids() const {
            vector<size_t> res(size());
            for(size_t i = 0; i < size(); ++i) {
                res[i] = i;
            }
            return res;
        }

        // shorthand for iterators
        using iterator = typename vector<unique_ptr<T> >::iterator;
        using const_iterator = typename vector<unique_ptr<T> >::const_iterator;

        // iterators
        iterator begin() {
            return vec.has_value() ? vec->begin() : empty_vec.begin();
        }
        const_iterator begin() const {
            return vec.has_value() ? vec->begin() : empty_vec.begin();
        }
        iterator end() {
            return vec.has_value() ? vec->end() : empty_vec.end();
        }
        const_iterator end() const {
            return vec.has_value() ? vec->end() : empty_vec.end();
        }

        // deep copy
        id_obj_manager clone() const {
            id_obj_manager copy;

            if(vec.has_value()) {
                vector<unique_ptr<T> > vec_copy;
                vec_copy.reserve(vec->size());

                for(const auto& ptr : vec.value()) {
                    vec_copy.emplace_back(ptr->clone());
                }

                copy.init(std::move(vec_copy));
            }
            return copy;
        }

        // explicitly not ordinarily copyable
        id_obj_manager(const id_obj_manager&) = delete;
        id_obj_manager& operator=(const id_obj_manager&) = delete;

        // movable
        id_obj_manager(id_obj_manager&& other) : id_obj_manager() {
            if(other.vec.has_value()) {
                init(std::move(other.vec.value()));
            }
        }
        id_obj_manager& operator=(id_obj_manager&& other) {
            cw_assert(!vec.has_value()); // cannot hold existing objects
            if(other.vec.has_value()) {
                init(std::move(other.vec.value()));
            }
            return *this;
        }

    protected:
        // underlying data
        optional<vector<unique_ptr<T> > > vec;

        // fallback for iterators when vec is empty
        static vector<unique_ptr<T> > empty_vec;

        // initialize with data
        void init(vector<unique_ptr<T> >&& data) {
            cw_assert(!vec.has_value());
            vec = std::move(data);
            for(size_t i = 0; i < size(); ++i) {
                cw_assert(vec.value()[i]->id == i);
            }
        }
};

// define empty_vec
template <class T>
requires std::conjunction_v<has_id<T, size_t>, has_clone<T> >
vector<unique_ptr<T> > id_obj_manager<T>::empty_vec = {};

/**
 * @brief template function to compare contents of hashsets for testing, T must have << operator defined
 * 
 * @param lhs ref to lhs set
 * @param rhs ref to rhs set
 * @param debug_prints prints debug messages iff debug_prints == true
 * @returns true iff contents of lhs & rhs are identical
*/
template <typename T> 
inline bool set_contents_equal(const unordered_set<T>& lhs, const unordered_set<T>& rhs, bool debug_prints) {

    cw::cw_utils utils("set_contents_equal()", VERBOSITY);
    bool result = true;

    if(lhs.size() != rhs.size()) {
        if(debug_prints) {
            utils.log(WARNING, "mismatched size: ", lhs.size(), " & ", rhs.size());
        }
        result = false;
    }

    for(const T& t : lhs) {
        if(rhs.count(t) == 0) {
            if(debug_prints) {
                utils.log(WARNING, "missing from rhs: ", t);
            }
            result = false;
        }
    }

    // not necessary for correctness checking, only for debug
    for(const T& t : rhs) {
        if(lhs.count(t) == 0) {
            if(debug_prints) {
                utils.log(WARNING, "missing from lhs: ", t);
            }
            result = false;
        }
    }

    return result;
}

/**
 * @brief template function to compare contents of hashmap to hashset for testing
 * 
 * @param lhs ref to lhs map
 * @param rhs ref to rhs map
 * @param debug_prints prints debug messages iff debug_prints == true
 * @returns true iff contents of lhs & rhs are identical
*/
template <typename K, typename V>
inline bool map_to_set_contents_equal(const unordered_map<K, unordered_set<V> >& lhs, const unordered_map<K, unordered_set<V> >& rhs, bool debug_prints) {
    
    cw::cw_utils utils("map_to_set_contents_equal()", VERBOSITY);
    bool result = true;

    if(lhs.size() != rhs.size()) {
        if(debug_prints) {
            utils.log(WARNING, "mismatched size: ", lhs.size(), " & ", rhs.size());
        }
        result = false;
    }
    
    for(const auto& pair : lhs) {
        if(rhs.find(pair.first) == rhs.end()) {
            if(debug_prints) {
                utils.log(WARNING, "key missing from rhs: ", pair.first);
            }
            result = false;
            continue;
        }
        if(!set_contents_equal(rhs.at(pair.first), pair.second, debug_prints)) {
            if(debug_prints) {
                utils.log(WARNING, "set contents unequal for key: ", pair.first);
            }
            result = false;
        }
    }

    return result;
}

/**
 * @cite https://stackoverflow.com/a/73674943
*/
template <class T> auto move_to_unique(T&& t) {
    return std::make_unique<std::remove_reference_t<T>>(std::move(t));
}
template <class V, class ... Args> auto make_vector_unique(Args ... args) {
    std::vector<std::unique_ptr<V>> rv;
    (rv.push_back(move_to_unique(args)), ...);
    return rv;
}
template <class V, class ... Args> auto make_unordered_set_unique(Args ... args) {
    std::unordered_set<std::unique_ptr<V>> rv;
    (rv.insert(move_to_unique(args)), ...);
    return rv;
}

#endif // CW_UTILS_H