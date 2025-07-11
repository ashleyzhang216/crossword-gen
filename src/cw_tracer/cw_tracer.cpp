// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   6/13/2024
// Description: execution tracer and related object implementations for cw_csp performance analysis
// ==================================================================

#include "cw_tracer.h"

using namespace cw_tracer_ns;

// ############### cw_trace_span ###############

/**
 * @brief constructor for cw_trace_span, starts new span and records start timestamp
 * 
 * @param type the type of this span
 * @param name the name of this span
 * @param id the id of this span for invariant checking
 * @param prev ptr to parent span that this span is nested in and is a subset of, or null if this span is the root span
*/
cw_trace_span::cw_trace_span(ts_type_t type, const string& name, size_t id, const shared_ptr<cw_trace_span>& prev) 
    : type(type),
      name(name),
      id(id),
      prev(prev),
      start(high_resolution_clock::now()),
      end(std::nullopt) {
    // do nothing, initializer list is enough
}

/**
 * @brief resolves this span and records end timestamp
 * 
 * @param id the expected id of this span
 * @param r rvalue json obj for why/how this span ended
*/
void cw_trace_span::resolve(size_t expected_id, ordered_json&& r) {
    cw_assert(id == expected_id);
    cw_assert(!resolved());
    cw_assert(children.empty() || children.back()->resolved());

    result = r;
    end = high_resolution_clock::now();
}

// ############### cw_tracer ###############

/**
 * @brief constructor for cw_tracer for cw_csp, creates root span to encompass entire execution
 * 
 * @param init_name name for root level span
 * @param enabled behaviors hold for this object iff enabled is true, otherwise all calls are ignored
*/
cw_tracer::cw_tracer(const string& init_name, bool enabled) : common_parent((init_name + "tracer"), VERBOSITY), enabled(enabled), next_id(0ul) {
    if(enabled) {
        root = make_shared<cw_trace_span>(TS_CSP_TOTAL, init_name, next_id, nullptr);
        cur = root;
        ++next_id;
    }
}

/**
 * @brief starts new span, nested inside the current deepest unresolved span (i.e. previous call)
 * 
 * @param type the type of the new span
 * @param name the name or description of the new span
 * @returns id of span, which is needed upon span resolution for invariant checking 
*/
size_t cw_tracer::start_span(ts_type_t type, const string& name) {
    if(enabled) {
        cw_assert(cur);
        cw_assert(!cur->resolved());
        cw_assert(cur->children.empty() || cur->children.back()->resolved());

        // add next layer
        cur->children.push_back(make_shared<cw_trace_span>(type, name, next_id, cur));
        cur = cur->children.back();
        return next_id++;
    }

    return 0;
}

/**
 * @brief ends the current deepest unresolved span, whose id must match the id provided
 * 
 * @param id the id the span expected to be resolved
 * @param result rvalue json obj for why/how this span ended
*/
void cw_tracer::end_span(size_t id, ordered_json&& result) {
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
 * @param result the json result for the root span
*/
void cw_tracer::save_result(const string& filepath, ordered_json&& result) {
    if(enabled) {
        cw_assert(root);
        cw_assert(root == cur);
        cw_assert(!root->resolved());
        end_span(0ul, std::move(result));

        ordered_json j = root;
        std::ofstream file(filepath);
        file << std::setw(2) << j << endl;
    }
}

// ############### json conversion ###############

void cw_tracer_ns::to_json(ordered_json& j, const shared_ptr<cw_trace_span>& span) {
    cw_assert(span->end.has_value());
    j["type"] = span->type;
    j["name"] = span->name;
    j["duration_us"] = std::chrono::duration_cast<std::chrono::microseconds>(span->end.value() - span->start).count();
    j["result"] = span->result;
    j["children"] = span->children;
}

// ############### cw_timestamper ###############

/**
 * @brief constructor for cw_timestamper, executes start_timestep() call for its timestep
 * 
 * @param tracer ref to cw_tracer to make calls to
 * @param type the type to assign to the cw_timestep this object manages
 * @param name the name to assigned to the cw_timestep this object manages
*/
cw_timestamper::cw_timestamper(cw_tracer& tracer, ts_type_t type, const string& name)
    : tracer(tracer),
      id(tracer.start_timestep(type, name)),
      _result(ordered_json::object()) {
    // do nothing, other initializations are enough
}

/**
 * @brief return ref to result json object for modification
 */
ordered_json& cw_timestamper::result() {
    return _result;
}

/**
 * @brief destructor for cw_timestamper, executes end_timestep() call for its timestep
*/
cw_timestamper::~cw_timestamper() {
    tracer.end_timestep(id, std::move(_result));
}
