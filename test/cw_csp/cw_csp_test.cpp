// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/14/2023
// Description: Catch2 tests for cw_csp class
// ==================================================================

#include "../catch/catch.hpp"
#include "cw_csp_test_driver.h"

using namespace cw_csp_test_driver_ns;

/**
 * hello world test for cw_csp
*/
TEST_CASE("cw_csp hello_world", "[cw_csp],[hello_world],[quick]") {
    REQUIRE(true);
}

/**
 * constructor test for cw_csp without contents
*/
/*
TEST_CASE("cw_csp constructor_no_contents", "[cw_csp],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp constructor_no_contents");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";

    // 1x1 blank crossword
    vector<unique_ptr<cw_variable> > vars_1_1 = {}; // no words with len > 1
    vector<unique_ptr<cw_constraint> > contrs_1_1 = {}; // no variables
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_1_1 = {}; // no constraints
    unordered_set<unique_ptr<cw_variable> >   vars_1_1_set  (vars_1_1.begin(),   vars_1_1.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_1_1_set(contrs_1_1.begin(), contrs_1_1.end());
    REQUIRE(dut->test_constructor_empty(1, 1, dict_barebones_path, vars_1_1_set, contrs_1_1_set, arc_dep_1_1));

    // 2x2 blank crossword
    vector<unique_ptr<cw_variable> > vars_2_2 = {
        make_unique<cw_variable>(0l, 0, 0, 2, VERTICAL,   unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
        make_unique<cw_variable>(1l, 0, 1, 2, VERTICAL,   unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
        make_unique<cw_variable>(2l, 0, 0, 2, HORIZONTAL, unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
        make_unique<cw_variable>(3l, 1, 0, 2, HORIZONTAL, unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_2_2 = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l, 0, 0, 0, 2), // 0
        make_unique<cw_arc>(1l, 0, 1, 1, 2), // 1
        make_unique<cw_arc>(2l, 1, 0, 0, 3), // 2
        make_unique<cw_arc>(3l, 1, 1, 1, 3), // 3

        // backward (horizontal --> vertical)
        make_unique<cw_arc>(4l, 0, 0, 2, 0), // 4
        make_unique<cw_arc>(5l, 1, 0, 2, 1), // 5
        make_unique<cw_arc>(6l, 0, 1, 3, 0), // 6
        make_unique<cw_arc>(7l, 1, 1, 3, 1), // 7
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_2_2 = {
        {vars_2_2[0], {contrs_2_2[4], contrs_2_2[6]}},
        {vars_2_2[1], {contrs_2_2[5], contrs_2_2[7]}},
        {vars_2_2[2], {contrs_2_2[0], contrs_2_2[1]}},
        {vars_2_2[3], {contrs_2_2[2], contrs_2_2[3]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_2_2_set  (vars_2_2.begin(),   vars_2_2.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_2_2_set(contrs_2_2.begin(), contrs_2_2.end());
    REQUIRE(dut->test_constructor_empty(2, 2, dict_barebones_path, vars_2_2_set, contrs_2_2_set, arc_dep_2_2));

    // 3x3 blank crossword
    vector<unique_ptr<cw_variable> > vars_3_3 = {
        make_unique<cw_variable>(0l, 0, 0, 3, VERTICAL,   unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(1l, 0, 1, 3, VERTICAL,   unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(2l, 0, 2, 3, VERTICAL,   unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(3l, 0, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(4l, 1, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(5l, 2, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_3_3 = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l,  0, 0, 0, 3), // 0
        make_unique<cw_arc>(1l,  0, 1, 1, 3), // 1
        make_unique<cw_arc>(2l,  0, 2, 2, 3), // 2
        make_unique<cw_arc>(3l,  1, 0, 0, 4), // 3
        make_unique<cw_arc>(4l,  1, 1, 1, 4), // 4
        make_unique<cw_arc>(5l,  1, 2, 2, 4), // 5
        make_unique<cw_arc>(6l,  2, 0, 0, 5), // 6
        make_unique<cw_arc>(7l,  2, 1, 1, 5), // 7
        make_unique<cw_arc>(8l,  2, 2, 2, 5), // 8

        // backward (horizontal --> vertical)
        make_unique<cw_arc>(9l,  0, 0, 3, 0), // 9
        make_unique<cw_arc>(10l, 1, 0, 3, 1), // 10
        make_unique<cw_arc>(11l, 2, 0, 3, 2), // 11
        make_unique<cw_arc>(12l, 0, 1, 4, 0), // 12
        make_unique<cw_arc>(13l, 1, 1, 4, 1), // 13
        make_unique<cw_arc>(14l, 2, 1, 4, 2), // 14
        make_unique<cw_arc>(15l, 0, 2, 5, 0), // 15
        make_unique<cw_arc>(16l, 1, 2, 5, 1), // 16
        make_unique<cw_arc>(17l, 2, 2, 5, 2), // 17
        
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_3_3 = {
        {vars_3_3[0], {contrs_3_3[9], contrs_3_3[12], contrs_3_3[15]}},
        {vars_3_3[1], {contrs_3_3[10], contrs_3_3[13], contrs_3_3[16]}},
        {vars_3_3[2], {contrs_3_3[11], contrs_3_3[14], contrs_3_3[17]}},
        {vars_3_3[3], {contrs_3_3[0], contrs_3_3[1], contrs_3_3[2]}},
        {vars_3_3[4], {contrs_3_3[3], contrs_3_3[4], contrs_3_3[5]}},
        {vars_3_3[5], {contrs_3_3[6], contrs_3_3[7], contrs_3_3[8]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_3_3_set  (vars_3_3.begin(),   vars_3_3.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_3_3_set(contrs_3_3.begin(), contrs_3_3.end());
    REQUIRE(dut->test_constructor_empty(3, 3, dict_barebones_path, vars_3_3_set, contrs_3_3_set, arc_dep_3_3));

    // 3x4 blank crossword
    vector<unique_ptr<cw_variable> > vars_3_4 = {
        make_unique<cw_variable>(0l, 0, 0, 4, VERTICAL,   unordered_set<word_t>{word_t("cars"), word_t("caps"), word_t("core"), word_t("door"), word_t("boob"), word_t("been"), word_t("node")}),
        make_unique<cw_variable>(1l, 0, 1, 4, VERTICAL,   unordered_set<word_t>{word_t("cars"), word_t("caps"), word_t("core"), word_t("door"), word_t("boob"), word_t("been"), word_t("node")}),
        make_unique<cw_variable>(2l, 0, 2, 4, VERTICAL,   unordered_set<word_t>{word_t("cars"), word_t("caps"), word_t("core"), word_t("door"), word_t("boob"), word_t("been"), word_t("node")}),
        make_unique<cw_variable>(3l, 0, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(4l, 1, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(5l, 2, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(6l, 3, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_3_4 = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l,  0, 0, 0, 3), // 0
        make_unique<cw_arc>(1l,  0, 1, 1, 3), // 1
        make_unique<cw_arc>(2l,  0, 2, 2, 3), // 2
        make_unique<cw_arc>(3l,  1, 0, 0, 4), // 3
        make_unique<cw_arc>(4l,  1, 1, 1, 4), // 4
        make_unique<cw_arc>(5l,  1, 2, 2, 4), // 5
        make_unique<cw_arc>(6l,  2, 0, 0, 5), // 6
        make_unique<cw_arc>(7l,  2, 1, 1, 5), // 7
        make_unique<cw_arc>(8l,  2, 2, 2, 5), // 8
        make_unique<cw_arc>(9l,  3, 0, 0, 6), // 9
        make_unique<cw_arc>(10l, 3, 1, 1, 6), // 10
        make_unique<cw_arc>(11l, 3, 2, 2, 6), // 11
        
        // backward (horizontal --> vertical)
        make_unique<cw_arc>(12l, 0, 0, 3, 0), // 12
        make_unique<cw_arc>(13l, 1, 0, 3, 1), // 13
        make_unique<cw_arc>(14l, 2, 0, 3, 2), // 14
        make_unique<cw_arc>(15l, 0, 1, 4, 0), // 15
        make_unique<cw_arc>(16l, 1, 1, 4, 1), // 16
        make_unique<cw_arc>(17l, 2, 1, 4, 2), // 17
        make_unique<cw_arc>(18l, 0, 2, 5, 0), // 18
        make_unique<cw_arc>(19l, 1, 2, 5, 1), // 19
        make_unique<cw_arc>(20l, 2, 2, 5, 2), // 20
        make_unique<cw_arc>(21l, 0, 3, 6, 0), // 21
        make_unique<cw_arc>(22l, 1, 3, 6, 1), // 22
        make_unique<cw_arc>(23l, 2, 3, 6, 2), // 23
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_3_4 = {
        {vars_3_4[0], {contrs_3_4[12], contrs_3_4[15], contrs_3_4[18], contrs_3_4[21]}},
        {vars_3_4[1], {contrs_3_4[13], contrs_3_4[16], contrs_3_4[19], contrs_3_4[22]}},
        {vars_3_4[2], {contrs_3_4[14], contrs_3_4[17], contrs_3_4[20], contrs_3_4[23]}},
        {vars_3_4[3], {contrs_3_4[0], contrs_3_4[1], contrs_3_4[2]}},
        {vars_3_4[4], {contrs_3_4[3], contrs_3_4[4], contrs_3_4[5]}},
        {vars_3_4[5], {contrs_3_4[6], contrs_3_4[7], contrs_3_4[8]}},
        {vars_3_4[6], {contrs_3_4[9], contrs_3_4[10], contrs_3_4[11]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_3_4_set  (vars_3_4.begin(),   vars_3_4.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_3_4_set(contrs_3_4.begin(), contrs_3_4.end());
    REQUIRE(dut->test_constructor_empty(3, 4, dict_barebones_path, vars_3_4_set, contrs_3_4_set, arc_dep_3_4));
}
*/

