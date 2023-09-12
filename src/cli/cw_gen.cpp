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
    if(has_grid_contents) {
        csp = make_unique<cw_csp>("puzzle", length, height, contents, dict_path[dict]);
    } else {
        csp = make_unique<cw_csp>("puzzle", length, height, dict_path[dict]);
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
        {"dict", {"top1000", "top3000", "top10000", "crossfire", "all"}},
        {"example", {"empty", "cross", "bridge", "stairs", "donut", "crosshair1", "crosshair2"}},
        {"verbosity", {"fatal", "error", "warning", "info", "debug"}}
    };

    // because these are very long
    stringstream contents_desc, examples_desc;
    contents_desc << "Puzzle grid contents from top left to bottom right by row, all lowercase, in quotes. wildcard is: \"" 
                  << WILDCARD << "\", black is: \"" << BLACK << "\"";
    examples_desc << "Example crossword puzzles (overrides size/contents/dict): " << cw_gen::squash_options(param_vals["example"]);

    options.add_options()
        ("s,size",      "Puzzle size, in format length,height",                                  cxxopts::value<vector<uint>>()->default_value("4,4"))
        ("c,contents",  contents_desc.str(),                                                     cxxopts::value<string>())
        ("d,dict",      "Word dictionary: " + cw_gen::squash_options(param_vals["dict"]),        cxxopts::value<string>()->default_value("top10000"))
        ("e,example",   examples_desc.str(),                                                     cxxopts::value<string>())
        ("v,verbosity", "Debug verbosity: " + cw_gen::squash_options(param_vals["verbosity"]),   cxxopts::value<string>()->default_value("fatal"))
        ("h,help",      "Print usage")
        ;

    auto result = options.parse(argc, argv);

    // ############### no args specified ###############

    if(!result.count("help") && !result.count("size") && !result.count("contents") && !result.count("dict") && !result.count("example") && !result.count("verbosity")) {
        cout << "Error: no args specified. Try running with arg \"-h\" for help or with arg \"-e [";
        for(uint i = 0; i < param_vals["example"].size(); i++) {
            cout << param_vals["example"][i];
            if(i != param_vals["example"].size() - 1) cout << "|";
        }
        cout << "]\" to generate an example crossword" << endl;
        exit(1);
    }

    // ############### help ###############

    // print help msg if specified
    if(result.count("help")) {
        cout << options.help() << endl;
        exit(0);
    }

    // ############### example ###############

    if(result.count("example")) {
        string example = result["example"].as<string>();
        if(std::find(param_vals["example"].begin(), param_vals["example"].end(), example) == param_vals["example"].end()) {
            cout << "got invalid example option " << example << ", allowed: " << cw_gen::squash_options(param_vals["example"]) << endl;
            exit(1);
        }

        cwgen->set_dimensions(examples_map[example].length, examples_map[example].height);
        cwgen->set_dict(examples_map[example].dict);
        cwgen->set_contents(examples_map[example].contents);
    } else {
        // non-example, use user-specified size/dict/contents

        // ############### size ###############

        vector<uint> dimensions = result["size"].as<vector<uint>>();
        if(dimensions.size() != 2) {
            cout << "got unknown number of size dimensions (" << dimensions.size() << "), should be 2" << endl;
            exit(1);
        }
        cwgen->set_dimensions(dimensions[0], dimensions[1]);

        // ############### dictionary ###############

        string dict = result["dict"].as<string>();
        if(std::find(param_vals["dict"].begin(), param_vals["dict"].end(), dict) == param_vals["dict"].end()) {
            cout << "got invalid dictionary option " << dict << ", allowed: " << cw_gen::squash_options(param_vals["dict"]) << endl;
            exit(1);
        }
        cwgen->set_dict(dict);

        // ############### contents ###############

        if(result.count("contents")) {
            string contents = result["contents"].as<string>();
            if(contents.size() != cwgen->num_tiles()) {
                cout << "got illegal contents length (" << contents.size() << ") given dimensions, should be: " << cwgen->num_tiles() << endl;
                exit(1);
            }
            for(char c : contents) {
                if(c != BLACK && c != WILDCARD && (c < 'a' || c > 'z')) {
                    cout << "got illegal char in contents: " << c << ", all chars should be a lowercase letter, a wildcard (\"" << WILDCARD << "\"), or a black tile (\"" << BLACK << "\")" << endl;
                    exit(1);
                }
            }
            cwgen->set_contents(contents);
        }
    }

    // ############### verbosity ###############

    string verbosity = result["verbosity"].as<string>();
    if(std::find(param_vals["verbosity"].begin(), param_vals["verbosity"].end(), verbosity) == param_vals["verbosity"].end()) {
        cout << "got invalid verbosity option " << verbosity << ", allowed " << cw_gen::squash_options(param_vals["verbosity"]) << endl;
        exit(1);
    }
    VERBOSITY = verbosity_map[verbosity];

    // ############### cw_csp solving ###############

    cwgen->build();
    if(cwgen->solve()) {
        cout << "found crossword: " << cwgen->result() << endl;
        exit(0);
    } else {
        cout << "no valid crossword generated for the given parameters; " 
             << "try an example, or use different dimensions, grid contents, or dictionary" << endl;
        exit(1);
    }

    // when program is allowed to reach here, it segfaults
    // TODO: investigate why it segfaults
    return 0;
}