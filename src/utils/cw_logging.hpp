// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   2/15/2025
// Description: cw logging and progress bars
// ==================================================================

#ifndef CW_LOGGING_HPP
#define CW_LOGGING_HPP

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

// ############### verbosity ###############

namespace cw {
    // verbosity levels
    enum verbosity_t {
        FATAL   = 0,
        ERROR   = 1,
        WARNING = 2, 
        INFO    = 3,
        DEBUG   = 4
    };

    // global verbosity
    #ifndef VERBOSITY_NUM
    inline verbosity_t VERBOSITY = INFO;
    #else
    inline verbosity_t VERBOSITY = static_cast<verbosity_t>(VERBOSITY_NUM);
    #endif

    // mapping from verbosity value to name
    inline unordered_map<verbosity_t, string> verbosity_type_to_name = {
        {FATAL,   "FATAL"},
        {ERROR,   "ERROR"},
        {WARNING, "WARNING"},
        {INFO,    "INFO"},
        {DEBUG,   "DEBUG"},
    };
} // namespace cw

// ############### cw_utils declaration ###############

namespace cw {
    /**
     * @brief object to handle message logging and progress bars
    */
    class cw_utils {
        public:
            // base constructor
            cw_utils(const string_view& name, const verbosity_t& max_verbosity);

            // register new bar always displayed on std::cout as the last line below log print statements
            void add_fixed_bar(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty);

            // update fixed bar on to std::cout, 0 <= fraction <= 1.0
            void request_write_bar(double fraction) const;

            // completes to std::cout fixed bar to 100%
            void end_bar();

            /**
             * @brief log messages to std::cout or std::cerr if verbosity is high enough
             *
             * @param verbosity the verbosity of this message, which is logged if <= max verbosity
             * @param args printable objects to log
            */
            template <typename... Types>
            void log(const verbosity_t& verbosity, const Types&... args) const {
                if(verbosity <= max_verbosity) [[unlikely]] {
                    lock_guard print_lg(print_mx);

                    // erase progress bar, if it exists
                    if(bar.has_value()) cout << bar.value().bar_flusher;

                    if(verbosity <= ERROR) {
                        log_to_ostream(cerr, verbosity_type_to_name.at(verbosity), ": ", name, ' ', args...);
                        throw cw::assertion_failure_exception("Log with verbosity: " + verbosity_type_to_name.at(verbosity));
                    } else {
                        log_to_ostream(cout, verbosity_type_to_name.at(verbosity), ": ", name, ' ', args...);
                    }

                    // restore progress bar, if it exists
                    if(bar.has_value()) write_bar(bar.value().filled_ratio);
                }
            }

        protected:
            // fields for printing
            string name;
            verbosity_t max_verbosity;

            // declaration of global mutex to protect printing
            static mutex print_mx;

            // settings for fixed bar
            struct bar_settings {
                // print settings
                const size_t bar_width;
                const string msg;

                // util helper objects
                const string full_bar;
                const string bar_flusher;

                // current fraction of bar filled, [0.0, 1.0]
                double filled_ratio;

                // base constructor
                bar_settings(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty);
            };
            
            // declaration of global mutex to protect bar creation access
            static mutex bar_mx;

            // settings for fixed bar, if it exists
            static optional<bar_settings> bar;

            // with print_max locked, update fixed bar, 0 <= fraction <= 1.0
            void write_bar(double fraction) const;

            /**
             * @brief preferred types for printing
             * @note the types are restricted so that with -O3 optimizations enabled, the arguments
             *       don't have to be actually loaded from memory if the verbosity check fails. with
             *       this optimization, the assembly for each call to log() is a compare instruction
             *       followed by a conditional jump. since these arguments have no external effect,
             *       they can safely be only loaded after the jump if it occurs, meaning that in
             *       prod, a call to log() only costs 2 instructions of no op
            */
            template <typename T>
            struct no_mov_on_verbosity_fail :
                std::disjunction<
                    std::is_convertible<T, string_view>,
                    std::is_arithmetic<T>
                > {};

            // allowed types for printing, is a superset of preferred print types
            template <typename T>
            static constexpr bool is_printable =
                std::disjunction_v<
                    no_mov_on_verbosity_fail<T>,
                    std::is_class<T>,
                    std::is_enum<T>,
                    std::is_pointer<T>
                >;
            static_assert(is_printable<const char*>);
            static_assert(is_printable<string>);

