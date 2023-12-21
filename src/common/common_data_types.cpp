// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   8/13/2023
// Description: operator implementations for common data types
// ==================================================================

#include "common_data_types.h"

using namespace common_data_types_ns;

/**
 * @brief equality operator for crossword word options
*/
bool word_t::operator==(const word_t& rhs) const {
    return word == rhs.word;
}

/**
 * @brief hash function for word_t
*/
size_t hash<word_t>::operator()(const word_t& w) const {
    return hash<string>{}(w.word);
}

/**
 * @brief progress bar constructor, starts printing a new bar upon initialization
 * 
 * @param os ostream to print to
 * @param line_width total width occupied by progress bar
 * @param msg message 
 * @param symbol_full char to represent a tick on the progress bar
 * @param symbol_empty char to represent a blank on the progress bar
*/
progress_bar::progress_bar(ostream& os, size_t line_width, string msg, const char symbol_full, const char symbol_empty) : os{os} {
    bar_width = line_width - sizeof("] 100%");
    full_bar = string(bar_width, symbol_full) + string(bar_width, symbol_empty);

    assert(msg.size() + 2 < bar_width && msg.find('\n') == msg.npos);
    this->msg = msg + " [";

    write(0.0);
}

/**
 * @brief displays fractional progress on this progress bar
 * 
 * @param fraction progress, [0, 1.0]
*/
void progress_bar::write(double fraction) {
    if(fraction < 0) fraction = 0;
    else if(fraction > 1) fraction = 1;

    size_t width = bar_width - msg.size();
    size_t offset = bar_width - static_cast<unsigned>(width * fraction);

    os << '\r' << msg;
    os.write(full_bar.data() + offset, (long)width);
    os << "] " << std::setw(3) << static_cast<int>(100 * fraction) << "% " << std::flush;
}

/**
 * @brief destructor for progress bar, finishes writing last line
*/
progress_bar::~progress_bar() {
    write('\n');
    os << '\n';
}