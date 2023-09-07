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
 * @brief shorthand function to squash param options into a readable str
 * 
 * @param options vector of options to squash
 * @returns str representation of options
*/
string cw_gen::squash_options(vector<string> options) {
    stringstream opts_ss;

    for(uint i = 0; i < options.size(); i++) {
        opts_ss << "\"" << options[i] << "\"";
        if(i != options.size() - 1) opts_ss << ", ";
        if(i == options.size() - 2) opts_ss << "or ";
    }

    return opts_ss.str();
}

/**
 * @brief build cw_csp after all params set
*/
void cw_gen::build() {
    switch(grid_type) {
        case GRID_NONE:
            csp = make_unique<cw_csp>("puzzle", length, height, dict_path[dict]);
            break;
        case GRID_CONTENTS:
            csp = make_unique<cw_csp>("puzzle", length, height, contents, dict_path[dict]);
            break;
        case GRID_PATTERN:
            // TODO: implement
            cout << "pattern grid population unimplemented, exiting" << endl;
            exit(1);
            break;
        default:
            cout << "build() got unknown grid_type: " << grid_type << endl;
            exit(1);
            break;
    }
}

/**
 * @brief main function using command line interface w/ cxxopts to generate crosswords
*/
int main(int argc, char** argv) {
    
    unique_ptr<cw_gen> cwgen = make_unique<cw_gen>("cwgen");
    cxxopts::Options options("cw_gen", "Crossword puzzle generation tool given word list and blank grid to fill in");

    // key: param name, value: set of allowed param values
    unordered_map<string, vector<string> > param_vals = {
        {"dict", {"top1000", "top3000", "top10000", "all"}},
        {"pattern", {"empty"}},
        {"verbosity", {"fatal", "error", "warning", "info", "debug"}}
    };

    // because this one is very long
    stringstream contents_desc;
    contents_desc << "Puzzle grid contents (cannot be used if pattern also specified)" 
                  << "from top left to bottom right by row, all lowercase, in quotes. wildcard is: \"" 
                  << WILDCARD << "\", black is: \"" << BLACK << "\"";

    options.add_options()
        ("s,size",      "Puzzle size, in format length,height",                                  cxxopts::value<vector<uint>>()->default_value("5,5"))
        ("c,contents",  contents_desc.str(),                                                     cxxopts::value<string>())
        ("d,dict",      "Word dictionary: " + cw_gen::squash_options(param_vals["dict"]),        cxxopts::value<string>()->default_value("top3000"))
        ("p,pattern",   "Puzzle grid pattern: " + cw_gen::squash_options(param_vals["pattern"]), cxxopts::value<string>())
        ("v,verbosity", "Debug verbosity: " + cw_gen::squash_options(param_vals["verbosity"]),   cxxopts::value<string>()->default_value("fatal"))
        ("h,help",      "Print usage")
        ;

    auto result = options.parse(argc, argv);

    // ############### help ###############

    // print help msg if specified or if no args provided
    if(result.count("help") || 
        (!result.count("size") && !result.count("contents") && !result.count("dict") && !result.count("pattern") && !result.count("verbosity"))
    ) {
        cout << options.help() << endl;
        exit(0);
    }

    // ############### size ###############

    vector<uint> dimensions = result["size"].as<vector<uint>>();
    if(dimensions.size() != 2) {
        cout << "got unknown number of size dimensions (" << dimensions.size() << "), should be 2" << endl;
        exit(1);
    }
    // cout << "set dimension size: " << dimensions[0] << ", " << dimensions[1] << endl; // debug
    cwgen->set_dimensions(dimensions[0], dimensions[1]);

    // ############### dictionary ###############

    string dict = result["dict"].as<string>();
    if(std::find(param_vals["dict"].begin(), param_vals["dict"].end(), dict) == param_vals["dict"].end()) {
        cout << "got invalid dictionary option " << dict << ", allowed: " << cw_gen::squash_options(param_vals["dict"]);
        exit(1);
    }
    // cout << "set dictionary: " << dict << endl; // debug
    cwgen->set_dict(dict);

    // ############### contents & pattern exclusitivity ###############

    // contents & pattern cannot both be set
    if(result.count("contents") && result.count("pattern")) {
        cout << "got illegal specification of both contents and pattern params" << endl;
        exit(1);
    }

    // ############### contents ###############

    if(result.count("contents")) {
        string contents = result["contents"].as<string>();
        if(contents.size() != cwgen->num_tiles()) {
            cout << "got illegal contents length (" << contents.size() << ") given dimensions, should be: " << cwgen->num_tiles() << endl;
            exit(1);
        }
        for(char c : contents) {
            if(c != BLACK && c != WILDCARD && (c < 'a' || c > 'z')) {

            }
        }
        // cout << "set contents: " << contents << endl; // debug
        cwgen->set_contents(contents);
    }

    // ############### pattern ###############

    if(result.count("pattern")) {
        string pattern = result["pattern"].as<string>();
        if(std::find(param_vals["pattern"].begin(), param_vals["pattern"].end(), pattern) == param_vals["pattern"].end()) {
            cout << "got invalid pattern option " << pattern << ", allowed " << cw_gen::squash_options(param_vals["pattern"]);
            exit(1);
        }
        // cout << "set pattern: " << pattern << endl; // debug
        cwgen->set_pattern(pattern);
    }

    // ############### verbosity ###############

    string verbosity = result["verbosity"].as<string>();
    if(std::find(param_vals["verbosity"].begin(), param_vals["verbosity"].end(), verbosity) == param_vals["verbosity"].end()) {
        cout << "got invalid verbosity option " << verbosity << ", allowed " << cw_gen::squash_options(param_vals["verbosity"]);
        exit(1);
    }
    // cout << "set verbosity: " << verbosity << endl; // debug
    VERBOSITY = verbosity_map[verbosity];

    // ############### cw_csp solving ###############

    cwgen->build();
    if(cwgen->solve()) {
        cout << "found crossword: " << cwgen->result() << endl;
        exit(0);
    } else {
        cout << "no valid crossword generated for the given parameters; " 
             << "try different dimensions, grid contents/pattern, or dictionary" << endl;
        exit(1);
    }

    // when program is allowed to reach here, it segfaults
    // TODO: investigate why it segfaults
    return 0;
}