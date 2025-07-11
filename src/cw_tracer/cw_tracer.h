// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/13/2024
// Description: execution tracer and related object declarations for cw_csp performance analysis
// ==================================================================

#ifndef CW_TRACER_H
#define CW_TRACER_H

#include "../common/common_data_types.h"
#include "../utils/cw_utils.h"
#include "cw_tracer_data_types.h"
#include "../common/common_parent.h"

using namespace common_data_types_ns;
using namespace cw;
using namespace cw_tracer_data_types_ns;
using namespace common_parent_ns;

namespace cw_tracer_ns {
    /**
     * @brief internal tree node representation of one interval of time for a single execution span 
    */
    struct cw_trace_span {
        // basic constructor, records start timestamp
        cw_trace_span(ts_type_t type, const string& name, size_t id, const shared_ptr<cw_trace_span>& prev);

        // records end timestamp
        void resolve(size_t expected_id, ordered_json&& r);

        // returns true iff this span has been resolved
        bool resolved() { return end.has_value(); }

        // type of this span's event
        ts_type_t type;

        // description of this span's event
        string name;

        // id of this span, for invariant validation
        size_t id;

        // parent span that this span is nested in and is a subset of, or null if this span is the root span
        weak_ptr<cw_trace_span> prev;

        // timestamp right before this span started
        time_point<high_resolution_clock> start; 

        // child spans nested in, or happened during, this span
        vector<shared_ptr<cw_trace_span> > children;

        // timestamp right after this span ended, if finished
        optional<time_point<high_resolution_clock> > end;

        // context as to how or why this span ended
        ordered_json result;
    }; // cw_trace_span

    /**
     * @brief manages a tree of cw_trace_span representing the whole search execution
    */
    class cw_tracer : public common_parent {
        public:
            // start new span
            size_t start_span(ts_type_t type, const string& name);
            
            // end previous span
            void end_span(size_t id, ordered_json&& result);

            // write results into JSON file and resolves root span
            void save_result(const string& filepath, ordered_json&& result = ordered_json::object());

            // basic constructor, initializes root span
            cw_tracer(const string& init_name, bool enabled);

        private:
            // whether this object should do anything at all
            bool enabled;

            // next id to assign, for invariant checking
            size_t next_id;

            // root of whole execution tree
            shared_ptr<cw_trace_span> root;

            // node of current deepest unresolved span
            shared_ptr<cw_trace_span> cur;
    }; // cw_tracer

    // conversion from cw_trace_span to json
    void to_json(ordered_json& j, const shared_ptr<cw_trace_span>& span);

    /**
     * @brief manages calls to a cw_tracer object during a single timestep
     * @note does not inherit common_parent for speed
    */
    class cw_timestamper {
        public:
            // constructor to initialize new timestep
            cw_timestamper(cw_tracer& tracer, ts_type_t type, const string& name);

            // returns ref to json object for modification
            ordered_json& result();

            // desctructor to resolve the timestep this object was created to manage
            ~cw_timestamper();

            // explicitly not copyable  
            cw_timestamper(const cw_timestamper& other) = delete;            // copy constructor
            cw_timestamper& operator=(const cw_timestamper& other) = delete; // copy assignment

            // explicitly not movable
            cw_timestamper(cw_timestamper&&) = delete;            // move constructor
            cw_timestamper& operator=(cw_timestamper&&) = delete; // move assignment
                    
        private:
            // cw_tracer ref to make calls to
            cw_tracer& tracer;

            // id of timestep this object manages
            size_t id;

            // result attached to timestep, format specific to each timestep type
            ordered_json _result;
    }; // cw_timestamper
}; // cw_tracer_ns

#endif // CW_TRACER_H