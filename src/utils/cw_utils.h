// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   7/4/2023
// Description: universal util module declaration
// ==================================================================

#ifndef CW_UTILS_H
#define CW_UTILS_H

#include "../common/common_data_types.h"

// verbosity levels
typedef enum {
  FATAL = 0,
  ERROR = 1,
  WARNING = 2, 
  INFO = 3,
  DEBUG = 4
} verbosity_t;

// global verbosity
extern verbosity_t VERBOSITY;

struct assertion_failure_exception : public exception {
  const char *what() const throw() {
    return "crossword-gen failed assertion";
  }
};

#undef assert
#define assert(x)                                                                                   \
  if(!(x)) {                                                                                        \
    cout << "\nAssertion failed\n" << "File: " << __FILE__ << ": " << std::dec << __LINE__ << endl; \
    throw assertion_failure_exception();                                                            \
  }                                                                                                 \

#undef assert_m
#define assert_m(x, msg)                                                                            \
  if(!(x)) {                                                                                        \
    cout << "\nAssertion failed\n" << "File: " << __FILE__ << ": " << std::dec << __LINE__ << endl; \
          << msg << endl;                                                                            \
    throw assertion_failure_exception();                                                            \
  } 

class cw_utils {
  public:
    string name;
    verbosity_t min_verbosity;

    // base constructor
    cw_utils(string name, verbosity_t verbosity);

    // unused destructor
    ~cw_utils(){};

    // general message print
    bool print_msg(string s, verbosity_t verbosity = INFO);
    bool print_msg(stringstream *s, verbosity_t verbosity = INFO);
};


#endif // CW_UTILS_H