/**
 * constructor test for cw_csp with contents
*/
/*
TEST_CASE("cw_csp constructor_with_contents", "[cw_csp],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp constructor_with_contents");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";

    // baseline 2x2 blank crossword
    vector<unique_ptr<cw_variable> > vars_2_2 = {
        make_unique<cw_variable>(0l, 0, 0, 2, VERTICAL,   unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
        make_unique<cw_variable>(1l, 0, 1, 2, VERTICAL,   unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
        make_unique<cw_variable>(2l, 0, 0, 2, HORIZONTAL, unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
        make_unique<cw_variable>(3l, 1, 0, 2, HORIZONTAL, unordered_set<word_t>{word_t("an"), word_t("at"), word_t("on"), word_t("to")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_2_2 = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l, 0, 0, 0, 2), // 0
        make_unique<cw_arc>(1l, 0, 1, 1, 2), // 1
        make_unique<cw_arc>(2l, 1, 0, 0, 3), // 2
        make_unique<cw_arc>(3l, 1, 1, 1, 3), // 3

        // backward (horizontal --> vertical)
        make_unique<cw_arc>(4l, 0, 0, 2, 0), // 4
        make_unique<cw_arc>(5l, 1, 0, 2, 1), // 5
        make_unique<cw_arc>(6l, 0, 1, 3, 0), // 6
        make_unique<cw_arc>(7l, 1, 1, 3, 1), // 7
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_2_2 = {
        {vars_2_2[0], {contrs_2_2[4], contrs_2_2[6]}},
        {vars_2_2[1], {contrs_2_2[5], contrs_2_2[7]}},
        {vars_2_2[2], {contrs_2_2[0], contrs_2_2[1]}},
        {vars_2_2[3], {contrs_2_2[2], contrs_2_2[3]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_2_2_set  (vars_2_2.begin(),   vars_2_2.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_2_2_set(contrs_2_2.begin(), contrs_2_2.end());
    stringstream contents_2_2;
    contents_2_2 << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_constructor_contents(2, 2, contents_2_2.str(), dict_barebones_path, vars_2_2_set, contrs_2_2_set, arc_dep_2_2));

    // baseline 3x4 blank crossword
    vector<unique_ptr<cw_variable> > vars_3_4 = {
        make_unique<cw_variable>(0l, 0, 0, 4, VERTICAL,   unordered_set<word_t>{word_t("cars"), word_t("caps"), word_t("core"), word_t("door"), word_t("boob"), word_t("been"), word_t("node")}),
        make_unique<cw_variable>(1l, 0, 1, 4, VERTICAL,   unordered_set<word_t>{word_t("cars"), word_t("caps"), word_t("core"), word_t("door"), word_t("boob"), word_t("been"), word_t("node")}),
        make_unique<cw_variable>(2l, 0, 2, 4, VERTICAL,   unordered_set<word_t>{word_t("cars"), word_t("caps"), word_t("core"), word_t("door"), word_t("boob"), word_t("been"), word_t("node")}),
        make_unique<cw_variable>(3l, 0, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(4l, 1, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(5l, 2, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(6l, 3, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_3_4 = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l,  0, 0, 0, 3), // 0
        make_unique<cw_arc>(1l,  0, 1, 1, 3), // 1
        make_unique<cw_arc>(2l,  0, 2, 2, 3), // 2
        make_unique<cw_arc>(3l,  1, 0, 0, 4), // 3
        make_unique<cw_arc>(4l,  1, 1, 1, 4), // 4
        make_unique<cw_arc>(5l,  1, 2, 2, 4), // 5
        make_unique<cw_arc>(6l,  2, 0, 0, 5), // 6
        make_unique<cw_arc>(7l,  2, 1, 1, 5), // 7
        make_unique<cw_arc>(8l,  2, 2, 2, 5), // 8
        make_unique<cw_arc>(9l,  3, 0, 0, 6), // 9
        make_unique<cw_arc>(10l, 3, 1, 1, 6), // 10
        make_unique<cw_arc>(11l, 3, 2, 2, 6), // 11
        
        // backward (horizontal --> vertical)
        make_unique<cw_arc>(12l, 0, 0, 3, 0), // 12
        make_unique<cw_arc>(13l, 1, 0, 3, 1), // 13
        make_unique<cw_arc>(14l, 2, 0, 3, 2), // 14
        make_unique<cw_arc>(15l, 0, 1, 4, 0), // 15
        make_unique<cw_arc>(16l, 1, 1, 4, 1), // 16
        make_unique<cw_arc>(17l, 2, 1, 4, 2), // 17
        make_unique<cw_arc>(18l, 0, 2, 5, 0), // 18
        make_unique<cw_arc>(19l, 1, 2, 5, 1), // 19
        make_unique<cw_arc>(20l, 2, 2, 5, 2), // 20
        make_unique<cw_arc>(21l, 0, 3, 6, 0), // 21
        make_unique<cw_arc>(22l, 1, 3, 6, 1), // 22
        make_unique<cw_arc>(23l, 2, 3, 6, 2), // 23
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_3_4 = {
        {vars_3_4[0], {contrs_3_4[12], contrs_3_4[15], contrs_3_4[18], contrs_3_4[21]}},
        {vars_3_4[1], {contrs_3_4[13], contrs_3_4[16], contrs_3_4[19], contrs_3_4[22]}},
        {vars_3_4[2], {contrs_3_4[14], contrs_3_4[17], contrs_3_4[20], contrs_3_4[23]}},
        {vars_3_4[3], {contrs_3_4[0], contrs_3_4[1], contrs_3_4[2]}},
        {vars_3_4[4], {contrs_3_4[3], contrs_3_4[4], contrs_3_4[5]}},
        {vars_3_4[5], {contrs_3_4[6], contrs_3_4[7], contrs_3_4[8]}},
        {vars_3_4[6], {contrs_3_4[9], contrs_3_4[10], contrs_3_4[11]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_3_4_set  (vars_3_4.begin(),   vars_3_4.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_3_4_set(contrs_3_4.begin(), contrs_3_4.end());
    stringstream contents_3_4;
    contents_3_4 << WCD << WCD << WCD
                 << WCD << WCD << WCD
                 << WCD << WCD << WCD
                 << WCD << WCD << WCD;
    REQUIRE(dut->test_constructor_contents(3, 4, contents_3_4.str(), dict_barebones_path, vars_3_4_set, contrs_3_4_set, arc_dep_3_4));

    // 3x4 crossword of larger size with black tiles blocking it
    stringstream contents_3_4_black_border;
    contents_3_4_black_border << WCD << WCD << WCD << BLK << BLK 
                              << WCD << WCD << WCD << BLK << BLK 
                              << WCD << WCD << WCD << BLK << BLK 
                              << WCD << WCD << WCD << BLK << BLK 
                              << BLK << BLK << BLK << BLK << BLK
                              << BLK << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_constructor_contents(5, 6, contents_3_4_black_border.str(), dict_barebones_path, vars_3_4_set, contrs_3_4_set, arc_dep_3_4));

    // 3x4 crossword embedded in a donut of black tiles and random single wildcards
    stringstream contents_3_4_black_border_with_dots;
    contents_3_4_black_border_with_dots << WCD << WCD << WCD << BLK << WCD << BLK << WCD
                                        << WCD << WCD << WCD << BLK << BLK << WCD << BLK
                                        << WCD << WCD << WCD << BLK << WCD << BLK << WCD
                                        << WCD << WCD << WCD << BLK << BLK << WCD << BLK
                                        << BLK << BLK << BLK << WCD << BLK << BLK << WCD
                                        << BLK << WCD << BLK << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_constructor_contents(7, 6, contents_3_4_black_border_with_dots.str(), dict_barebones_path, vars_3_4_set, contrs_3_4_set, arc_dep_3_4));

    // 3x3 donut crossword
    vector<unique_ptr<cw_variable> > vars_3_3_donut = {
        make_unique<cw_variable>(0l, 0, 0, 3, VERTICAL,   unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(1l, 0, 2, 3, VERTICAL,   unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(2l, 0, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
        make_unique<cw_variable>(3l, 2, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_3_3_donut = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l, 0, 0, 0, 2), // 0
        make_unique<cw_arc>(1l, 2, 0, 0, 3), // 1
        make_unique<cw_arc>(2l, 0, 2, 1, 2), // 2
        make_unique<cw_arc>(3l, 2, 2, 1, 3), // 3

        // backward (horizontal --> vertical)
        make_unique<cw_arc>(4l, 0, 0, 2, 0), // 4
        make_unique<cw_arc>(5l, 0, 2, 3, 0), // 5
        make_unique<cw_arc>(6l, 2, 0, 2, 1), // 6
        make_unique<cw_arc>(7l, 2, 2, 3, 1), // 7
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_3_3_donut = {
        {vars_3_3_donut[0], {contrs_3_3_donut[4], contrs_3_3_donut[5]}},
        {vars_3_3_donut[1], {contrs_3_3_donut[6], contrs_3_3_donut[7]}},
        {vars_3_3_donut[2], {contrs_3_3_donut[0], contrs_3_3_donut[2]}},
        {vars_3_3_donut[3], {contrs_3_3_donut[1], contrs_3_3_donut[3]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_3_3_donut_set  (vars_3_3_donut.begin(),   vars_3_3_donut.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_3_3_donut_set(contrs_3_3_donut.begin(), contrs_3_3_donut.end());
    stringstream contents_3_3_donut;
    contents_3_3_donut << WCD << WCD << WCD 
                       << WCD << BLK << WCD 
                       << WCD << WCD << WCD;
    REQUIRE(dut->test_constructor_contents(3, 3, contents_3_3_donut.str(), dict_barebones_path, vars_3_3_donut_set, contrs_3_3_donut_set, arc_dep_3_3_donut));

    // single cross crossword with a single letter filled in
    vector<unique_ptr<cw_variable> > vars_5_6_cross = {
        make_unique<cw_variable>(0l, 1, 2, 4, VERTICAL,   unordered_set<word_t>{word_t("boob"), word_t("been")}),
        make_unique<cw_variable>(1l, 2, 1, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("cat"), word_t("cab"), word_t("can"), word_t("cup"), word_t("cub"), word_t("dan"), word_t("dab")}),
    };
    vector<unique_ptr<cw_constraint> > contrs_5_6_cross = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l, 1, 1, 0, 1),

        // backward (horizontal --> vertical)
        make_unique<cw_arc>(1l, 1, 1, 1, 0),
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_5_6_cross = {
        {vars_5_6_cross[0], {contrs_5_6_cross[1]}},
        {vars_5_6_cross[1], {contrs_5_6_cross[0]}}
    };
    unordered_set<unique_ptr<cw_variable> >   vars_5_6_cross_set  (vars_5_6_cross.begin(),   vars_5_6_cross.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_5_6_cross_set(contrs_5_6_cross.begin(), contrs_5_6_cross.end());
    stringstream contents_5_6_cross;
    contents_5_6_cross << BLK << BLK << BLK << BLK << BLK 
                       << BLK << BLK << 'b' << BLK << BLK
                       << BLK << WCD << WCD << WCD << BLK
                       << BLK << BLK << WCD << BLK << BLK
                       << BLK << BLK << WCD << BLK << BLK
                       << BLK << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_constructor_contents(5, 6, contents_5_6_cross.str(), dict_barebones_path, vars_5_6_cross_set, contrs_5_6_cross_set, arc_dep_5_6_cross));

    // simple crossword with single variables & no constraints
    vector<unique_ptr<cw_variable> > vars_5_3_no_constraints = {
        make_unique<cw_variable>(0l, 0, 0, 3, VERTICAL,   unordered_set<word_t>{word_t("eta")}), // 0
        make_unique<cw_variable>(1l, 0, 2, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("are"), word_t("bed"), word_t("cab"), word_t("can"), word_t("cat"), word_t("cub"), word_t("cup"), word_t("dab"), word_t("dan"), word_t("eta"), word_t("gap"), word_t("pot")}), // 14
        make_unique<cw_variable>(2l, 2, 2, 3, HORIZONTAL, unordered_set<word_t>{word_t("cab"), word_t("can"), word_t("cat"), word_t("dab"), word_t("dan"), word_t("gap")}), // 14
    };
    vector<unique_ptr<cw_constraint> > contrs_5_3_no_constraints = {}; // no intersections between words
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_5_3_no_constraints = {}; // no constraints
    unordered_set<unique_ptr<cw_variable> >   vars_5_3_no_constraints_set  (vars_5_3_no_constraints.begin(),   vars_5_3_no_constraints.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_5_3_no_constraints_set(contrs_5_3_no_constraints.begin(), contrs_5_3_no_constraints.end());
    stringstream contents_5_3_no_constraints;
    contents_5_3_no_constraints << 'e' << BLK << WCD << WCD << WCD
                                << 't' << BLK << BLK << BLK << BLK
                                << 'a' << BLK << WCD << 'a' << WCD;
    REQUIRE(dut->test_constructor_contents(5, 3, contents_5_3_no_constraints.str(), dict_simple_path, vars_5_3_no_constraints_set, contrs_5_3_no_constraints_set, arc_dep_5_3_no_constraints));

    // 6x7 crossword with complex intersections & mix of wildcards and letters
    vector<unique_ptr<cw_variable> > vars_6_7_complex = {
        make_unique<cw_variable>(0l,  0, 0, 4, VERTICAL,   unordered_set<word_t>{word_t("atol"), word_t("atom")}), // 0
        make_unique<cw_variable>(1l,  5, 0, 2, VERTICAL,   unordered_set<word_t>{word_t("ad"), word_t("an"), word_t("at"), word_t("be"), word_t("on"), word_t("to")}), // 1
        make_unique<cw_variable>(2l,  3, 1, 4, VERTICAL,   unordered_set<word_t>{word_t("plan")}), // 2
        make_unique<cw_variable>(3l,  4, 2, 3, VERTICAL,   unordered_set<word_t>{}), // 3
        make_unique<cw_variable>(4l,  0, 3, 3, VERTICAL,   unordered_set<word_t>{word_t("bed")}), // 4
        make_unique<cw_variable>(5l,  2, 4, 4, VERTICAL,   unordered_set<word_t>{word_t("boob"), word_t("knob")}), // 5
        make_unique<cw_variable>(6l,  0, 5, 4, VERTICAL,   unordered_set<word_t>{word_t("pole"), word_t("pore")}), // 6
        make_unique<cw_variable>(7l,  5, 5, 2, VERTICAL,   unordered_set<word_t>{word_t("an")}), // 7
        make_unique<cw_variable>(8l,  0, 3, 3, HORIZONTAL, unordered_set<word_t>{word_t("gap"), word_t("cup")}), // 8
        make_unique<cw_variable>(9l,  1, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("tore"), word_t("tote")}), // 9
        make_unique<cw_variable>(10l, 2, 3, 3, HORIZONTAL, unordered_set<word_t>{word_t("ace"), word_t("and"), word_t("are"), word_t("bed"), word_t("cab"), word_t("can"), word_t("cat"), word_t("cub"), word_t("cup"), word_t("dab"), word_t("dan"), word_t("eta"), word_t("gap"), word_t("pot")}), // 10
        make_unique<cw_variable>(11l, 3, 0, 2, HORIZONTAL, unordered_set<word_t>{word_t("ad"), word_t("an"), word_t("at"), word_t("be"), word_t("on"), word_t("to")}), // 11
        make_unique<cw_variable>(12l, 3, 4, 2, HORIZONTAL, unordered_set<word_t>{word_t("be")}), // 12
        make_unique<cw_variable>(13l, 4, 1, 4, HORIZONTAL, unordered_set<word_t>{word_t("halo")}), // 13
        make_unique<cw_variable>(14l, 5, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("cat")}), // 14
        make_unique<cw_variable>(15l, 5, 4, 2, HORIZONTAL, unordered_set<word_t>{}), // 15
        make_unique<cw_variable>(16l, 6, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("knob"), word_t("know")}), // 16
    };
    vector<unique_ptr<cw_constraint> > contrs_6_7_complex = {
        // forward (vertical --> horizontal)
        make_unique<cw_arc>(0l,  0, 0, 4, 8), // 0
        make_unique<cw_arc>(1l,  0, 2, 6, 8), // 1
        make_unique<cw_arc>(2l,  1, 0, 0, 9), // 2
        make_unique<cw_arc>(3l,  1, 3, 4, 9), // 3
        make_unique<cw_arc>(4l,  2, 0, 4, 10), // 4
        make_unique<cw_arc>(5l,  0, 1, 5, 10), // 5
        make_unique<cw_arc>(6l,  2, 2, 6, 10), // 6
        make_unique<cw_arc>(7l,  3, 0, 0, 11), // 7
        make_unique<cw_arc>(8l,  0, 1, 2, 11), // 8
        make_unique<cw_arc>(9l,  1, 0, 5, 12), // 9
        make_unique<cw_arc>(10l, 3, 1, 6, 12), // 10
        make_unique<cw_arc>(11l, 1, 0, 2, 13), // 11
        make_unique<cw_arc>(12l, 0, 1, 3, 13), // 12
        make_unique<cw_arc>(13l, 2, 3, 5, 13), // 13
        make_unique<cw_arc>(14l, 0, 0, 1, 14), // 14
        make_unique<cw_arc>(15l, 2, 1, 2, 14), // 15
        make_unique<cw_arc>(16l, 1, 2, 3, 14), // 16
        make_unique<cw_arc>(17l, 3, 0, 5, 15), // 17
        make_unique<cw_arc>(18l, 0, 1, 7, 15), // 18
        make_unique<cw_arc>(19l, 1, 0, 1, 16), // 19
        make_unique<cw_arc>(20l, 3, 1, 2, 16), // 20
        make_unique<cw_arc>(21l, 2, 2, 3, 16), // 21

        // backward (horizontal --> vertical)
        make_unique<cw_arc>(22l, 0, 0, 8, 4), // 22
        make_unique<cw_arc>(23l, 2, 0, 8, 6), // 23
        make_unique<cw_arc>(24l, 0, 1, 9, 0), // 24
        make_unique<cw_arc>(25l, 3, 1, 9, 4), // 25
        make_unique<cw_arc>(26l, 0, 2, 10, 4), // 26
        make_unique<cw_arc>(27l, 1, 0, 10, 5), // 27
        make_unique<cw_arc>(28l, 2, 2, 10, 6), // 28
        make_unique<cw_arc>(29l, 0, 3, 11, 0), // 29
        make_unique<cw_arc>(30l, 1, 0, 11, 2), // 30
        make_unique<cw_arc>(31l, 0, 1, 12, 5), // 31
        make_unique<cw_arc>(32l, 1, 3, 12, 6), // 32
        make_unique<cw_arc>(33l, 0, 1, 13, 2), // 33
        make_unique<cw_arc>(34l, 1, 0, 13, 3), // 34
        make_unique<cw_arc>(35l, 3, 2, 13, 5), // 35
        make_unique<cw_arc>(36l, 0, 0, 14, 1), // 36
        make_unique<cw_arc>(37l, 1, 2, 14, 2), // 37
        make_unique<cw_arc>(38l, 2, 1, 14, 3), // 38
        make_unique<cw_arc>(39l, 0, 3, 15, 5), // 39
        make_unique<cw_arc>(40l, 1, 0, 15, 7), // 40
        make_unique<cw_arc>(41l, 0, 1, 16, 1), // 41
        make_unique<cw_arc>(42l, 1, 3, 16, 2), // 42
        make_unique<cw_arc>(43l, 2, 2, 16, 3), // 43
    };
    unordered_map<unique_ptr<cw_variable>, unordered_set<unique_ptr<cw_constraint> > > arc_dep_6_7_complex = {
        {vars_6_7_complex[0], {contrs_6_7_complex[24], contrs_6_7_complex[29]}},
        {vars_6_7_complex[1], {contrs_6_7_complex[36], contrs_6_7_complex[41]}},
        {vars_6_7_complex[2], {contrs_6_7_complex[30], contrs_6_7_complex[33], contrs_6_7_complex[37], contrs_6_7_complex[42]}},
        {vars_6_7_complex[3], {contrs_6_7_complex[34], contrs_6_7_complex[38], contrs_6_7_complex[43]}},
        {vars_6_7_complex[4], {contrs_6_7_complex[22], contrs_6_7_complex[25], contrs_6_7_complex[26]}},
        {vars_6_7_complex[5], {contrs_6_7_complex[27], contrs_6_7_complex[31], contrs_6_7_complex[35], contrs_6_7_complex[39]}},
        {vars_6_7_complex[6], {contrs_6_7_complex[23], contrs_6_7_complex[28], contrs_6_7_complex[32]}},
        {vars_6_7_complex[7], {contrs_6_7_complex[40]}},
        {vars_6_7_complex[8], {contrs_6_7_complex[0], contrs_6_7_complex[1]}},
        {vars_6_7_complex[9], {contrs_6_7_complex[2], contrs_6_7_complex[3]}},
        {vars_6_7_complex[10], {contrs_6_7_complex[4], contrs_6_7_complex[5], contrs_6_7_complex[6]}},
        {vars_6_7_complex[11], {contrs_6_7_complex[7], contrs_6_7_complex[8]}},
        {vars_6_7_complex[12], {contrs_6_7_complex[9], contrs_6_7_complex[10]}},
        {vars_6_7_complex[13], {contrs_6_7_complex[11], contrs_6_7_complex[12], contrs_6_7_complex[13]}},
        {vars_6_7_complex[14], {contrs_6_7_complex[14], contrs_6_7_complex[15], contrs_6_7_complex[16]}},
        {vars_6_7_complex[15], {contrs_6_7_complex[17], contrs_6_7_complex[18]}},
        {vars_6_7_complex[16], {contrs_6_7_complex[19], contrs_6_7_complex[20], contrs_6_7_complex[21]}},
    };
    unordered_set<unique_ptr<cw_variable> >   vars_6_7_complex_set  (vars_6_7_complex.begin(),   vars_6_7_complex.end());
    unordered_set<unique_ptr<cw_constraint> > contrs_6_7_complex_set(contrs_6_7_complex.begin(), contrs_6_7_complex.end());
    stringstream contents_6_7_complex;
    contents_6_7_complex << WCD << BLK << BLK << WCD << WCD << 'p' 
                         << 't' << WCD << WCD << 'e' << BLK << WCD 
                         << WCD << BLK << BLK << WCD << WCD << WCD 
                         << WCD << WCD << BLK << BLK << WCD << 'e' 
                         << BLK << WCD << 'a' << WCD << 'o' << BLK 
                         << WCD << 'a' << 't' << BLK << 'b' << 'a' 
                         << WCD << 'n' << WCD << WCD << BLK << 'n';
    REQUIRE(dut->test_constructor_contents(6, 7, contents_6_7_complex.str(), dict_simple_path, vars_6_7_complex_set, contrs_6_7_complex_set, arc_dep_6_7_complex));
}
*/

