// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: Catch2 tests for crossword class
// ==================================================================

#include "../catch/catch.hpp"
#include "crossword_test_driver.h"

using namespace crossword_test_driver_ns;

/**
 * hello world test for crossword
*/
TEST_CASE("crossword hello_world", "[crossword],[hello_world],[quick]") {
    REQUIRE(true);
}

/**
 * construction test for crossword without contents
*/
TEST_CASE("crossword constructor_no_contents", "[crossword],[quick]") {
    crossword_test_driver* driver = new crossword_test_driver("crossword constructor_no_contents");

    stringstream expected_1_1;
    expected_1_1 << endl << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(1, 1, expected_1_1.str()));

    stringstream expected_1_2;
    expected_1_2 << endl << WILDCARD << " " << endl << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(1, 2, expected_1_2.str()));

    stringstream expected_2_1;
    expected_2_1 << endl << WILDCARD << " " << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(2, 1, expected_2_1.str()));

    stringstream expected_2_3;
    expected_2_3 << endl << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(2, 3, expected_2_3.str()));

    stringstream expected_3_2;
    expected_3_2 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(3, 2, expected_3_2.str()));

    stringstream expected_4_4;
    expected_4_4 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(4, 4, expected_4_4.str()));

    stringstream expected_6_6;
    expected_6_6 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " 
                 << endl << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " " << WILDCARD << " ";
    REQUIRE(driver->test_constructor_empty(6, 6, expected_6_6.str()));
}

/**
 * construction test for crossword with contents in vector form
*/
TEST_CASE("crossword constructor_with_contents_vector", "[crossword],[quick]") {
    crossword_test_driver* driver = new crossword_test_driver("crossword constructor_with_contents_vector");

    stringstream expected_1_1_wildcard;
    expected_1_1_wildcard << endl << WILDCARD << " ";
    REQUIRE(driver->test_constructor_contents_vector(1, 1, {{WCD}}, expected_1_1_wildcard.str()));

    stringstream expected_1_1_black;
    expected_1_1_black << endl << BLACK << " ";
    REQUIRE(driver->test_constructor_contents_vector(1, 1, {{BLK}}, expected_1_1_black.str()));

    stringstream expected_1_1_a;
    expected_1_1_a << endl << 'a' << " ";
    REQUIRE(driver->test_constructor_contents_vector(1, 1, {{'a'}}, expected_1_1_a.str()));

    stringstream expected_1_2_ab;
    expected_1_2_ab << endl << 'a' << " " << endl << 'b' << " ";
    REQUIRE(driver->test_constructor_contents_vector(1, 2, {{'a'}, {'b'}}, expected_1_2_ab.str()));

    stringstream expected_2_1_ab;
    expected_2_1_ab << endl << 'a' << " " << 'b' << " ";
    REQUIRE(driver->test_constructor_contents_vector(2, 1, {{'a', 'b'}}, expected_2_1_ab.str()));

    stringstream expected_2_3_alphabet;
    expected_2_3_alphabet << endl << 'a' << " " << 'b' << " " 
                          << endl << 'c' << " " << 'd' << " " 
                          << endl << 'e' << " " << 'f' << " ";
    REQUIRE(driver->test_constructor_contents_vector(2, 3, {{'a','b'},{'c','d'},{'e','f'}}, expected_2_3_alphabet.str()));
}

