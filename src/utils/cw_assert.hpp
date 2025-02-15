// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   2/15/2025
// Description: cw custom asserts
// ==================================================================

#ifndef CW_ASSERT_HPP
#define CW_ASSERT_HPP

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw {
    struct assertion_failure_exception : public exception {
        public:
            explicit assertion_failure_exception(const std::string& msg) : message(msg) {}

            const char* what() const noexcept override {
                return message.c_str();
            }

        private:
            std::string message;
    };
}

#undef cw_assert
#define cw_assert(...) do {                                                      \
    if (!(__VA_ARGS__)) [[unlikely]] {                                           \
        std::stringstream ss;                                                    \
        ss << "\nAssertion failed\n"                                             \
           << "File: " << __FILE__ << ": " << std::dec << __LINE__ << std::endl; \
        throw cw::assertion_failure_exception(ss.str());                         \
    }                                                                            \
} while(0)

#undef cw_assert_m
#define cw_assert_m(cond, ...) do {                                                                          \
    if (!(cond)) [[unlikely]] {                                                                              \
        std::stringstream ss;                                                                                \
        ss << "\nAssertion failed\n"                                                                         \
           << "File: " << __FILE__ << ": " << std::dec << __LINE__ << ", Msg: " << __VA_ARGS__ << std::endl; \
        throw cw::assertion_failure_exception(ss.str());                                                     \
    }                                                                                                        \
} while(0)

#endif // CW_ASSERT_HPP