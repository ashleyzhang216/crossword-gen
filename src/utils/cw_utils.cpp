// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module implementation
// ==================================================================

#include "cw_utils.h"

#ifndef VERBOSITY_NUM
verbosity_t VERBOSITY = INFO;
#else
verbosity_t VERBOSITY = (verbosity_t)VERBOSITY_NUM;
#endif

/**
 * @brief construct new utils object
 * 
 * @param name name of this util object
 * @param min_verbosity min verbosity of messages to be printed
 */
cw_utils::cw_utils(string name, verbosity_t min_verbosity) {
    this->name = name;
    this->min_verbosity = min_verbosity;
}

/**
 * @brief print message to std::cout or std::cerr
 * 
 * @param s message to print
 * @param verbosity verbosity of message, INFO default
 * @return status of message, true == ok
 */
bool cw_utils::print_msg(string s, verbosity_t verbosity) {
    if(this->min_verbosity >= verbosity) {
        switch(verbosity) {
        case FATAL:
            cerr << "Fatal Error: " << name << " " << s << endl;
            return false;
            break;
        case ERROR:
            cerr << "Error: " << name << " " << s << endl;
            return false;
            break;
        case WARNING:
            cout << "Warning: " << name << " " << s << endl;
            return false;
            break;
        case INFO:
            cout << "Info: " << name << " " << s << endl;
            return true;
            break;
        case DEBUG:
            cout << "Debug: " << name << " " << s << endl;
            return true;
            break;
        default:
            cerr << "Debug Warning: " << name << " got unknown verbosity with msg " << s << endl;
            return true;
            break;
        }
    } else {
        return verbosity != FATAL && verbosity != ERROR;
    }
}

/**
 * @brief print message to std::cout or std::cerr
 * 
 * @param s stringstream ptr to message
 * @param verbosity verbosity of message, INFO default
 * @return status of message, true == ok
 */
bool cw_utils::print_msg(stringstream *s, verbosity_t verbosity) {
    bool result = print_msg(s->str(), verbosity);
    s->str("");
    return result;
}

// ############### set_contents_equal() ###############

template bool set_contents_equal<string>(const unordered_set<string>* lhs, const unordered_set<string>* rhs);