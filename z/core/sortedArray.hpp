#pragma once
#ifndef SORTEDARRAY_H_INCLUDED
#define SORTEDARRAY_H_INCLUDED

#include "array.hpp"

namespace z {
namespace core {
/**
 * @brief An extension of the core::array class
 * which attempts to keep all data sorted.
 *
 * @see array
 * @see refArray
 * @see sortedRefArray
 */
template <typename T>
class sortedArray : public array<T> {
public:
	/// Default constructor.
	sortedArray() {}

	/**
	 * @brief List-initialized constructor.
	 *
	 * Constructs the array with an arbitrary
	 * number of elements already contained.
	 * @note All elements will be automatically sorted.
	 *
	 * @param arg1 initializing data.
	 * @param args cont. initializing data.
	 */
	template <typename... Args>
	sortedArray(const T &arg1, const Args &...args) {
		this->init(arg1, args...);
	}

	/**
	 * @brief Add an object to the array.
	 *
	 * Finds an appropriate location and inserts the
	 * object into the array, such that the array
	 * remains sorted.
	 *
	 * @param object the data to add to the array.
	 *
	 * @return The index where the inserted object now resides.
	 */
	virtual int add(const T &object) override;

	/**
	 * @brief Check if a given object is in the array.
	 *
	 * Locates the desired index using a binary search,
	 * as the array is expected to be sorted.
	 *
	 * /note The objects this array contains must be trivially
	 * comparable, or have operator>() and operator<() defined.
	 *
	 * @param object the object to search for.
	 *
	 * @return The first index that the object was found at.
	 * @b -1 if it was not found.
	 */
	virtual int find(const T &object) const override;

	/**
	 * @brief Find an index where the given object
	 * can be inserted while keeping the array sorted.
	 *
	 * Locates the desired index using a binary search,
	 * as the array is expected to be sorted.
	 *
	 * @param object The object to search for.
	 * @param allowDuplicates Whether to allow multiple identical objects.
	 *
	 * @return The first index where the object can be inserted.
	 */
	virtual int findInsert(const T &object, bool allowDuplicates = true) const;

	// For sorted arrays, shuffle does nothing.
	virtual void shuffle() noexcept override {}

	// For sorted arrays, reverse does nothing.
	virtual void reverse() noexcept override {}
};

template <typename T>
int sortedArray<T>::add(const T &object) {
	int index = findInsert(object);

	this->insert(object, index);

	return index;
}

template <typename T>
int sortedArray<T>::find(const T &object) const {
	if (this->array_data.size() == 0) {
		return -1;
	}

	int left = 0;
	int right = this->array_data.size() - 1;

	while (left < right) {
		int center = (left + right) / 2;

		if (this->lt(this->array_data.at(center), object)) {
			left = center + 1;
		} else if (this->gt(this->array_data.at(center), object)) {
			right = center - 1;
		} else {
			return center;
		}
	}

	if (this->eq(this->array_data.at(left), object)) {
		return left;
	} else {
		return -1;
	}
}

template <typename T>
int sortedArray<T>::findInsert(const T &object, bool allowDuplicates) const {
	if (this->array_data.size() == 0) {
		return 0;
	}

	int left = 0;
	int right = this->array_data.size() - 1;

	while (left < right) {
		int center = (left + right) / 2;

		if (this->lt(this->array_data.at(center), object)) {
			left = center + 1;
		} else if (this->gt(this->array_data.at(center), object)) {
			right = center - 1;
		} else {
			return allowDuplicates ? center : -1;
		}
	}

	if (this->lt(this->array_data.at(left), object)) {
		return left + 1;
	} else if (allowDuplicates || !this->eq(this->array_data.at(left), object)) {
		return left;
	} else {
		return -1;
	}
}

} // namespace core
} // namespace z

#endif // SORTEDARRAY_H_INCLUDED
