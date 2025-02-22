// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/14/2024
// Description: data types for execution time tracker and related object declarations for cw_csp performance analysis
// ==================================================================

#ifndef CW_TIMETRACKER_DATA_TYPES_H
#define CW_TIMETRACKER_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"
#include "../lib/src/json.hpp"

using namespace common_data_types_ns;
using namespace cw;

// need to use ordered_json to have children as the last key for each timestep
// otherwise, output json files become very tricky to debug
using ordered_json = nlohmann::ordered_json;

// for timestep results, order agnostic
using basic_jason = nlohmann::json;

// #define TIMETRACKER_TRACK_AC3

namespace cw_timetracker_data_types_ns {
    /**
     * @brief type of tasks executed during a timestep
    */
    enum ts_type_t {
        // cw_csp
        TS_CSP_TOTAL,             // all execution in cw_csp
        TS_CSP_INITIALIZE,        // cw_csp.initialize_csp()
        TS_CSP_SOLVE,             // cw_csp.solve()
        TS_CSP_BACKTRACK_STEP,    // cw_csp.solve_backtracking()
        TS_CSP_TRY_ASSIGN,        // an attempt to assign word in cw_csp.solve_backtracking()

        #ifdef TIMETRACKER_TRACK_AC3
        TS_CSP_AC3,               // cw_csp.ac3()
        TS_CSP_UNDO_AC3,          // cw_csp.undo_ac3()
        #endif // TIMETRACKER_TRACK_AC3
    };

    // mapping from timestep type to display name
    NLOHMANN_JSON_SERIALIZE_ENUM( ts_type_t, {
        {TS_CSP_TOTAL,             "Total"},
        {TS_CSP_INITIALIZE,        "Initialize"},
        {TS_CSP_SOLVE,             "Solve"},
        {TS_CSP_BACKTRACK_STEP,    "Solve Backtracking"},
        {TS_CSP_TRY_ASSIGN,        "Try Assign"},

        #ifdef TIMETRACKER_TRACK_AC3
        {TS_CSP_AC3,               "AC3"},
        {TS_CSP_UNDO_AC3,          "Undo AC3"},
        #endif // TIMETRACKER_TRACK_AC3
    })
}; // cw_timetracker_data_types_ns

#endif // CW_TIMETRACKER_DATA_TYPES_H