// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: test driver declaration for cw_csp
// ==================================================================

#include "../../src/common/common_data_types.h"
#include "../../src/common/common_parent.h"
#include "../../src/cw_csp/cw_csp_data_types.h"
#include "../../src/cw_csp/cw_csp.h"

using namespace common_data_types_ns;
using namespace common_parent_ns;
using namespace cw_csp_data_types_ns;
using namespace cw_csp_ns;

namespace cw_csp_test_driver_ns {
    class cw_csp_test_driver : public common_parent {
        public:
            // base constructor
            cw_csp_test_driver(string name);
            
            // destructor
            ~cw_csp_test_driver() { delete dut; }

        private:
            // TODO: change to use unique_ptr
            cw_csp* dut = nullptr;
    }; // cw_csp_test_driver
} // cw_csp_test_driver_ns