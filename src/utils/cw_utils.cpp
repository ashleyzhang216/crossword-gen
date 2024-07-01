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

/**
 * @brief construct new utils object
 * 
 * @param name name of this util object
 * @param min_verbosity min verbosity of messages to be printed
*/
cw_utils::cw_utils(const string_view& name, const verbosity_t& min_verbosity) : name(name), min_verbosity(min_verbosity) {
    // do nothing, initializer list is sufficient
}

// ############### set_contents_equal() ###############

template bool set_contents_equal<string>(const unordered_set<string>& lhs, const unordered_set<string>& rhs, bool debug_prints);
template bool set_contents_equal<word_t>(const unordered_set<word_t>& lhs, const unordered_set<word_t>& rhs, bool debug_prints);