/**
 * simple AC-3 test for cw_csp for valid/invalid checking
*/
TEST_CASE("cw_csp ac3_valid_check", "[cw_csp],[ac3],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp ac3_valid_check");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";

    // ############### invalid crosswords ###############

    // invalid 6x7 crossword with complex intersections & mix of wildcards and letters
    stringstream contents_6_7_complex_invalid;
    contents_6_7_complex_invalid << WCD << BLK << BLK << WCD << WCD << 'p' 
                                 << 't' << WCD << WCD << 'e' << BLK << WCD 
                                 << WCD << BLK << BLK << WCD << WCD << WCD 
                                 << WCD << WCD << BLK << BLK << WCD << 'e' 
                                 << BLK << WCD << 'a' << WCD << 'o' << BLK 
                                 << WCD << 'a' << 't' << BLK << 'b' << 'a' 
                                 << WCD << 'n' << WCD << WCD << BLK << 'n';
    REQUIRE(dut->test_ac3_validity(6, 7, contents_6_7_complex_invalid.str(), dict_simple_path, false));

    // simple invalid 2x2 crossword
    stringstream contents_2_2_invalid;
    contents_2_2_invalid << WCD << BLK 
                         << WCD << 't';
    REQUIRE(dut->test_ac3_validity(2, 2, contents_2_2_invalid.str(), dict_barebones_path, false));

    // initially invalid 4x3 crossword
    stringstream contents_4_3_invalid;
    contents_4_3_invalid << 'x' << 'y' << WCD << 'z'
                         << WCD << WCD << WCD << WCD 
                         << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_ac3_validity(4, 3, contents_4_3_invalid.str(), dict_simple_path, false));

    // ############### valid crosswords ###############

    // simple valid 2x2 crossword
    stringstream contents_2_2_valid;
    contents_2_2_valid << WCD << BLK 
                       << WCD << WCD;
    REQUIRE(dut->test_ac3_validity(2, 2, contents_2_2_valid.str(), dict_barebones_path, true));

    // simple valid blank 3x3 crossword
    stringstream contents_3_3_blank_valid;
    contents_3_3_blank_valid << WCD << BLK << WCD 
                             << WCD << WCD << WCD 
                             << WCD << BLK << WCD;
    REQUIRE(dut->test_ac3_validity(3, 3, contents_3_3_blank_valid.str(), dict_simple_path, true));

    // simple valid blank 4x4 crossword
    stringstream contents_4_4_blank_valid;
    contents_4_4_blank_valid << WCD << WCD << WCD << WCD 
                             << WCD << BLK << BLK << WCD 
                             << WCD << BLK << BLK << WCD 
                             << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_ac3_validity(4, 4, contents_4_4_blank_valid.str(), dict_simple_path, true));

    // valid 6x7 crossword with complex intersections & mix of wildcards and letters
    stringstream contents_6_7_complex_valid;
    contents_6_7_complex_valid << WCD << BLK << BLK << WCD << BLK << 'p' 
                               << 't' << WCD << WCD << 'e' << BLK << BLK 
                               << WCD << BLK << BLK << WCD << WCD << WCD 
                               << WCD << BLK << WCD << BLK << WCD << BLK 
                               << BLK << BLK << WCD << BLK << 'o' << BLK 
                               << WCD << 'a' << 't' << BLK << WCD << BLK 
                               << BLK << 'n' << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_ac3_validity(6, 7, contents_6_7_complex_valid.str(), dict_simple_path, true));
}

