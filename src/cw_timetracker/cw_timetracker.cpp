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
cw_timestep::cw_timestep(ts_type_t type, const string& name, size_t id, const shared_ptr<cw_timestep>& prev) 
    : type(type),
      name(name),
      id(id),
      prev(prev),
      start(high_resolution_clock::now()),
      end(std::nullopt) {
    // do nothing, initializer list is enough
}

/**
 * @brief resolves this timestep
 * 
 * @param id the expected id of this timestep
 * @param r rvalue json obj for why/how this timestep ended
*/
void cw_timestep::resolve(size_t expected_id, basic_json&& r) {
    cw_assert(id == expected_id);
    cw_assert(!resolved());
    cw_assert(children.empty() || children.back()->resolved());

    result = r;
    end = high_resolution_clock::now();
}

// ############### cw_timetracker ###############

/**
 * @brief constructor for cw_timetracker for cw_csp, creates root timestep to encompass entire execution
 * 
 * @param init_name name for root level timestep
 * @param enabled behaviors hold for this object iff enabled is true, otherwise all calls are ignored
*/
cw_timetracker::cw_timetracker(const string& init_name, bool enabled) : common_parent((init_name + "timetracker"), VERBOSITY), enabled(enabled), next_id(0ul) {
    if(enabled) {
        root = make_shared<cw_timestep>(TS_CSP_TOTAL, init_name, next_id, nullptr);
        cur = root;
        ++next_id;
    }
}

/**
 * @brief starts new timestep, nested inside the current deepest unresolved timestep (i.e. previous call)
 * 
 * @param type the type of the new timestep
 * @param name the name or description of the new timestep
 * @returns id of timestep, which is needed upon timestep resolution for invariant checking 
*/
size_t cw_timetracker::start_timestep(ts_type_t type, const string& name) {
    if(enabled) {
        cw_assert(cur);
        cw_assert(!cur->resolved());
        cw_assert(cur->children.empty() || cur->children.back()->resolved());

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
 * @param result rvalue json obj for why/how this timestep ended
*/
void cw_timetracker::end_timestep(size_t id, basic_json&& result) {
    if(enabled) {
        cw_assert(cur);
        cur->resolve(id, std::move(result));
        cw_assert(cur == root || (cur = cur->prev.lock()));
    }
}

/**
 * @brief saves the results in a JSON file at the specified path
 * @pre this is the first time results are saved
 *
 * @param result the json result for the root timestep
*/
void cw_timetracker::save_results(const string& filepath, basic_json&& result) {
    if(enabled) {
        cw_assert(root);
        cw_assert(root == cur);
        cw_assert(!root->resolved());
        end_timestep(0ul, std::move(result));

        ordered_json j = root;
        std::ofstream file(filepath);
        file << std::setw(2) << j << endl;
    }
}

// ############### json conversion ###############

void cw_timetracker_ns::to_json(ordered_json& j, const shared_ptr<cw_timestep>& step) {
    cw_assert(step->end.has_value());
    j["type"] = step->type;
    j["name"] = step->name;
    j["result"] = step->result;
    j["duration_us"] = std::chrono::duration_cast<std::chrono::microseconds>(step->end.value() - step->start).count();
    j["children"] = step->children;
}

// ############### cw_timestamper ###############

/**
 * @brief constructor for cw_timestamper, executes start_timestep() call for its timestep
 * 
 * @param tracker ref to cw_timetracker to make calls to
 * @param type the type to assign to the cw_timestep this object manages
 * @param name the name to assigned to the cw_timestep this object manages
*/
cw_timestamper::cw_timestamper(cw_timetracker& tracker, ts_type_t type, const string& name)
    : tracker(tracker),
      id(tracker.start_timestep(type, name)),
      _result(basic_json::object()) {
    // do nothing, other initializations are enough
}

/**
 * @brief return ref to result json object for modification
 */
basic_json& cw_timestamper::result() {
    return _result;
}

/**
 * @brief destructor for cw_timestamper, executes end_timestep() call for its timestep
*/
cw_timestamper::~cw_timestamper() {
    tracker.end_timestep(id, std::move(_result));
}
