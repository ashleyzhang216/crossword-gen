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

/**
 * test case for crossword modification
*/
TEST_CASE("crossword modification", "[crossword],[quick]") {
    unique_ptr<crossword_test_driver> driver = make_unique<crossword_test_driver>("crossword modification");

    const string stairs_contents = "  ??????? ?????? ??????? ??????? ??????? ??????? ??????? ??????? ?????? ???????  ";
    vector<word_assignment> stairs_assignments = {
        // order randomized
        {.origin_row = 0, .origin_col = 3, .word = "areas",   .dir = DOWN  }, // 18
        {.origin_row = 2, .origin_col = 7, .word = "oeuvre",  .dir = DOWN  }, // 26
        {.origin_row = 0, .origin_col = 6, .word = "is",      .dir = DOWN  }, // 24
        {.origin_row = 8, .origin_col = 0, .word = "genuine", .dir = ACROSS}, // 13
        {.origin_row = 7, .origin_col = 2, .word = "in",      .dir = DOWN  }, // 17
        {.origin_row = 5, .origin_col = 4, .word = "semi",    .dir = DOWN  }, // 21
        {.origin_row = 4, .origin_col = 5, .word = "maul",    .dir = ACROSS}, // 7
        {.origin_row = 0, .origin_col = 4, .word = "brad",    .dir = DOWN  }, // 20
        {.origin_row = 2, .origin_col = 0, .word = "demean",  .dir = ACROSS}, // 2
        {.origin_row = 5, .origin_col = 0, .word = "end",     .dir = ACROSS}, // 8
        {.origin_row = 2, .origin_col = 0, .word = "dicebag", .dir = DOWN  }, // 14
        {.origin_row = 0, .origin_col = 5, .word = "bin",     .dir = DOWN  }, // 22
        {.origin_row = 5, .origin_col = 4, .word = "solve",   .dir = ACROSS}, // 9
        {.origin_row = 0, .origin_col = 2, .word = "rabbits", .dir = ACROSS}, // 0
        {.origin_row = 6, .origin_col = 0, .word = "by",      .dir = ACROSS}, // 10
        {.origin_row = 6, .origin_col = 3, .word = "gnu",     .dir = DOWN  }, // 19
        {.origin_row = 3, .origin_col = 6, .word = "peg",     .dir = ACROSS}, // 5
        {.origin_row = 0, .origin_col = 8, .word = "singled", .dir = DOWN  }, // 27
        {.origin_row = 4, .origin_col = 0, .word = "cons",    .dir = ACROSS}, // 6
        {.origin_row = 7, .origin_col = 2, .word = "inmate",  .dir = ACROSS}, // 12
        {.origin_row = 6, .origin_col = 3, .word = "gerard",  .dir = ACROSS}, // 11
        {.origin_row = 1, .origin_col = 1, .word = "ferris",  .dir = ACROSS}, // 1
        {.origin_row = 1, .origin_col = 1, .word = "felony",  .dir = DOWN  }, // 15
        {.origin_row = 3, .origin_col = 0, .word = "iliad",   .dir = ACROSS}, // 4
        {.origin_row = 0, .origin_col = 2, .word = "remind",  .dir = DOWN  }, // 16
        {.origin_row = 4, .origin_col = 5, .word = "moran",   .dir = DOWN  }, // 23
        {.origin_row = 3, .origin_col = 6, .word = "palate",  .dir = DOWN  }, // 25
        {.origin_row = 2, .origin_col = 7, .word = "on",      .dir = ACROSS}, // 3
    };
    vector<string> stairs_ground_truth = {
        construct_cw_output("  ?a????? ??r??? ????e?? ?????a? ??????s ??????? ??????? ??????? ?????? ???????  ", 9), // areas
        construct_cw_output("  ?a????? ??r??? ????e?? o????a? ?e????s ??u???? ???v??? ????r?? ?????e ???????  ", 9), // oeuvre
        construct_cw_output("  ?a??i?? ??r??s ????e?? o????a? ?e????s ??u???? ???v??? ????r?? ?????e ???????  ", 9), // is
        construct_cw_output("  ?a??i?? ??r??s ????e?? o????a? ?e????s ??u???? ???v??? ????r?? ?????e genuine  ", 9), // genuine
        construct_cw_output("  ?a??i?? ??r??s ????e?? o????a? ?e????s ??u???? ???v??? ????r?? i????e genuine  ", 9), // in
        construct_cw_output("  ?a??i?? ??r??s ????e?? o????a? ?e????s ??u???? s??v??? ?e??r?? i?m??e genuine  ", 9), // semi
        construct_cw_output("  ?a??i?? ??r??s ????e?? o????a? ?e????s maul??? s??v??? ?e??r?? i?m??e genuine  ", 9), // maul
        construct_cw_output("  ?ab?i?? ??rr?s ????ea? o????ad ?e????s maul??? s??v??? ?e??r?? i?m??e genuine  ", 9), // brad
        construct_cw_output("  ?ab?i?? ??rr?s ?demean o????ad ?e????s maul??? s??v??? ?e??r?? i?m??e genuine  ", 9), // demean
        construct_cw_output("  ?ab?i?? ??rr?s ?demean o????ad ?e????s maulend s??v??? ?e??r?? i?m??e genuine  ", 9), // end
        construct_cw_output("  ?ab?i?? ??rr?s ?demean o?i??ad ?e?c??s maulend s??v?b? ?e??r?a i?m??e genuine  ", 9), // dicebag
        construct_cw_output("  ?abbi?? ??rris ?demean o?i??ad ?e?c??s maulend s??v?b? ?e??r?a i?m??e genuine  ", 9), // bin
        construct_cw_output("  ?abbi?? ??rris ?demean o?i??ad ?e?c??s maulend solveb? ?e??r?a i?m??e genuine  ", 9), // solve
        construct_cw_output("  rabbits ??rris ?demean o?i??ad ?e?c??s maulend solveb? ?e??r?a i?m??e genuine  ", 9), // rabbits
        construct_cw_output("  rabbits ??rris ?demean o?i??ad ?e?c??s maulend solveby ?e??r?a i?m??e genuine  ", 9), // by
        construct_cw_output("  rabbits ??rris ?demean o?i??ad ?e?c??s maulend solveby ge??r?a inm??e genuine  ", 9), // gnu
        construct_cw_output("  rabbits ??rris ?demean o?i??ad pegc??s maulend solveby ge??r?a inm??e genuine  ", 9), // peg
        construct_cw_output("  rabbits ??rris idemean oni??ad pegc??s maulend solveby ge??rda inm??e genuine  ", 9), // singled
        construct_cw_output("  rabbits ??rris idemean oni??ad pegcons maulend solveby ge??rda inm??e genuine  ", 9), // cons
        construct_cw_output("  rabbits ??rris idemean oni??ad pegcons maulend solveby ge??rda inmate genuine  ", 9), // inmate
        construct_cw_output("  rabbits ??rris idemean oni??ad pegcons maulend solveby gerarda inmate genuine  ", 9), // gerard
        construct_cw_output("  rabbits ferris idemean oni??ad pegcons maulend solveby gerarda inmate genuine  ", 9), // ferris
        construct_cw_output("  rabbits ferris idemean onil?ad pegcons maulend solveby gerarda inmate genuine  ", 9), // felony
        construct_cw_output("  rabbits ferris idemean oniliad pegcons maulend solveby gerarda inmate genuine  ", 9), // iliad
        construct_cw_output("  rabbits ferris idemean oniliad pegcons maulend solveby gerarda inmate genuine  ", 9), // remind
        construct_cw_output("  rabbits ferris idemean oniliad pegcons maulend solveby gerarda inmate genuine  ", 9), // moran
        construct_cw_output("  rabbits ferris idemean oniliad pegcons maulend solveby gerarda inmate genuine  ", 9), // palate
        construct_cw_output("  rabbits ferris idemean oniliad pegcons maulend solveby gerarda inmate genuine  ", 9)  // on
    };

    REQUIRE(driver->test_modification(9, 9, stairs_contents, stairs_assignments, stairs_ground_truth));

    const string bridge_contents = "??? ???????? ????????? ????  ????????? ??? ?????????  ???? ????????? ???????? ???";
    vector<word_assignment> bridge_assignments = {
        // order randomized
        {.origin_row = 6, .origin_col = 0, .word = "noun",    .dir = ACROSS}, // 11
        {.origin_row = 1, .origin_col = 4, .word = "delhi",   .dir = ACROSS}, // 3
        {.origin_row = 7, .origin_col = 4, .word = "in",      .dir = DOWN  }, // 26
        {.origin_row = 0, .origin_col = 2, .word = "from",    .dir = DOWN  }, // 21
        {.origin_row = 4, .origin_col = 7, .word = "ny",      .dir = ACROSS}, // 9
        {.origin_row = 7, .origin_col = 6, .word = "til",     .dir = ACROSS}, // 14
        {.origin_row = 3, .origin_col = 2, .word = "meadows", .dir = ACROSS}, // 6
        {.origin_row = 1, .origin_col = 0, .word = "orr",     .dir = ACROSS}, // 2
        {.origin_row = 4, .origin_col = 0, .word = "ms",      .dir = ACROSS}, // 7
        {.origin_row = 4, .origin_col = 0, .word = "mends",   .dir = DOWN  }, // 18
        {.origin_row = 0, .origin_col = 8, .word = "sissy",   .dir = DOWN  }, // 32
        {.origin_row = 0, .origin_col = 4, .word = "cd",      .dir = DOWN  }, // 24
        {.origin_row = 0, .origin_col = 1, .word = "era",     .dir = DOWN  }, // 19
        {.origin_row = 5, .origin_col = 0, .word = "enticed", .dir = ACROSS}, // 10
        {.origin_row = 0, .origin_col = 6, .word = "elmo",    .dir = DOWN  }, // 28
        {.origin_row = 4, .origin_col = 1, .word = "snout",   .dir = DOWN  }, // 20
        {.origin_row = 2, .origin_col = 3, .word = "seminar", .dir = DOWN  }, // 23
        {.origin_row = 4, .origin_col = 3, .word = "mrs",     .dir = ACROSS}, // 8
        {.origin_row = 5, .origin_col = 6, .word = "data",    .dir = DOWN  }, // 29
        {.origin_row = 6, .origin_col = 5, .word = "take",    .dir = ACROSS}, // 12
        {.origin_row = 6, .origin_col = 7, .word = "kit",     .dir = DOWN  }, // 31
        {.origin_row = 3, .origin_col = 4, .word = "arc",     .dir = DOWN  }, // 25
        {.origin_row = 8, .origin_col = 0, .word = "stern",   .dir = ACROSS}, // 15
        {.origin_row = 8, .origin_col = 6, .word = "atm",     .dir = ACROSS}, // 16
        {.origin_row = 6, .origin_col = 8, .word = "elm",     .dir = DOWN  }, // 33
        {.origin_row = 7, .origin_col = 0, .word = "dubai",   .dir = ACROSS}, // 13
        {.origin_row = 0, .origin_col = 7, .word = "shown",   .dir = DOWN  }, // 30
        {.origin_row = 0, .origin_col = 4, .word = "chess",   .dir = ACROSS}, // 1
        {.origin_row = 5, .origin_col = 2, .word = "tube",    .dir = DOWN  }, // 22
        {.origin_row = 2, .origin_col = 0, .word = "taos",    .dir = ACROSS}, // 4
        {.origin_row = 2, .origin_col = 5, .word = "amos",    .dir = ACROSS}, // 5
        {.origin_row = 0, .origin_col = 0, .word = "dot",     .dir = DOWN  }, // 17
        {.origin_row = 0, .origin_col = 5, .word = "headset", .dir = DOWN  }, // 27
        {.origin_row = 0, .origin_col = 0, .word = "def",     .dir = ACROSS}  // 0
    };
    vector<string> bridge_ground_truth = {
        construct_cw_output("??? ???????? ????????? ????  ????????? ??? ?????????  noun ????????? ???????? ???", 9), // noun
        construct_cw_output("??? ???????? delhi???? ????  ????????? ??? ?????????  noun ????????? ???????? ???", 9), // delhi
        construct_cw_output("??? ???????? delhi???? ????  ????????? ??? ?????????  noun ????????i ???????n ???", 9), // in
        construct_cw_output("??f ???????r delhi??o? ????  m???????? ??? ?????????  noun ????????i ???????n ???", 9), // from
        construct_cw_output("??f ???????r delhi??o? ????  m???????? ??? ny???????  noun ????????i ???????n ???", 9), // ny
        construct_cw_output("??f ???????r delhi??o? ????  m???????? ??? ny???????  noun ????????i til????n ???", 9), // til
        construct_cw_output("??f ???????r delhi??o? ????  meadows?? ??? ny???????  noun ????????i til????n ???", 9), // meadows
        construct_cw_output("??f ?????orr delhi??o? ????  meadows?? ??? ny???????  noun ????????i til????n ???", 9), // orr
        construct_cw_output("??f ?????orr delhi??o? ????  meadowsms ??? ny???????  noun ????????i til????n ???", 9), // ms
        construct_cw_output("??f ?????orr delhi??o? ????  meadowsms ??? nye??????  noun ????d???i tils???n ???", 9), // mends
        construct_cw_output("??f ????sorr delhi??o? ???s  meadowsms ??? nye??????  noun ????d???i tils???n ???", 9), // sissy
        construct_cw_output("??f c???sorr delhi??o? ???s  meadowsms ??? nye??????  noun ????d???i tils???n ???", 9), // cd
        construct_cw_output("?ef c???sorr delhi?ao? ???s  meadowsms ??? nye??????  noun ????d???i tils???n ???", 9), // era
        construct_cw_output("?ef c???sorr delhi?ao? ???s  meadowsms ??? nyenticed  noun ????d???i tils???n ???", 9), // enticed
        construct_cw_output("?ef c?e?sorr delhi?ao? ?m?s  meadowsms ??? nyenticed  noun ????d???i tils???n ???", 9), // elmo
        construct_cw_output("?ef c?e?sorr delhi?ao? ?m?s  meadowsms ??? nyenticed  noun ????du??i tilst??n ???", 9), // snout
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms m?? nyenticed  noun ????du?ai tilst?rn ???", 9), // seminar
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun ????du?ai tilst?rn ???", 9), // mrs
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun ?a??du?ai tilst?rn a??", 9), // data
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedu?ai tilst?rn a??", 9), // take
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedu?ai tilst?rn at?", 9), // kit
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedu?ai tilst?rn at?", 9), // arc
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedu?ai tilstern at?", 9), // stern
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedu?ai tilstern atm", 9), // atm
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedu?ai tilstern atm", 9), // elm
        construct_cw_output("?ef c?e?sorr delhi?aos ?m?s  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // dubai
        construct_cw_output("?ef c?essorr delhi?aos ?mos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // shown
        construct_cw_output("?ef chessorr delhi?aos ?mos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // chess
        construct_cw_output("?ef chessorr delhi?aos ?mos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // tube
        construct_cw_output("?ef chessorr delhitaos ?mos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // taos
        construct_cw_output("?ef chessorr delhitaos amos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // amos
        construct_cw_output("def chessorr delhitaos amos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // dot
        construct_cw_output("def chessorr delhitaos amos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9), // headset
        construct_cw_output("def chessorr delhitaos amos  meadowsms mrs nyenticed  noun takedubai tilstern atm", 9)  // def
    };
    
    REQUIRE(driver->test_modification(9, 9, bridge_contents, bridge_assignments, bridge_ground_truth));
}

/**
 * test for crossword write_at()
*/
/*
TEST_CASE("crossword write_at", "[crossword],[quick]") {
    crossword_test_driver* driver = new crossword_test_driver("crossword write_at");

    stringstream input_1_1_wildcard;
    input_1_1_wildcard << WCD;
    stringstream expected_1_1_wildcard;
    expected_1_1_wildcard << endl << WILDCARD << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_wildcard.str(), expected_1_1_wildcard.str()));
    stringstream expected_1_1_wildcard_write_a;
    expected_1_1_wildcard_write_a << endl << 'a' << " ";
    REQUIRE(driver->test_write_at('a', 0, 0, expected_1_1_wildcard_write_a.str()));
    REQUIRE(driver->test_write_at('a', 0, 0, expected_1_1_wildcard_write_a.str()));
    stringstream expected_1_1_wildcard_write_b;
    expected_1_1_wildcard_write_b << endl << 'b' << " ";
    REQUIRE(driver->test_write_at('b', 0, 0, expected_1_1_wildcard_write_b.str()));
    REQUIRE(driver->test_write_at('b', 0, 0, expected_1_1_wildcard_write_b.str()));
    stringstream expected_1_1_wildcard_write_blk;
    expected_1_1_wildcard_write_blk << endl << BLK << " ";
    REQUIRE(driver->test_write_at(BLK, 0, 0, expected_1_1_wildcard_write_blk.str()));
    REQUIRE(driver->test_write_at(BLK, 0, 0, expected_1_1_wildcard_write_blk.str()));
    stringstream expected_1_1_wildcard_write_wcd;
    expected_1_1_wildcard_write_wcd << endl << WCD << " ";
    REQUIRE(driver->test_write_at(WCD, 0, 0, expected_1_1_wildcard_write_wcd.str()));
    REQUIRE(driver->test_write_at(WCD, 0, 0, expected_1_1_wildcard_write_wcd.str()));

    stringstream input_5_5_mixed;
    input_5_5_mixed << BLK << WCD << "cd" << WCD << 'f' << BLK << "hijk" << BLK << 'm' << BLK << 'o' << WCD << BLK << 'r' << WCD << BLK << WCD << BLK << "wxy";
    stringstream expected_5_5_mixed;
    expected_5_5_mixed << endl << BLK << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                       << endl << 'f' << " " << BLK << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                       << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                       << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                       << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'y' << " ";
    REQUIRE(driver->test_constructor_contents_string(5, 5, input_5_5_mixed.str(), expected_5_5_mixed.str()));
    stringstream expected_5_5_mixed_write_a;
    expected_5_5_mixed_write_a << endl << 'a' << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                               << endl << 'f' << " " << BLK << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                               << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                               << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                               << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'y' << " ";
    REQUIRE(driver->test_write_at('a', 0, 0, expected_5_5_mixed_write_a.str()));
    REQUIRE(driver->test_write_at('a', 0, 0, expected_5_5_mixed_write_a.str()));
    stringstream expected_5_5_mixed_write_x;
    expected_5_5_mixed_write_x << endl << 'a' << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                               << endl << 'f' << " " << BLK << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                               << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                               << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                               << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'y' << " ";
    REQUIRE(driver->test_write_at('x', 4, 3, expected_5_5_mixed_write_a.str()));
    REQUIRE(driver->test_write_at('x', 4, 3, expected_5_5_mixed_write_a.str()));
    stringstream expected_5_5_mixed_write_b;
    expected_5_5_mixed_write_b << endl << 'a' << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                               << endl << 'f' << " " << BLK << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                               << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                               << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                               << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'b' << " ";
    REQUIRE(driver->test_write_at('b', 4, 4, expected_5_5_mixed_write_b.str()));
    REQUIRE(driver->test_write_at('b', 4, 4, expected_5_5_mixed_write_b.str()));
    stringstream expected_5_5_mixed_write_blk;
    expected_5_5_mixed_write_blk << endl << 'a' << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                                 << endl << 'f' << " " << BLK << " " << 'h' << " " << BLK << " " << 'j' << " " 
                                 << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                                 << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                                 << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'b' << " ";
    REQUIRE(driver->test_write_at(BLK, 1, 3, expected_5_5_mixed_write_blk.str()));
    REQUIRE(driver->test_write_at(BLK, 1, 3, expected_5_5_mixed_write_blk.str()));
    stringstream expected_5_5_mixed_write_wcd;
    expected_5_5_mixed_write_wcd << endl << 'a' << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                                 << endl << 'f' << " " << BLK << " " << 'h' << " " << BLK << " " << 'j' << " " 
                                 << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                                 << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                                 << endl << WCD << " " << WCD << " " << 'w' << " " << 'x' << " " << 'b' << " ";
    REQUIRE(driver->test_write_at(WCD, 4, 1, expected_5_5_mixed_write_wcd.str()));
    REQUIRE(driver->test_write_at(WCD, 4, 1, expected_5_5_mixed_write_wcd.str()));
}
*/

/**
 * test for crossword read_at()
*/
/*
TEST_CASE("crossword read_at", "[crossword],[quick]") {
    crossword_test_driver* driver = new crossword_test_driver("crossword read_at");

    stringstream input_1_1_wildcard;
    input_1_1_wildcard << WCD;
    stringstream expected_1_1_wildcard;
    expected_1_1_wildcard << endl << WILDCARD << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_wildcard.str(), expected_1_1_wildcard.str()));
    REQUIRE(driver->test_read_at(0, 0, WCD));

    stringstream input_1_1_black;
    input_1_1_black << BLACK;
    stringstream expected_1_1_black;
    expected_1_1_black << endl << BLACK << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_black.str(), expected_1_1_black.str()));
    REQUIRE(driver->test_read_at(0, 0, BLK));

    stringstream input_1_1_x;
    input_1_1_x << 'x';
    stringstream expected_1_1_x;
    expected_1_1_x << endl << 'x' << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 1, input_1_1_x.str(), expected_1_1_x.str()));
    REQUIRE(driver->test_read_at(0, 0, 'x'));

    stringstream input_1_2_ab;
    input_1_2_ab << "ab";
    stringstream expected_1_2_ab;
    expected_1_2_ab << endl << 'a' << " " << endl << 'b' << " ";
    REQUIRE(driver->test_constructor_contents_string(1, 2, input_1_2_ab.str(), expected_1_2_ab.str()));
    REQUIRE(driver->test_read_at(0, 0, 'a'));
    REQUIRE(driver->test_read_at(1, 0, 'b'));

    stringstream input_5_5_mixed;
    input_5_5_mixed << BLK << WCD << "cd" << WCD << 'f' << BLK << "hijk" << BLK << 'm' << BLK << 'o' << WCD << BLK << 'r' << WCD << BLK << WCD << BLK << "wxy";
    stringstream expected_5_5_mixed;
    expected_5_5_mixed << endl << BLK << " " << WCD << " " << 'c' << " " << 'd' << " " << WCD << " " 
                       << endl << 'f' << " " << BLK << " " << 'h' << " " << 'i' << " " << 'j' << " " 
                       << endl << 'k' << " " << BLK << " " << 'm' << " " << BLK << " " << 'o' << " " 
                       << endl << WCD << " " << BLK << " " << 'r' << " " << WCD << " " << BLK << " " 
                       << endl << WCD << " " << BLK << " " << 'w' << " " << 'x' << " " << 'y' << " ";
    REQUIRE(driver->test_constructor_contents_string(5, 5, input_5_5_mixed.str(), expected_5_5_mixed.str()));
    REQUIRE(driver->test_read_at(0, 0, BLK));
    REQUIRE(driver->test_read_at(0, 1, WCD));
    REQUIRE(driver->test_read_at(0, 2, 'c'));
    REQUIRE(driver->test_read_at(0, 4, WCD));
    REQUIRE(driver->test_read_at(4, 0, WCD));
    REQUIRE(driver->test_read_at(4, 4, 'y'));
    REQUIRE(driver->test_read_at(2, 3, BLK));
    REQUIRE(driver->test_read_at(3, 2, 'r'));
}
*/
