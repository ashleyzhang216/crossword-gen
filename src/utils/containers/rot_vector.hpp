// ==================================================================
// Author: Ashley Zhang (ayz27@cornell.edu)
// Date:   2/15/2025
// Description: rotating immutable vector datatype
// ==================================================================

#ifndef CW_ROT_VECTOR_HPP
#define CW_ROT_VECTOR_HPP

#include "../../common/common_data_types.h"

using namespace common_data_types_ns;

namespace cw {
    /**
     * @brief immutable wrapper of std::vector whose equality operator is independent of the vector's rotation or direction
     * @note originally intended for usage in hashset with constraint cycles
     */
    template <typename T>
    class rot_vector {
        public:
            rot_vector(const vector<T>& data) : data(data) {}

            bool operator==(const rot_vector<T>& other) const {
                if(data.size() != other.data.size()) return false;
                if(data.size() == 0) return true;

                // same idea as finding if one string is a rotation of another
                // concat first vector to itself, second vector must be a subvector of the first doubled vector
                vector<T> lhslhs{data};
                lhslhs.insert(lhslhs.end(), data.begin(), data.end());

                // also check reverse direction for equality
                return std::search(lhslhs.begin(), lhslhs.end(), other.data.begin(),  other.data.end() ) != lhslhs.end() ||
                       std::search(lhslhs.begin(), lhslhs.end(), other.data.rbegin(), other.data.rend()) != lhslhs.end();
            }

            // allow hash function to access data
            friend size_t hash<cw::rot_vector<T> >::operator()(const cw::rot_vector<T>&) const;

        protected:
            vector<T> data;
    }; // class rot_vector
} // namespace cw

/**
 * @brief for hash function specialization for rot_vector
 */
namespace std {
    /**
     * @cite https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x/
     */
    template <typename T>
    struct hash<cw::rot_vector<T> > {
        size_t operator()(const cw::rot_vector<T>& vec) const {
            std::hash<T> hasher;
            size_t seed{vec.data.size()};

            // dirty way to ensure two different vecs considered equal by rotation give same hash value
            // results in potentially collisions between inequal rot_vector with same elements
            // a better way to do this is to initally rotate/reverse input data into lexographically smallest rotation in rot_vector constructor
            // to ensure order-dependent hash combining below combines elements of equal rot_vectors in the same order
            // but, as of now, rot_vector will not contain many elements so this better approach seems unnecessary for now
            vector<T> sorted_vec{vec.data.begin(), vec.data.end()};
            std::sort(sorted_vec.begin(), sorted_vec.end()); // TODO: improve this if needed

            for(const T& val : sorted_vec) {
                seed ^= hasher(val) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            }

            return seed;
        }
    };
} // namespace std

#endif // CW_ROT_VECTOR_HPP