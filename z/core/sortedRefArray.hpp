#pragma once

#include <type_traits>

#include "sortedArray.hpp"

namespace z {
namespace core {
/**
 * @brief An extension of the core::sortedArray class,
 * specialized for pointers.
 *
 * Unlike with the sortedArray, elements are
 * assumed to be pointers and are sorted
 * with this in mind.
 *
 * @note Rather than deal with the diamond problem or use
 * a C++ version > C++11, this class inherits from sortedArray
 * and reimplements refArray.
 *
 * @see array
 * @see refArray
 * @see sortedArray
 */
template <typename T>
class sortedRefArray : public sortedArray<T> {
private:
	// only allow this class to work with pointers
	static_assert(std::is_pointer<T>::value, "Template must be of pointer type.");

protected:
	/**
	 * @brief Check if two objects are equal.
	 *
	 * This member function allows object comparison to be
	 * different for different array types (e.g. reference arrays
	 * will need to sort by value after dereference).
	 *
	 * @param arg1 First object to compare.
	 * @param arg2 Second object to compare.
	 *
	 * @return True if the objects are equal, false otherwise.
	 */
	virtual bool eq(const T &arg1, const T &arg2) const override {
		return (arg1 == arg2) || (arg1 && arg2 && equals(*arg1, *arg2));
	}

	/**
	 * @brief Check if one object is greater than another.
	 *
	 * This member function allows object comparison to be
	 * different for different array types (e.g. reference arrays
	 * will need to sort by value after dereference).
	 *
	 * @param arg1 First object to compare.
	 * @param arg2 Second object to compare.
	 *
	 * @return True if arg1 is greater than arg2, false otherwise.
	 */
	virtual bool gt(const T &arg1, const T &arg2) const override {
		if (arg1 == arg2) {
			return false;
		} else if (arg1 && arg2) {
			return greater(*arg1, *arg2);
		} else {
			return arg1;
		}
	}

	/**
	 * @brief Check if one object is less than another.
	 *
	 * This member function allows object comparison to be
	 * different for different array types (e.g. reference arrays
	 * will need to sort by value after dereference).
	 *
	 * @param arg1 First object to compare.
	 * @param arg2 Second object to compare.
	 *
	 * @return True if arg1 is less than arg2, false otherwise.
	 */
	virtual bool lt(const T &arg1, const T &arg2) const override {
		if (arg1 == arg2) {
			return false;
		} else if (arg1 && arg2) {
			return lesser(*arg1, *arg2);
		} else {
			return arg2;
		}
	}

public:
	/// Default constructor.
	sortedRefArray() {}

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
	sortedRefArray(const T &arg1, const Args &...args) {
		this->init(arg1, args...);
	}

	/**
	 * @brief Callable operator.
	 *
	 * This is used to compare array elements for sorting.
	 *
	 * @param arg1 The first element to compare.
	 * @param arg2 The second element to compare.
	 * @return \b true if arg1 is greater than arg2.
	 */
	virtual bool operator()(const T &arg1, const T &arg2) const override {
		return greater(*arg1, *arg2);
	}
};
} // namespace core
} // namespace z
