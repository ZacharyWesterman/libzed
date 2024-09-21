#pragma once

#include "indexable.hpp"
#include "iterable.hpp"

namespace z {
namespace core {
/**
 * \interface arrayLike
 * \brief An interface for all objects that can be both iterated over and directly indexed.
 */
template <typename T, typename ITER>
class arrayLike : public indexable<T>, public iterable<ITER> {
public:
	/**
	 * \brief Get the length of the array.
	 * \return The number of objects in the array.
	 */
	virtual int length() const noexcept = 0;
};
} // namespace core
} // namespace z