/**
 * construction test for crossword with contents in string form
*/
TEST_CASE("crossword constructor_with_contents_string", "[crossword],[quick]") {
    crossword_test_driver* driver = new crossword_test_driver("crossword constructor_with_contents_string");

    stringstream input_1_1_wildcard;
    input_1_1_wildcard << WCD;
    stringstream expected_1_1_wildcard;
    expected_1_1_wildcard << endl << WILDCARD << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_wildcard.str(), expected_1_1_wildcard.str()));

    stringstream input_1_1_black;
    input_1_1_black << BLACK;
    stringstream expected_1_1_black;
    expected_1_1_black << endl << BLACK << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_black.str(), expected_1_1_black.str()));

    stringstream input_1_1_x;
    input_1_1_x << 'x';
    stringstream expected_1_1_x;
    expected_1_1_x << endl << 'x' << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_x.str(), expected_1_1_x.str()));

    stringstream input_1_2_ab;
    input_1_2_ab << "ab";
    stringstream expected_1_2_ab;
    expected_1_2_ab << endl << 'a' << " " << endl << 'b' << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 2, input_1_2_ab.str(), expected_1_2_ab.str()));

    stringstream input_2_13_alphabet;
    input_2_13_alphabet << "abcdefghijklmnopqrstuvwxyz";
    stringstream expected_2_13_alphabet;
    expected_2_13_alphabet << endl << 'a' << " " << 'b' << " " 
                           << endl << 'c' << " " << 'd' << " " 
                           << endl << 'e' << " " << 'f' << " " 
                           << endl << 'g' << " " << 'h' << " " 
                           << endl << 'i' << " " << 'j' << " " 
                           << endl << 'k' << " " << 'l' << " " 
                           << endl << 'm' << " " << 'n' << " " 
                           << endl << 'o' << " " << 'p' << " " 
                           << endl << 'q' << " " << 'r' << " " 
                           << endl << 's' << " " << 't' << " " 
                           << endl << 'u' << " " << 'v' << " " 
                           << endl << 'w' << " " << 'x' << " " 
                           << endl << 'y' << " " << 'z' << " ";
    REQUIRE(driver->test_constructor_contents_string(2, 13, input_2_13_alphabet.str(), expected_2_13_alphabet.str()));

    stringstream input_13_2_alphabet;
    input_13_2_alphabet << "abcdefghijklmnopqrstuvwxyz";
    stringstream expected_13_2_alphabet;
    expected_13_2_alphabet << endl << 'a' << " " << 'b' << " " << 'c' << " " << 'd' << " " << 'e' << " " << 'f' << " " << 'g' << " " << 'h' << " " << 'i' << " " << 'j' << " " << 'k' << " " << 'l' << " " << 'm' << " " 
                           << endl << 'n' << " " << 'o' << " " << 'p' << " " << 'q' << " " << 'r' << " " << 's' << " " << 't' << " " << 'u' << " " << 'v' << " " << 'w' << " " << 'x' << " " << 'y' << " " << 'z' << " ";
    REQUIRE(driver->test_constructor_contents_string(13, 2, input_13_2_alphabet.str(), expected_13_2_alphabet.str()));

    stringstream input_5_5_alphabet;
    input_5_5_alphabet << "abcdefghijklmnopqrstuvwxy";
    stringstream expected_5_5_alphabet;
    expected_5_5_alphabet << endl << 'a' << " " << 'b' << " " << 'c' << " " << 'd' << " " << 'e' << " " 
                          << endl << 'f' << " " << 'g' << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                          << endl << 'k' << " " << 'l' << " " << 'm' << " " << 'n' << " " << 'o' << " " 
                          << endl << 'p' << " " << 'q' << " " << 'r' << " " << 's' << " " << 't' << " " 
                          << endl << 'u' << " " << 'v' << " " << 'w' << " " << 'x' << " " << 'y' << " ";
    REQUIRE(driver->test_constructor_contents_string(5, 5, input_5_5_alphabet.str(), expected_5_5_alphabet.str()));

    stringstream input_5_5_mixed;
    input_5_5_mixed << BLK << WCD << "cd" << WCD << 'f' << BLK << "hijk" << BLK << 'm' << BLK << 'o' << WCD << BLK << 'r' << WCD << BLK << WCD << BLK << "wxy";
    stringstream expected_5_5_mixed;
    expected_5_5_mixed << endl << BLK << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                       << endl << 'f' << " " << BLK << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                       << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                       << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                       << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'y' << " ";
    REQUIRE(driver->test_constructor_contents_string(5, 5, input_5_5_mixed.str(), expected_5_5_mixed.str()));
}