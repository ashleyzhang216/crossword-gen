// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module implementation
// ==================================================================

#include "cw_utils.h"

#ifndef VERBOSITY_NUM
verbosity_t VERBOSITY = INFO;
#else
verbosity_t VERBOSITY = static_cast<verbosity_t>(VERBOSITY_NUM);
#endif

unordered_map<verbosity_t, string> verbosity_type_to_name = {
    {FATAL, "FATAL"},
    {ERROR, "ERROR"},
    {WARNING, "WARNING"},
    {INFO, "INFO"},
    {DEBUG, "DEBUG"},
};

// ############### cw_utils ###############

/**
 * @brief construct new utils object
 * 
 * @param name name of this util object
 * @param max_verbosity min verbosity of messages to be printed
*/
cw_utils::cw_utils(const string_view& name, const verbosity_t& max_verbosity) : name(name), max_verbosity(max_verbosity) {
    // do nothing, initializer list is sufficient
}

/**
 * @brief adds new fixed bar
 * @param line_width total width occupied by progress bar
 * @param msg message 
 * @param symbol_full char to represent a tick on the progress bar
 * @param symbol_empty char to represent a blank on the progress bar
 * @pre no prior fixed bar still exists
 * 
 * TODO: add support for multiple simultaneous progress bars, if needed
*/
void cw_utils::add_fixed_bar(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty) {
    if(!bar_mx.try_lock()) {
        log(FATAL, "Support for multiple bars requested and is unimplemented");
    }
    assert(!bar.has_value());
    bar.emplace(line_width, msg, symbol_full, symbol_empty);
    request_write_bar(0.0);
}

/**
 * @brief print fixed bar with a ratio filled to std::cout
 * @param fraction ratio of bar to be filled, [0.0, 1.0]
*/
void cw_utils::request_write_bar(double fraction) const {
    lock_guard print_lg(print_mx);
    write_bar(fraction);
}

/**
 * @brief with print_mx locked, print fixed bar with a ratio filled to std::cout
 * @param fraction ratio of bar to be filled, [0.0, 1.0]
 * @pre print_mx has been locked for this operation
*/
void cw_utils::write_bar(double fraction) const {
    assert(bar.has_value());
    if(fraction < 0) fraction = 0;
    else if(fraction > 1) fraction = 1;

    bar.value().filled_ratio = fraction;
    size_t width = bar.value().bar_width - bar.value().msg.size();
    size_t offset = bar.value().bar_width - static_cast<unsigned>(width * fraction);

    cout << '\r' << bar.value().msg;
    cout.write(bar.value().full_bar.data() + std::move(offset), static_cast<long>(std::move(width)));
    cout << "] " << std::setw(3) << static_cast<int>(100 * fraction) << "% " << std::flush;
}

/**
 * @brief closes out the current fixed bar, allows a new one to be created
*/
void cw_utils::end_bar() {
    lock_guard print_lg(print_mx);
    assert(bar.has_value());
    assert(!bar_mx.try_lock());

    write_bar(1.0);
    cout << endl;

    bar.reset();
    bar_mx.unlock();
}

/**
 * @brief constructor for utils' fixed bar settings
 * @param line_width total char width of fixed bar
 * @param msg label to display in front of fixed bar
 * @param symbol_full char to represent filled portion of fixed bar
 * @param symbol_empty char to represent empty portion of fixed bar
*/
cw_utils::bar_settings::bar_settings(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty) 
        : bar_width(line_width - sizeof("] 100%")), 
          msg(string(msg) + " ["), 
          full_bar(string(bar_width, symbol_full) + string(bar_width, symbol_empty)), 
          bar_flusher('\r' + string(line_width, ' ') + '\r') {
    assert(this->msg.size() < bar_width && msg.find('\n') == msg.npos);
}

/**
 * @brief definition of cw_util global mutexes/bar settings to protect print and bar creation access 
*/
mutex cw_utils::print_mx;
mutex cw_utils::bar_mx;
optional<cw_utils::bar_settings> cw_utils::bar;

// ############### progress_bar ###############

/**
 * @brief adds one to numerator, and updates progress bar 
*/
void progress_bar::incr_numerator() {
    numerator++;

    // update progress bar if change in completion surpasses granularity
    if(static_cast<double>(numerator)/denominator >= progress_ratio + granularity) {
        progress_ratio += granularity;
        utils.request_write_bar(progress_ratio);
    }
}

/**
 * @brief destructor that finishes writing last line
*/
progress_bar::~progress_bar() {
    utils.end_bar();
}

// ############### set_contents_equal() ###############

template bool set_contents_equal<string>(const unordered_set<string>& lhs, const unordered_set<string>& rhs, bool debug_prints);
template bool set_contents_equal<word_t>(const unordered_set<word_t>& lhs, const unordered_set<word_t>& rhs, bool debug_prints);