/**
 * simple AC-3 test for cw_csp for constraint duplicate word prevention
*/
TEST_CASE("cw_csp ac3_valid_constraint_duplicates", "[cw_csp],[ac3],[duplicates],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp ac3_valid_constraint_duplicates");
    const string dict_single_word = "cw_csp/data/dict_single_word.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";

    // ############### valid crosswords ###############

    // simple crossword that requires duplicates
    // note: while this is an illegal configuration, word_domain cannot detect variables in a 
    //  constraint using the same word so it cannot detect that this is illegal, so expected is true
    stringstream contents_2_2_duplicate_invalid;
    contents_2_2_duplicate_invalid << WCD << WCD
                                   << WCD << WCD;
    REQUIRE(dut->test_ac3_validity(2, 2, contents_2_2_duplicate_invalid.str(), dict_single_word, true));

    // valid 6x7 crossword with complex intersections & mix of wildcards and letters
    // forces two words to both be 'cat', but is allowed due to same reason as above
    stringstream contents_6_7_complex_valid;
    contents_6_7_complex_valid << WCD << BLK << BLK << WCD << BLK << 'p' 
                               << 't' << WCD << WCD << 'e' << BLK << BLK 
                               << WCD << BLK << BLK << WCD << WCD << WCD 
                               << WCD << BLK << WCD << BLK << BLK << BLK 
                               << BLK << BLK << WCD << BLK << WCD << WCD 
                               << WCD << 'a' << 't' << BLK << BLK << BLK 
                               << BLK << 'n' << BLK << WCD << 'a' << 't';
    REQUIRE(dut->test_ac3_validity(6, 7, contents_6_7_complex_valid.str(), dict_simple_path, true));

    // ############### invalid crosswords ###############

    // invalid 6x7 crossword with complex intersections & mix of wildcards and letters
    stringstream contents_6_7_complex_invalid;
    contents_6_7_complex_invalid << WCD << BLK << BLK << WCD << BLK << 'p' 
                                 << 't' << WCD << WCD << 'e' << BLK << BLK 
                                 << WCD << BLK << BLK << WCD << WCD << WCD 
                                 << WCD << BLK << WCD << BLK << WCD << BLK 
                                 << BLK << BLK << 'a' << BLK << 'o' << BLK 
                                 << 'c' << 'o' << 't' << BLK << WCD << BLK 
                                 << BLK << 'n' << BLK << BLK << BLK << BLK;
    REQUIRE(dut->test_ac3_validity(6, 7, contents_6_7_complex_invalid.str(), dict_simple_path, false));
}

