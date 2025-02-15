// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   2/15/2025
// Description: id_obj_manager datatype
// ==================================================================

#ifndef CW_ID_OBJ_MANAGER_HPP
#define CW_ID_OBJ_MANAGER_HPP

#include "../../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw {
    /**
     * @brief helpers to check if struct has 'id' field of a certain type
    */
    template <typename T, typename F>
    struct has_id : std::false_type {};

    template <typename T>
    struct has_id<T, decltype(std::declval<T>().id)>
        : std::true_type {};

    /**
     * @brief helpers to check if struct has clone() function returning a unique_ptr to itself
    */
    template <typename T, typename = void>
    struct has_clone : std::false_type {};

    template <typename T>
    struct has_clone<T, std::void_t<decltype(std::declval<T>().clone())> > : std::is_same<decltype(std::declval<T>().clone()), std::unique_ptr<T> > {};

    /**
     * @brief holds vector of elements containing a unique size_t 'id' field equal to its index and clone() function
    */
    template <class T>
    requires std::conjunction_v<has_id<T, size_t>, has_clone<T> >
    class id_obj_manager {
        public:
            id_obj_manager () = default;
            ~id_obj_manager() = default;

            // for id placeholders
            static constexpr size_t INVALID_ID = ULONG_MAX;

            // add element
            void push_back(unique_ptr<T>&& ptr) {
                if(!vec.has_value()) {
                    vec = vector<unique_ptr<T> >();
                }

                cw_assert(vec.value().size() == ptr->id);
                vec.value().push_back(std::move(ptr));
            }

            // access size
            size_t size() const {
                if(!vec.has_value()) return 0ul;
                return vec.value().size();
            }

            // access vector element
            unique_ptr<T>& operator[](size_t n) {
                cw_assert(vec.has_value());
                cw_assert(vec.value()[n]->id == n);
                return vec.value()[n];
            }
            const unique_ptr<T>& operator[](size_t n) const {
                return const_cast<const unique_ptr<T>&>(const_cast<id_obj_manager*>(this)->operator[](n));
            }
            
            // ids of all managed elements
            vector<size_t> ids() const {
                vector<size_t> res(size());
                for(size_t i = 0; i < size(); ++i) {
                    res[i] = i;
                }
                return res;
            }

            // shorthand for iterators
            using iterator = typename vector<unique_ptr<T> >::iterator;
            using const_iterator = typename vector<unique_ptr<T> >::const_iterator;

            // iterators
            iterator begin() {
                return vec.has_value() ? vec->begin() : empty_vec.begin();
            }
            const_iterator begin() const {
                return vec.has_value() ? vec->begin() : empty_vec.begin();
            }
            iterator end() {
                return vec.has_value() ? vec->end() : empty_vec.end();
            }
            const_iterator end() const {
                return vec.has_value() ? vec->end() : empty_vec.end();
            }

            // deep copy
            id_obj_manager clone() const {
                id_obj_manager copy;

                if(vec.has_value()) {
                    vector<unique_ptr<T> > vec_copy;
                    vec_copy.reserve(vec->size());

                    for(const auto& ptr : vec.value()) {
                        vec_copy.emplace_back(ptr->clone());
                    }

                    copy.init(std::move(vec_copy));
                }
                return copy;
            }

            // explicitly not ordinarily copyable
            id_obj_manager(const id_obj_manager&) = delete;
            id_obj_manager& operator=(const id_obj_manager&) = delete;

            // movable
            id_obj_manager(id_obj_manager&& other) : id_obj_manager() {
                if(other.vec.has_value()) {
                    init(std::move(other.vec.value()));
                }
            }
            id_obj_manager& operator=(id_obj_manager&& other) {
                cw_assert(!vec.has_value()); // cannot hold existing objects
                if(other.vec.has_value()) {
                    init(std::move(other.vec.value()));
                }
                return *this;
            }

        protected:
            // underlying data
            optional<vector<unique_ptr<T> > > vec;

            // fallback for iterators when vec is empty
            static vector<unique_ptr<T> > empty_vec;

            // initialize with data
            void init(vector<unique_ptr<T> >&& data) {
                cw_assert(!vec.has_value());
                vec = std::move(data);
                for(size_t i = 0; i < size(); ++i) {
                    cw_assert(vec.value()[i]->id == i);
                }
            }
    };

    // define empty_vec
    template <class T>
    requires std::conjunction_v<has_id<T, size_t>, has_clone<T> >
    vector<unique_ptr<T> > id_obj_manager<T>::empty_vec = {};
} // namespace cw

#endif // CW_ID_OBJ_MANAGER_HPP