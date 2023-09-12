// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: datatypes for top level cw generation module
// ==================================================================

#ifndef CW_GEN_DATA_TYPES_H
#define CW_GEN_DATA_TYPES_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw_gen_data_types_ns {

    // mapping from verbosity name to value
    unordered_map<string, verbosity_t> verbosity_map = {
        {"fatal", FATAL},
        {"error", ERROR},
        {"warning", WARNING},
        {"info", INFO},
        {"debug", DEBUG},
    };

    // map from dictionary name to path where it is located
    unordered_map<string, string> dict_path = {
        {"top1000",  "data/words_top1000.txt"},
        {"top3000",  "data/words_top3000.txt"},
        {"top10000", "data/words_top10000.txt"},
        {"crossfire", "data/crossfire_default.txt"},
        {"all",      "data/words_alpha.txt"},
    };

    // grid population methods 
    typedef enum {
        GRID_NONE = 0,
        GRID_CONTENTS = 1,
        GRID_PATTERN = 2
    } grid_population_method_t;

} // cw_gen_data_types_ns

#endif // CW_GEN_DATA_TYPES_H