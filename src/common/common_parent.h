// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: common virtual class declaration
// ==================================================================

#ifndef COMMON_PARENT_H
#define COMMON_PARENT_H

#include "common_data_types.h"
#include "../utils/cw_utils.h"

namespace common_parent_ns {
    class common_parent {
        public:
            string name;
            cw_utils* utils;
            stringstream ss;

            common_parent(string name);

            // TODO: add more common functions
            
        private:
            // TODO: implement
    };
}

#endif // COMMON_PARENT_H