// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: test driver whose only purpose is to contain Catch2
// ==================================================================

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

TEST_CASE("catch_hello_world", "[catch],[hello_world],[quick]") {
  REQUIRE(true);
}