/**
 * AC-3 test for cw_csp for proper domain pruning
*/
/*
TEST_CASE("cw_csp ac3_pruning", "[cw_csp],[ac3],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp ac3_pruning");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";
    const string dict_nytimes_8_28_23 = "cw_csp/data/dict_nytimes_8_28_23.txt";
    const string dict_nytimes_10_17_13 = "cw_csp/data/dict_nytimes_10_17_13.txt";
    const string dict_nytimes_2_3_17 = "cw_csp/data/dict_nytimes_2_3_17.txt";
    
    // simple valid 2x2 crossword
    unordered_set<unique_ptr<cw_variable> > vars_2_2 = {
        make_unique<cw_variable>(0l, 0, 0, 2, VERTICAL,   unordered_set<word_t>{word_t("at"), word_t("to")}),
        make_unique<cw_variable>(1l, 1, 0, 2, HORIZONTAL, unordered_set<word_t>{word_t("to"), word_t("on")}),
    };
    stringstream contents_2_2;
    contents_2_2 << WCD << BLK 
                 << WCD << WCD;
    REQUIRE(dut->test_ac3(2, 2, contents_2_2.str(), dict_barebones_path, true, vars_2_2));

    // simple valid H-shaped 3x3 crossword
    unordered_set<unique_ptr<cw_variable> > vars_3_3_h = {
        make_unique<cw_variable>(0l, 0, 0, 3, VERTICAL,   unordered_set<word_t>{word_t("ace"), word_t("bed"), word_t("cab"), word_t("can"), word_t("cat"), word_t("dab"), word_t("dan"), word_t("gap")}),
        make_unique<cw_variable>(1l, 1, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("cat"), word_t("can"), word_t("eta"), word_t("ace"), word_t("are")}),
        make_unique<cw_variable>(2l, 0, 2, 3, VERTICAL,   unordered_set<word_t>{word_t("eta"), word_t("and"), word_t("cab"), word_t("can"), word_t("cat"), word_t("dab"), word_t("dan"), word_t("gap"), word_t("bed")}),
    };
    stringstream contents_3_3_h;
    contents_3_3_h << WCD << BLK << WCD 
                   << WCD << WCD << WCD 
                   << WCD << BLK << WCD;
    REQUIRE(dut->test_ac3(3, 3, contents_3_3_h.str(), dict_simple_path, true, vars_3_3_h));

    // simple valid H-shaped 4x4 crossword
    unordered_set<unique_ptr<cw_variable> > vars_4_4_h = {
        make_unique<cw_variable>(0l, 0, 0, 4, VERTICAL,   unordered_set<word_t>{word_t("atol"), word_t("atom"), word_t("caps"), word_t("cars"), word_t("halo"), word_t("knob"), word_t("know"), word_t("pant")}),
        make_unique<cw_variable>(1l, 1, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("tear"), word_t("tore"), word_t("tote"), word_t("trot"), word_t("atol"), word_t("near"), word_t("node")}),
        make_unique<cw_variable>(2l, 0, 3, 4, VERTICAL,   unordered_set<word_t>{word_t("trot"), word_t("troy"), word_t("been"), word_t("near"), word_t("pear"), word_t("tear"), word_t("atol"), word_t("atom"), word_t("plan")}),
    };
    stringstream contents_4_4_h;
    contents_4_4_h << WCD << BLK << BLK << WCD 
                   << WCD << WCD << WCD << WCD 
                   << WCD << BLK << BLK << WCD 
                   << WCD << BLK << BLK << WCD;
    REQUIRE(dut->test_ac3(4, 4, contents_4_4_h.str(), dict_simple_path, true, vars_4_4_h));

    // 5x5 nytimes crossword 8/28/2023
    unordered_set<unique_ptr<cw_variable> > vars_nytimes_8_28_23 = {
        make_unique<cw_variable>(0l, 2, 0, 3, VERTICAL,   unordered_set<word_t>{word_t("bmx")}),
        make_unique<cw_variable>(1l, 0, 1, 5, VERTICAL,   unordered_set<word_t>{word_t("broom")}),
        make_unique<cw_variable>(2l, 0, 2, 5, VERTICAL,   unordered_set<word_t>{word_t("leave")}),
        make_unique<cw_variable>(3l, 0, 3, 5, VERTICAL,   unordered_set<word_t>{word_t("eaten")}),
        make_unique<cw_variable>(4l, 0, 4, 3, VERTICAL,   unordered_set<word_t>{word_t("wds")}),
        make_unique<cw_variable>(5l, 0, 1, 4, HORIZONTAL, unordered_set<word_t>{word_t("blew")}),
        make_unique<cw_variable>(6l, 1, 1, 4, HORIZONTAL, unordered_set<word_t>{word_t("read")}),
        make_unique<cw_variable>(7l, 2, 0, 5, HORIZONTAL, unordered_set<word_t>{word_t("boats")}),
        make_unique<cw_variable>(8l, 3, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("move")}),
        make_unique<cw_variable>(9l, 4, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("xmen")}),
    };
    stringstream contents_nytimes_8_28_23;
    contents_nytimes_8_28_23 << BLK << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_ac3(5, 5, contents_nytimes_8_28_23.str(), dict_nytimes_8_28_23, true, vars_nytimes_8_28_23));

    // 5x5 nytimes crossword 10/17/13
    unordered_set<unique_ptr<cw_variable> > vars_nytimes_10_17_13 = {
        make_unique<cw_variable>(0l, 0, 0, 4, VERTICAL,   unordered_set<word_t>{word_t("doze")}),
        make_unique<cw_variable>(1l, 2, 1, 3, VERTICAL,   unordered_set<word_t>{word_t("eno")}),
        make_unique<cw_variable>(2l, 0, 2, 5, VERTICAL,   unordered_set<word_t>{word_t("cyber")}),
        make_unique<cw_variable>(3l, 0, 3, 3, VERTICAL,   unordered_set<word_t>{word_t("ear")}),
        make_unique<cw_variable>(4l, 1, 4, 4, VERTICAL,   unordered_set<word_t>{word_t("mayo")}),
        make_unique<cw_variable>(5l, 0, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("dice")}),
        make_unique<cw_variable>(6l, 1, 2, 3, HORIZONTAL, unordered_set<word_t>{word_t("yam")}),
        make_unique<cw_variable>(7l, 2, 0, 5, HORIZONTAL, unordered_set<word_t>{word_t("zebra")}),
        make_unique<cw_variable>(8l, 3, 0, 3, HORIZONTAL, unordered_set<word_t>{word_t("ene")}),
        make_unique<cw_variable>(9l, 4, 1, 4, HORIZONTAL, unordered_set<word_t>{word_t("oreo")}),
    };
    stringstream contents_nytimes_10_17_13;
    contents_nytimes_10_17_13 << WCD << 'i' << WCD << WCD << BLK 
                              << WCD << BLK << WCD << WCD << WCD
                              << WCD << WCD << WCD << WCD << WCD 
                              << WCD << WCD << WCD << BLK << WCD 
                              << BLK << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_ac3(5, 5, contents_nytimes_10_17_13.str(), dict_nytimes_10_17_13, true, vars_nytimes_10_17_13));

    // 5x5 nytimes crossword 2/3/17
    unordered_set<unique_ptr<cw_variable> > vars_nytimes_2_3_17 = {
        make_unique<cw_variable>(0l, 2, 0, 3, VERTICAL,   unordered_set<word_t>{word_t("may")}),
        make_unique<cw_variable>(1l, 1, 1, 4, VERTICAL,   unordered_set<word_t>{word_t("sale")}),
        make_unique<cw_variable>(2l, 0, 2, 5, VERTICAL,   unordered_set<word_t>{word_t("april")}),
        make_unique<cw_variable>(3l, 0, 3, 5, VERTICAL,   unordered_set<word_t>{word_t("decal")}),
        make_unique<cw_variable>(4l, 0, 4, 4, VERTICAL,   unordered_set<word_t>{word_t("ochs")}),
        make_unique<cw_variable>(5l, 0, 2, 3, HORIZONTAL, unordered_set<word_t>{word_t("ado")}),
        make_unique<cw_variable>(6l, 1, 1, 4, HORIZONTAL, unordered_set<word_t>{word_t("spec")}),
        make_unique<cw_variable>(7l, 2, 0, 5, HORIZONTAL, unordered_set<word_t>{word_t("march")}),
        make_unique<cw_variable>(8l, 3, 0, 5, HORIZONTAL, unordered_set<word_t>{word_t("alias")}),
        make_unique<cw_variable>(9l, 4, 0, 4, HORIZONTAL, unordered_set<word_t>{word_t("yell")}),
    };
    stringstream contents_nytimes_2_3_17;
    contents_nytimes_2_3_17 << BLK << BLK << 'a' << WCD << WCD 
                            << BLK << WCD << WCD << WCD << WCD
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_ac3(5, 5, contents_nytimes_2_3_17.str(), dict_nytimes_2_3_17, true, vars_nytimes_2_3_17));
}
*/

