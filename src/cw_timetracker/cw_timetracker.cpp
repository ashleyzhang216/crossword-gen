// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/13/2024
// Description: execution time tracker and related object implementations for cw_csp performance analysis
// ==================================================================

#include "cw_timetracker.h"

using namespace cw_timetracker_ns;

// ############### cw_timestep ###############

/**
 * @brief constructor for cw_timestep, starts measurement of a new timestep
 * 
 * @param type the type of this timestep
 * @param name the name of this timestep
 * @param id the id of this timestep for invariant checking
 * @param prev ptr to parent step that this step is nested in and is a subset of, or this step is the root step if null
*/
cw_timetracker::cw_timestep::cw_timestep(ts_type_t type, string name, uint id, shared_ptr<cw_timestep> prev) 
    : type(type), 
      name(name), 
      id(id), 
      prev(prev), 
      start(system_clock::now()), 
      end(std::nullopt), 
      result(std::nullopt) {
    // do nothing, initializer list is enough
}

/**
 * @brief resolves this timestep
 * 
 * @param id the expected id of this timestep
 * @param result optional message for why/how this timestep ended
 * @throws assertion_failure_exception iff id does not match
*/
void cw_timetracker::cw_timestep::resolve(uint id, string result) {
    assert(this->id == id);
    assert(!resolved());
    assert(children.empty() || children.back()->resolved());

    end = system_clock::now();
    if(result != "") this->result = result;
}

// ############### cw_timetracker ###############

/**
 * @brief constructor for cw_timetracker for cw_csp, creates root timestep to encompass entire execution
 * 
 * @param init_name name for root level timestep
 * @param enabled behaviors hold for this object iff enabled is true, otherwise all calls are ignored
*/
cw_timetracker::cw_timetracker(string init_name, bool enabled) : enabled(enabled), next_id(0) {
    if(enabled) {
        root = make_shared<cw_timestep>(TS_CSP_TOTAL, init_name, next_id, nullptr);
        cur = root;
        next_id++;
    }
}

/**
 * @brief starts new timestep, nested inside the current deepest unresolved timestep (i.e. previous call)
 * 
 * @param type the type of the new timestep
 * @param name the name or description of the new timestep
 * @returns id of timestep, which is needed upon timestep resolution for invariant checking 
*/
uint cw_timetracker::start_timestep(ts_type_t type, string name) {
    if(enabled) {
        assert(cur);
        assert(!cur->resolved());
        assert(cur->children.empty() || cur->children.back()->resolved());

        // add next layer
        cur->children.push_back(make_shared<cw_timestep>(type, name, next_id, cur));
        cur = cur->children.back();
        return next_id++;
    }

    return 0;
}

/**
 * @brief ends the current deepest unresolved timestep, whose id must match the id provided
 * 
 * @param id the id the timestep expected to be resolved
 * @param result optional message for why/how this timestep ended
 * @throws assertion_failure_exception iff id does not match
*/
void cw_timetracker::end_timestep(uint id, string result) {
    if(enabled) {
        assert(cur);
        cur->resolve(id, result);
        assert(cur = cur->prev.lock());
    }
}

/**
 * @brief saves the results in a JSON file at the specified path
 * @throws assertion_failure_exception iff not all timesteps are resolved
*/
// void cw_timetracker::save_results(string filepath) {
//     if(enabled) {
//         assert(root == cur);
//         assert(root->end.has_value());

//         // TODO: implement
//     }
// }

// ############### cw_timestamper ###############

/**
 * @brief constructor for cw_timestamper, executes start_timestep() call for its timestep
 * 
 * @param tracker ref to cw_timetracker to make calls to
 * @param type the type to assign to the cw_timestep this object manages
 * @param name the name to assigned to the cw_timestep this object manages
*/
cw_timestamper::cw_timestamper(cw_timetracker& tracker, ts_type_t type, string name) : tracker(tracker), id(tracker.start_timestep(type, name)) {
    // do nothing, other initializations are enough
}


/**
 * @brief destructor for cw_timestamper, executes end_timestep() call for its timestep
*/
cw_timestamper::~cw_timestamper() {
    if(result.has_value()) {
        tracker.end_timestep(id, result.value());
    } else {
        tracker.end_timestep(id);
    }
}
