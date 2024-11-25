// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/14/2024
// Description: data types implementations for execution time tracker and related object declarations for cw_csp performance analysis
// ==================================================================

#include "cw_timetracker_data_types.h"

using namespace cw_timetracker_data_types_ns;

namespace cw_timetracker_data_types_ns {
    // mapping from timestep type to display name
    unordered_map<ts_type_t, string> ts_type_name_map = {
        {TS_CSP_TOTAL,             "Total"},
        {TS_CSP_INITIALIZE,        "Initialize"},
        {TS_CSP_SOLVE,             "Solve"},
        {TS_CSP_SOLVED,            "Solved"},
        {TS_CSP_AC3,               "AC3"},
        {TS_CSP_UNDO_AC3,          "Undo AC3"},
        {TS_CSP_BACKTRACK_STEP,    "Solve Backtracking"},
        {TS_CSP_TRY_ASSIGN,        "Try Assign"},
        {TS_CSP_GATHER_DOMAIN,     "Gather Domain"},
    };
};
