// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/14/2024
// Description: data types for execution time tracker and related object declarations for cw_csp performance analysis
// ==================================================================

#ifndef CW_TIMETRACKER_DATA_TYPES_H
#define CW_TIMETRACKER_DATA_TYPES_H

#include "../common/common_data_types.h"
#include "../lib/src/json.hpp"
#include "../utils/cw_utils.h"

using namespace common_data_types_ns;

using json = nlohmann::json;

namespace cw_timetracker_data_types_ns {
    /**
     * @brief type of tasks executed during a timestep
    */
    enum ts_type_t {
        // cw_csp
        TS_CSP_TOTAL,             // all execution in cw_csp
        TS_CSP_INITIALIZE,        // cw_csp.initialize_csp()
        TS_CSP_SOLVE,             // cw_csp.solve()
        TS_CSP_SOLVED,            // cw_csp.solved()
        TS_CSP_AC3,               // cw_csp.ac3()
        TS_CSP_UNDO_AC3,          // cw_csp.undo_ac3()
        TS_CSP_BACKTRACK_STEP,    // cw_csp.solve_backtracking()
        TS_CSP_TRY_ASSIGN,        // an attempt to assign word in cw_csp.solve_backtracking()
        TS_CSP_GATHER_DOMAIN,     // get_cur_domain() call and sort in in cw_csp.solve_backtracking()
    };

    // mapping from timestep type to display name
    extern unordered_map<ts_type_t, string> ts_type_name_map;
}; // cw_timetracker_data_types_ns

#endif // CW_TIMETRACKER_DATA_TYPES_H