// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/14/2024
// Description: data types for execution time tracker and related object declarations for cw_csp performance analysis
// ==================================================================

#ifndef CW_TRACER_DATA_TYPES_H
#define CW_TRACER_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"

using namespace common_data_types_ns;
using namespace cw;

#define TRACER_TRACK_AC3

namespace cw_tracer_data_types_ns {
    /**
     * @brief type of tasks executed during a timestep
    */
    enum ts_type_t {
        // cw_csp
        TS_CSP_TOTAL,       // all execution in cw_csp
        TS_CSP_INITIALIZE,  // cw_csp.initialize_csp()
        TS_CSP_SOLVE,       // cw_csp.solve()
        TS_CSP_SEARCH_STEP, // an attempt to assign any value to a variable, represents a group of search tree nodes branched off of a common parent node
        TS_CSP_TRY_ASSIGN,  // an attempt to assign a specific word during a search step, represents a search tree node

        #ifdef TRACER_TRACK_AC3
        TS_CSP_AC3,         // cw_csp.ac3()
        TS_CSP_AC3_PRUNE,   // a single call to cw_variable::prune_domain()
        TS_CSP_UNDO_AC3,    // cw_csp.undo_ac3()
        #endif // TRACER_TRACK_AC3
    };

    // mapping from timestep type to display name
    NLOHMANN_JSON_SERIALIZE_ENUM( ts_type_t, {
        {TS_CSP_TOTAL,       "CSP"},
        {TS_CSP_INITIALIZE,  "Initialize"},
        {TS_CSP_SOLVE,       "Solve"},
        {TS_CSP_SEARCH_STEP, "Search Step"},
        {TS_CSP_TRY_ASSIGN,  "Try Assign"},

        #ifdef TRACER_TRACK_AC3
        {TS_CSP_AC3,         "AC3"},
        {TS_CSP_AC3_PRUNE,   "AC3 Prune"},
        {TS_CSP_UNDO_AC3,    "Undo AC3"},
        #endif // TRACER_TRACK_AC3
    })
}; // cw_tracer_data_types_ns

#endif // CW_TRACER_DATA_TYPES_H