#pragma once

namespace z {
namespace core {
/**
 * \interface indexable
 * \brief A base interface for all objects whose elements can be directly indexed.
 */
template <typename T>
class indexable {
public:
	/// Virtual destructor
	virtual ~indexable() noexcept {}

	/**
	 * \brief Function to get the object at the given index.
	 * \param index the index of the desired object.
	 * \return The object at the given index.
	 * \see operator[](int)
	 */
	virtual T at(int index) const = 0;

	/**
	 * \brief Function to get the object at the given index.
	 *
	 * Identical behavior to at(int), but allows indexing
	 * with square brackets.
	 *
	 * \param index the index of the desired object.
	 * \return The object at the given index.
	 * \see at(int)
	 */
	virtual T operator[](int index) const {
		return at(index);
	}
};
} // namespace core
} // namespace z
