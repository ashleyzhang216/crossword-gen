// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/13/2024
// Description: execution time tracker and related object declarations for cw_csp performance analysis
// ==================================================================

#ifndef CW_TIMETRACKER_H
#define CW_TIMETRACKER_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"
#include "cw_timetracker_data_types.h"
#include "../common/common_parent.h"

using namespace common_data_types_ns;
using namespace cw;
using namespace cw_timetracker_data_types_ns;
using namespace common_parent_ns;

namespace cw_timetracker_ns {
    /**
     * @brief internal tree node representation of one interval of time for a single execution step 
    */
    struct cw_timestep {
        // basic constructor, starts timestep measurement
        cw_timestep(ts_type_t type, const string& name, size_t id, const shared_ptr<cw_timestep>& prev);

        // ends timestep measurement
        void resolve(size_t expected_id, basic_jason&& r);

        // returns true iff timestep has been resolved
        bool resolved() { return end.has_value(); }

        // type of this timestep
        ts_type_t type;

        // description of this timestep
        string name;

        // id of this timestep, for invariant validation
        size_t id;

        // parent step that this step is nested in and is a subset of, or this step is the root step if null
        weak_ptr<cw_timestep> prev;

        // timestamp right before this step started
        time_point<high_resolution_clock> start; 

        // child timesteps nested in, or happened during, this timestep
        vector<shared_ptr<cw_timestep> > children;

        // timestamp right after this step ended, if finished
        optional<time_point<high_resolution_clock> > end;

        // context as to how or why this timestep ended
        basic_jason result;
    }; // cw_timestep

    /**
     * @brief manages a tree of cw_timestep representing the whole search execution
    */
    class cw_timetracker : public common_parent {
        public:
            // start new timestep
            size_t start_timestep(ts_type_t type, const string& name);
            
            // end previous timestep
            void end_timestep(size_t id, basic_jason&& result);

            // write results into JSON file and resolves root timestep
            void save_results(const string& filepath);

            // basic constructor, initializes root timestep
            cw_timetracker(const string& init_name, bool enabled);

        private:
            // whether this object should do anything at all
            bool enabled;

            // next id to assign, for invariant checking
            size_t next_id;

            // root of whole execution tree
            shared_ptr<cw_timestep> root;

            // node of current deepest unresolved timestep
            shared_ptr<cw_timestep> cur;
    }; // cw_timetracker

    // conversion from cw_timestep to json
    void to_json(ordered_json& j, const shared_ptr<cw_timestep>& step);

    /**
     * @brief manages calls to a cw_timetracker object during a single timestep
     * @note does not inherit common_parent for speed
    */
    class cw_timestamper {
        public:
            // constructor to initialize new timestep
            cw_timestamper(cw_timetracker& tracker, ts_type_t type, const string& name);

            // returns ref to json object for modification
            basic_jason& result();

            // desctructor to resolve the timestep this object was created to manage
            ~cw_timestamper();

            // explicitly not copyable  
            cw_timestamper(const cw_timestamper& other) = delete;            // copy constructor
            cw_timestamper& operator=(const cw_timestamper& other) = delete; // copy assignment

            // explicitly not movable
            cw_timestamper(cw_timestamper&&) = delete;            // move constructor
            cw_timestamper& operator=(cw_timestamper&&) = delete; // move assignment
                    
        private:
            // cw_timetracker ref to make calls to
            cw_timetracker& tracker;

            // id of timestep this object manages
            size_t id;

            // result attached to timestep, format specific to each timestep type
            basic_jason _result;
    }; // cw_timestamper
}; // cw_timetracker_ns

#endif // CW_TIMETRACKER_H