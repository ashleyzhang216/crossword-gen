// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   9/6/2023
// Description: class implementation & main() for main crossword generation tool
// ==================================================================

#include "cw_gen.h"

using namespace cw_gen_ns;

/**
 * @brief constructor for a crossword generation tool with all default values
*/
cw_gen::cw_gen(string name) : common_parent(name) {
    // do nothing
}

/**
 * @brief main function using cxxopts to generate crosswords
*/
int main(int argc, char** argv) {
    cout << "hello, world!" << endl;

    // cxxopts example, TODO: implement real CLI
    cxxopts::Options options("test", "A brief description");

    options.add_options()
        ("b,bar", "Param bar", cxxopts::value<std::string>())
        ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
        ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
        ("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    bool debug = result["debug"].as<bool>();
    std::string bar;
    if (result.count("bar"))
        bar = result["bar"].as<std::string>();
    int foo = result["foo"].as<int>();
    
    // debug
    cout << "debug: " << debug << endl;
    cout << "foo: " << foo << endl;

    return 0;
}