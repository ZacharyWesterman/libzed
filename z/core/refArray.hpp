#pragma once

#include <type_traits>

#include "array.hpp"

namespace z {
namespace core {
/**
 * @brief An extension of the core::array class which
 * assumes all elements to be pointers.
 *
 * @see array
 * @see sortedArray
 */
template <typename T>
class refArray : public array<T> {
private:
	// only allow this class to work with pointers
	static_assert(std::is_pointer<T>::value, "Template must be of pointer type.");

protected:
	virtual bool eq(const T &arg1, const T &arg2) const override {
		return (arg1 == arg2) || (arg1 && arg2 && equals(*arg1, *arg2));
	}

	virtual bool gt(const T &arg1, const T &arg2) const override {
		if (arg1 == arg2) {
			return false;
		} else if (arg1 && arg2) {
			return greater(*arg1, *arg2);
		} else {
			return arg1;
		}
	}

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
	refArray() {}

	/**
	 * @brief List-initialized constructor.
	 *
	 * Constructs the array with an arbitrary
	 * number of elements already contained.
	 *
	 * @param arg1 initializing data.
	 * @param args cont. initializing data.
	 */
	template <typename... Args>
	refArray(const T &arg1, const Args &...args) {
		this->init(arg1, args...);
	}

	virtual bool operator()(const T &arg1, const T &arg2) const override {
		return greater(*arg1, *arg2);
	}
};
} // namespace core
} // namespace z
