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
cw_gen::cw_gen(string name) : common_parent(name, VERBOSITY) {
    // do nothing
}

/**
 * @brief shorthand function to squash param options into a readable str
 * 
 * @param options vector of options to squash
 * @returns str representation of options
*/
string cw_gen::squash_options(const vector<string>& options) {
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
// void cw_gen::build() {
//     crossword cw = crossword("puzzle cw", length, height, contents.value_or(string(num_tiles, WILDCARD)));
//     tree = make_unique<cw_tree>("cw_tree", std::move(cw), dict_path.at(dict), display_progress_bar, profile_header.has_value());
// }

/**
 * @brief attempt to solve with given parameters
 *
 * @return up to 1 solution per requested solution, if these do not match, search space was exhausted
 */
vector<string> cw_gen::solve() {
    crossword cw = crossword("puzzle cw", length, height, contents.value_or(string(num_tiles(), WILDCARD)));
    tree = make_unique<cw_tree>("cw_tree", std::move(cw), dict_path.at(dict), display_progress_bar, profile_header);

    return tree->solve(num_solutions);
}

/**
 * @brief main function using command line interface w/ cxxopts to generate crosswords
*/
int main(int argc, char** argv) {
    cw_gen cwgen = cw_gen("cwgen");
    cxxopts::Options options("cw_gen", "Crossword puzzle generation tool given word list and blank grid to fill in");

    // key: param name, value: set of allowed param values
    unordered_map<string, vector<string> > param_vals = {
        {"dict", {"small", "medium", "large", "xlarge", "giant", "all"}},
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
        ("d,dict",      "Word dictionary: " + cw_gen::squash_options(param_vals["dict"]),        cxxopts::value<string>()->default_value("xlarge"))
        ("e,example",   examples_desc.str(),                                                     cxxopts::value<string>())
        ("n,num",       "Number of grid permutations to find puzzles on",                        cxxopts::value<size_t>()->default_value("1"))
        ("v,verbosity", "Debug verbosity: " + cw_gen::squash_options(param_vals["verbosity"]),   cxxopts::value<string>()->default_value("fatal"))
        ("p,progress",  "Enable progress bar (default: false)",                                  cxxopts::value<bool>())
        ("P,profile",   "Name of JSON profile file to generate if provided",                     cxxopts::value<string>())
        ("h,help",      "Print usage")
        ;

    auto result = options.parse(argc, argv);

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
            cout << "Error: got invalid example option " << example << ", allowed: " << cw_gen::squash_options(param_vals["example"]) << endl;
            exit(1);
        }

        cwgen.set_dimensions(examples_map[example].length, examples_map[example].height);
        cwgen.set_dict(examples_map[example].dict);
        cwgen.set_contents(examples_map[example].contents);
    } else {
        // non-example, use user-specified size/dict/contents

        // ############### size ###############

        vector<uint> dimensions = result["size"].as<vector<uint>>();
        if(dimensions.size() != 2) {
            cout << "Error: got unknown number of size dimensions (" << dimensions.size() << "), should be 2" << endl;
            exit(1);
        }
        if(dimensions[0] <= 0 || dimensions[1] <= 0) {
            cout << "Error: crossword dimensions must be positive and non-zero" << endl;
            exit(1);
        } else if(dimensions[0] == 1 && dimensions[1] == 1) {
            cout << "Error: 1x1 crossword contains no valid words, word min len = " << MIN_WORD_LEN << endl;
            exit(1);
        }
        cwgen.set_dimensions(dimensions[0], dimensions[1]);

        // ############### dictionary ###############

        string dict = result["dict"].as<string>();
        if(std::find(param_vals["dict"].begin(), param_vals["dict"].end(), dict) == param_vals["dict"].end()) {
            cout << "Error: got invalid dictionary option " << dict << ", allowed: " << cw_gen::squash_options(param_vals["dict"]) << endl;
            exit(1);
        }
        cwgen.set_dict(dict);

        // ############### contents ###############

        if(result.count("contents")) {
            string contents = result["contents"].as<string>();
            if(contents.size() != cwgen.num_tiles()) {
                cout << "Error: got illegal contents length (" << contents.size() << ") given dimensions, should be: " << cwgen.num_tiles() << endl;
                exit(1);
            }
            for(char c : contents) {
                if(c != BLACK && c != WILDCARD && (c < 'a' || c > 'z')) {
                    cout << "Error: got illegal char in contents: " << c << ", all chars should be a lowercase letter, a wildcard (\"" << WILDCARD << "\"), or a black tile (\"" << BLACK << "\")" << endl;
                    exit(1);
                }
            }
            cwgen.set_contents(contents);
        }
    }

    // ############### solutions ###############

    size_t num_solutions = result["num"].as<size_t>();
    if(num_solutions == 0) {
        cout << "Error: got illegal number of grid to search: " << num_solutions << ", must be nonzero" << endl;
        exit(1);
    }
    cwgen.set_num_solutions(num_solutions);

    // ############### verbosity ###############

    string verbosity = result["verbosity"].as<string>();
    if(std::find(param_vals["verbosity"].begin(), param_vals["verbosity"].end(), verbosity) == param_vals["verbosity"].end()) {
        cout << "Error: got invalid verbosity option " << verbosity << ", allowed " << cw_gen::squash_options(param_vals["verbosity"]) << endl;
        exit(1);
    }
    VERBOSITY = verbosity_name_to_type.at(verbosity);

    // ############### progress bar ###############

    if(result.count("progress")) {
        cwgen.enable_progress_bar();
    }

    // ############### cw_timetracker ###############

    if(result.count("profile")) {
        if(result["profile"].as<string>() == "") {
            cout << "Error: profiling output filepath must be nonempty, got: " << endl;
            exit(1);
        }

        cwgen.enable_profile(result["profile"].as<string>());
    }

    // ############### solving ###############

    vector<string> solutions = cwgen.solve();
    for(const string& s : solutions) {
        cout << "Found: " << s << endl;
    }
    if(solutions.size() != result["num"].as<size_t>()) {
        cout << "Error: insufficient number of crosswords generated for the given parameters. " 
             << "Try an example, or use different dimensions, grid contents, or dictionary" << endl;
    }

    // cwgen->build();
    // if(cwgen->solve()) {
    //     cout << "Found: " << cwgen->result() << endl;
    // } else {
    //     cout << "Error: no valid crossword generated for the given parameters. " 
    //          << "Try an example, or use different dimensions, grid contents, or dictionary" << endl;
    //     exit(1);
    // }

    return 0;
}