// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module implementation
// ==================================================================

#include "cw_utils.h"

#ifndef VERBOSITY_NUM
verbosity_t VERBOSITY = INFO;
#else
verbosity_t VERBOSITY = static_cast<verbosity_t>(VERBOSITY_NUM);
#endif

unordered_map<verbosity_t, string> verbosity_type_to_name = {
    {FATAL, "FATAL"},
    {ERROR, "ERROR"},
    {WARNING, "WARNING"},
    {INFO, "INFO"},
    {DEBUG, "DEBUG"},
};

// /**
//  * @brief construct new utils object
//  * 
//  * @param name name of this util object
//  * @param min_verbosity min verbosity of messages to be printed
//  */
// cw_utils::cw_utils(string name, verbosity_t min_verbosity) {
//     this->name = name;
//     this->min_verbosity = min_verbosity;
// }

// /**
//  * @brief print message to std::cout or std::cerr
//  * 
//  * @param s message to print
//  * @param verbosity verbosity of message, INFO default
//  * @return status of message, true == ok
//  */
// bool cw_utils::print_msg(string s, verbosity_t verbosity) {
//     if(this->min_verbosity >= verbosity) {
//         switch(verbosity) {
//         case FATAL:
//             cerr << "Fatal Error: " << name << " " << s << endl;
//             throw assertion_failure_exception();   
//             return false;
//             break;
//         case ERROR:
//             cerr << "Error: " << name << " " << s << endl;
//             throw assertion_failure_exception();   
//             return false;
//             break;
//         case WARNING:
//             cout << "Warning: " << name << " " << s << endl;
//             return false;
//             break;
//         case INFO:
//             cout << "Info: " << name << " " << s << endl;
//             return true;
//             break;
//         case DEBUG:
//             cout << "Debug: " << name << " " << s << endl;
//             return true;
//             break;
//         default:
//             cerr << "Debug Warning: " << name << " got unknown verbosity with msg " << s << endl;
//             return true;
//             break;
//         }
//     } else {
//         return verbosity != FATAL && verbosity != ERROR;
//     }
// }

// /**
//  * @brief print message to std::cout or std::cerr
//  * 
//  * @param s stringstream ptr to message
//  * @param verbosity verbosity of message, INFO default
//  * @return status of message, true == ok
//  */
// bool cw_utils::print_msg(stringstream *s, verbosity_t verbosity) {
//     bool result = print_msg(s->str(), verbosity);
//     s->str("");
//     return result;
// }

/**
 * @brief construct new utils object
 * 
 * @param name name of this util object
 * @param min_verbosity min verbosity of messages to be printed
*/
cw_utils::cw_utils(const string_view& name, const verbosity_t& min_verbosity) : name(name), min_verbosity(min_verbosity) {
    // do nothing, initializer list is sufficient
}

/**
 * @brief log messages to std::cout or std::cerr if verbosity is high enough
 * 
 * @param verbosity the verbosity of this message, which is logged if >= min verbosity
 * @param args printable objects to log
*/
template <typename... Types>
void cw_utils::log(const verbosity_t& verbosity, const Types&... args) const {
    if(verbosity >= min_verbosity) [[unlikely]] {
        if(verbosity >= ERROR) {
            log_std_cerr(verbosity_type_to_name.at(verbosity), ": ", name, ' ', args..., endl);
        } else {
            log_std_cout(verbosity_type_to_name.at(verbosity), ": ", name, ' ', args..., endl);
        }
    }
}

/**
 * @brief log messages to std::cout
 * 
 * @param msg the next printable object to logged
 * @param args the other arguments to be logged
*/
template <class T, typename, typename... Types>
void cw_utils::log_std_cout(const T& msg, const Types&... args) const {
    cout << msg;
    if constexpr(sizeof...(args) > 0) {
        log_std_cout(args...);
    }
}

/**
 * @brief log messages to std::cerr
 * 
 * @param msg the next printable object to logged
 * @param args the other arguments to be logged
*/
template <class T, typename, typename... Types>
void cw_utils::log_std_cerr(const T& msg, const Types&... args) const {
    cerr << msg;
    if constexpr(sizeof...(args) > 0) {
        log_std_cerr(args...);
    }
}

// ############### set_contents_equal() ###############

template bool set_contents_equal<string>(const unordered_set<string>* lhs, const unordered_set<string>* rhs, bool debug_prints);
template bool set_contents_equal<word_t>(const unordered_set<word_t>* lhs, const unordered_set<word_t>* rhs, bool debug_prints);