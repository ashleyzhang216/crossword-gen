// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/13/2024
// Description: execution time tracker and related object declarations for cw_csp performance analysis
// ==================================================================

#ifndef CW_TIMETRACKER_H
#define CW_TIMETRACKER_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw_timetracker_ns {
    /**
     * @brief manages a tree of cw_timestep representing the whole search execution
    */
    class cw_timetracker {
        public:
            // start new timestep
            uint start_timestep(string name);
            
            // end previous timestep
            void end_timestep(uint id);

            // write results into JSON file
            void save_results(string filepath);

            // basic constructor, initializes root timestep
            cw_timetracker(string init_name);

        protected:
            /**
             * @brief internal tree node representation of one division of time for a single execution step 
            */
            struct cw_timestep {
                // description of this timestep
                string name;

                // id of this timestep, for invariant validation
                uint id;

                // parent step that this step is nested in and is a subset of, or this step is the root step if null
                weak_ptr<cw_timestep> prev;

                // timestamp right before this step started
                string pre; 

                // child timesteps nested in, or happened during, this timestep
                vector<shared_ptr<cw_timestep> > children;

                // timestamp right after this step ended, if finished
                optional<string> post; 

                // basic constructor
                cw_timestep(string name, uint id, shared_ptr<cw_timestep> parent);
            }; // cw_timestep

        private:
            // whether this object should do anything at all
            const bool enabled;

            // next id to assign, for invariant checking
            uint next_id;

            // root of whole execution tree
            shared_ptr<cw_timestep> root;

            // node of current deepest unresolved timestep
            shared_ptr<cw_timestep> cur;
    }; // cw_timetracker

    /**
     * @brief manages calls to a cw_timetracker object during a single timestep
    */
    class cw_timestamper {
        public:
            // constructor to initialize new timestep
            cw_timestamper(cw_timetracker& tracker, string name);

            // desctructor to resolve the timestep this object was created to manage
            ~cw_timestamper();
            
            // TODO: abide by rule of 5, make object non-copyable but movable
        
        private:
            // id of timestep this object manages
            uint id;
    }; // cw_timestamper
}; // cw_timetracker_ns

#endif // CW_TIMETRACKER_H