            /**
             * @brief log messages to ostream, after verbosity check has passed
             *
             * @param os the stream to print to
             * @param msg the next printable object to logged
             * @param args the other arguments to be logged
            */
            template <class T, typename... Types>
            requires is_printable<T>
            void log_to_ostream(ostream& os, const T& msg, const Types&... args) const {
                os << msg;
                if constexpr(sizeof...(args) > 0) {
                    log_to_ostream(os, args...);
                } else {
                    os << endl << std::flush;
                }
            }

    }; // class cw_utils

    /**
     * @brief object to manage a progress bar lifetime and printing with cw::cw_utils
     * @cite https://codereview.stackexchange.com/a/186537
    */
    class progress_bar {
        public:
            // base constructor
            template <typename... Types>
            progress_bar(cw::cw_utils& utils, uint denominator, double granularity, const Types&... args)
                    : utils(utils),
                    numerator(0),
                    progress_ratio(0.0),
                    denominator(denominator),
                    granularity(granularity) {
                utils.add_fixed_bar(args...);
            }

            // add one to numerator of progress
            void incr_numerator();

            // destructor, terminates bar and prints newline
            ~progress_bar();

        private:
            cw::cw_utils& utils;
            uint numerator;
            double progress_ratio; // [0.0, 1.0]
            const uint denominator;
            const double granularity;

    }; // class progress_bar
} // namespace cw

// ############### cw_utils implementation ###############

namespace cw {
    /**
     * @brief construct new utils object
     *
     * @param name name of this util object
     * @param max_verbosity min verbosity of messages to be printed
    */
    inline cw_utils::cw_utils(const string_view& name, const verbosity_t& max_verbosity) : name(name), max_verbosity(max_verbosity) {
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
    inline void cw_utils::add_fixed_bar(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty) {
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
    inline void cw_utils::request_write_bar(double fraction) const {
        lock_guard print_lg(print_mx);
        write_bar(fraction);
    }

    /**
     * @brief with print_mx locked, print fixed bar with a ratio filled to std::cout
     * @param fraction ratio of bar to be filled, [0.0, 1.0]
     * @pre print_mx has been locked for this operation
    */
    inline void cw_utils::write_bar(double fraction) const {
        assert(bar.has_value());
        if(fraction < 0) fraction = 0;
        else if(fraction > 1) fraction = 1;

        bar.value().filled_ratio = fraction;
        size_t width = bar.value().bar_width - bar.value().msg.size();
        size_t offset = bar.value().bar_width - static_cast<unsigned>(static_cast<double>(width) * fraction);

        cout << '\r' << bar.value().msg;
        cout.write(bar.value().full_bar.data() + std::move(offset), static_cast<long>(std::move(width)));
        cout << "] " << std::setw(3) << static_cast<int>(100 * fraction) << "% " << std::flush;
    }

    /**
     * @brief closes out the current fixed bar, allows a new one to be created
    */
    inline void cw_utils::end_bar() {
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
    inline cw_utils::bar_settings::bar_settings(size_t line_width, const string_view& msg, char symbol_full, char symbol_empty)
            : bar_width(line_width - sizeof("] 100%")),
            msg(string(msg) + " ["),
            full_bar(string(bar_width, symbol_full) + string(bar_width, symbol_empty)),
            bar_flusher('\r' + string(line_width, ' ') + '\r') {
        assert(this->msg.size() < bar_width && msg.find('\n') == msg.npos);
    }

    /**
     * @brief definition of cw_util global mutexes/bar settings to protect print and bar creation access
    */
    inline mutex cw_utils::print_mx;
    inline mutex cw_utils::bar_mx;
    inline optional<cw_utils::bar_settings> cw_utils::bar;

    /**
     * @brief adds one to numerator, and updates progress bar
    */
    inline void progress_bar::incr_numerator() {
        // update progress bar if change in completion surpasses granularity
        if(static_cast<double>(++numerator)/denominator >= progress_ratio + granularity) {
            progress_ratio += granularity;
            utils.request_write_bar(progress_ratio);
        }
    }

    /**
     * @brief destructor that finishes writing last line
    */
    inline progress_bar::~progress_bar() {
        utils.end_bar();
    }
} // namespace cw

#endif // CW_LOGGING_HPP