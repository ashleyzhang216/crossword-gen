// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module implementation
// ==================================================================

#include "cw_utils.h"

using namespace cw;

// ############### cw_utils ###############



// ############### set_contents_equal() ###############

template bool set_contents_equal<string>(const unordered_set<string>& lhs, const unordered_set<string>& rhs, bool debug_prints);
template bool set_contents_equal<word_t>(const unordered_set<word_t>& lhs, const unordered_set<word_t>& rhs, bool debug_prints);