/**
 * simple backtracking solving test for cw_csp for valid/invalid checking
*/
TEST_CASE("cw_csp backtracking_valid_check", "[cw_csp],[backtracking],[quick]") {
    cw_csp_test_driver* dut = new cw_csp_test_driver("cw_csp backtracking_valid_check");
    const string dict_barebones_path = "cw_csp/data/dict_barebones.txt";
    const string dict_1000 = "cw_csp/data/words_top1000.txt";
    const string dict_1000_with_nytimes_5_16_23 = "cw_csp/data/words_top1000_with_nytimes_5_16_23.txt";
    const string dict_1000_with_nytimes_1_14_22 = "cw_csp/data/words_top1000_with_nytimes_1_14_22.txt";
    const string dict_1000_with_nytimes_1_13_22 = "cw_csp/data/words_top1000_with_nytimes_1_13_22.txt";
    const string dict_1000_with_nytimes_1_12_22 = "cw_csp/data/words_top1000_with_nytimes_1_12_22.txt";
    const string dict_1000_with_nytimes_1_11_22 = "cw_csp/data/words_top1000_with_nytimes_1_11_22.txt";
    const string dict_simple_path = "cw_csp/data/dict_simple.txt";
    const string dict_nytimes_8_28_23 = "cw_csp/data/dict_nytimes_8_28_23.txt";
    const string dict_nytimes_10_17_13 = "cw_csp/data/dict_nytimes_10_17_13.txt";
    const string dict_nytimes_2_3_17 = "cw_csp/data/dict_nytimes_2_3_17.txt";

    // ############### valid crosswords ###############

    // simple valid 2x2 crossword
    stringstream contents_2_2;
    contents_2_2 << WCD << BLK 
                 << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(2, 2, contents_2_2.str(), dict_barebones_path, true, true));

    // 5x5 nytimes crossword 8/28/2023
    stringstream contents_nytimes_8_28_23;
    contents_nytimes_8_28_23 << BLK << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_nytimes_8_28_23.str(), dict_nytimes_8_28_23, true, true));

    // 5x5 nytimes crossword 10/17/13
    stringstream contents_nytimes_10_17_13;
    contents_nytimes_10_17_13 << WCD << WCD << WCD << WCD << BLK 
                              << WCD << BLK << WCD << WCD << WCD
                              << WCD << WCD << WCD << WCD << WCD 
                              << WCD << WCD << WCD << BLK << WCD 
                              << BLK << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_nytimes_10_17_13.str(), dict_nytimes_10_17_13, true, true));

    // 5x5 nytimes crossword 2/3/17
    stringstream contents_nytimes_2_3_17;
    contents_nytimes_2_3_17 << BLK << BLK << WCD << WCD << WCD 
                            << BLK << WCD << WCD << WCD << WCD
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << WCD 
                            << WCD << WCD << WCD << WCD << BLK;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_nytimes_2_3_17.str(), dict_nytimes_2_3_17, true, true));

    // empty 3x3 crossword
    stringstream contents_3_3_empty;
    contents_3_3_empty << WCD << WCD << WCD 
                       << WCD << WCD << WCD 
                       << WCD << WCD << WCD ;
    REQUIRE(dut->test_backtracking_validity(3, 3, contents_3_3_empty.str(), dict_1000, true, true));

    // 3x3 donut crossword
    stringstream contents_3_3_donut;
    contents_3_3_donut << WCD << WCD << WCD 
                       << WCD << BLK << WCD 
                       << WCD << WCD << WCD ;
    REQUIRE(dut->test_backtracking_validity(3, 3, contents_3_3_donut.str(), dict_1000, true, true));

    // empty 4x3 crossword
    stringstream contents_4_3_empty;
    contents_4_3_empty << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD; 
    REQUIRE(dut->test_backtracking_validity(4, 3, contents_4_3_empty.str(), dict_1000, true, true));

    // 4x4 donut crossword
    stringstream contents_4_4_donut;
    contents_4_4_donut << WCD << WCD << WCD << WCD 
                       << WCD << BLK << BLK << WCD 
                       << WCD << BLK << BLK << WCD
                       << WCD << WCD << WCD << WCD; 
    REQUIRE(dut->test_backtracking_validity(4, 4, contents_4_4_donut.str(), dict_1000, true, true));

    // 4x4 diamond crossword
    stringstream contents_4_4_diamond;
    contents_4_4_diamond << BLK << WCD << WCD << WCD 
                         << WCD << WCD << WCD << WCD 
                         << WCD << WCD << WCD << WCD
                         << WCD << WCD << WCD << BLK; 
    REQUIRE(dut->test_backtracking_validity(4, 4, contents_4_4_diamond.str(), dict_1000, true, true));

    // 5x5 diamond crossword
    stringstream contents_5_5_diamond;
    contents_5_5_diamond << BLK << BLK << BLK << WCD << WCD 
                         << BLK << BLK << WCD << WCD << WCD 
                         << BLK << WCD << WCD << WCD << BLK 
                         << WCD << WCD << WCD << BLK << BLK 
                         << WCD << WCD << BLK << BLK << BLK;
    REQUIRE(dut->test_backtracking_validity(5, 5, contents_5_5_diamond.str(), dict_1000, true, true));

    // 15x15 nytimes crossword 5/16/23
    stringstream contents_nytimes_5_16_23;
    contents_nytimes_5_16_23 << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << BLK << BLK << BLK << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << BLK << BLK << BLK
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(15, 15, contents_nytimes_5_16_23.str(), dict_1000_with_nytimes_5_16_23, true, true));

    // 15x15 nytimes crossword 1/14/22
    stringstream contents_nytimes_1_14_22;
    contents_nytimes_1_14_22 << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << BLK << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << BLK 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK;
    REQUIRE(dut->test_backtracking_validity(15, 15, contents_nytimes_1_14_22.str(), dict_1000_with_nytimes_1_14_22, true, true));

    // 15x15 nytimes crossword 1/13/22
    stringstream contents_nytimes_1_13_22;
    contents_nytimes_1_13_22 << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << BLK << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK 
                             << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD 
                             << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << BLK 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(15, 15, contents_nytimes_1_13_22.str(), dict_1000_with_nytimes_1_13_22, true, true));

    // 15x15 nytimes crossword 1/12/22
    stringstream contents_nytimes_1_12_22;
    contents_nytimes_1_12_22 << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << BLK << BLK << BLK << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD << WCD << BLK << BLK << WCD << WCD << WCD << WCD << BLK << BLK << BLK 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(15, 15, contents_nytimes_1_12_22.str(), dict_1000_with_nytimes_1_12_22, true, true));

    // 15x15 nytimes crossword 1/11/22
    stringstream contents_nytimes_1_11_22;
    contents_nytimes_1_11_22 << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD 
                             << BLK << BLK << BLK << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << BLK << BLK << BLK 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << WCD << BLK << BLK << BLK << WCD << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD 
                             << BLK << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << BLK 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD 
                             << WCD << WCD << WCD << WCD << WCD << BLK << WCD << WCD << WCD << BLK << WCD << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(15, 15, contents_nytimes_1_11_22.str(), dict_1000_with_nytimes_1_11_22, true, true));

    // ############### invalid crosswords ###############

    // initially invalid 4x3 crossword
    stringstream contents_4_3_invalid;
    contents_4_3_invalid << 'x' << 'y' << WCD << 'z'
                         << WCD << WCD << WCD << WCD 
                         << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(4, 3, contents_4_3_invalid.str(), dict_1000, false, true));

    // empty 2x2 crossword, w/ barebones dict
    stringstream contents_2_2_empty;
    contents_2_2_empty << WCD << WCD 
                       << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(2, 2, contents_2_2_empty.str(), dict_barebones_path, false, true));

    // 3x3 donut crossword, w/ barebones dict
    REQUIRE(dut->test_backtracking_validity(3, 3, contents_3_3_donut.str(), dict_barebones_path, false, true));

    // empty 4x4 crossword
    stringstream contents_4_4_empty;
    contents_4_4_empty << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD 
                       << WCD << WCD << WCD << WCD;
    REQUIRE(dut->test_backtracking_validity(4, 4, contents_4_4_empty.str(), dict_1000, false, true));
}