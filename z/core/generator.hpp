#pragma once

#include "array.hpp"
#include <functional>

namespace z {
namespace core {

template <typename T>
struct yield {
	bool done;
	T value;
};

template <typename T, typename S>
class generator {
	S state;
	std::function<const yield<T>(S &)> lambda;

public:
	generator(const T &initial, std::function<const yield<T>(S &)> lambda) : state(initial), lambda(lambda) {}
	generator(std::function<const yield<T>(S &)> lambda) : state(), lambda(lambda) {}

	// Iterator class
	class iterator {
		std::function<const yield<T>(S &)> lambda;
		yield<T> current_yield;
		S state;

	public:
		explicit iterator(std::function<const yield<T>(S &)> lambda, const S &state, bool is_end = false) : lambda(lambda), state(state), current_yield({false, {}}) {
			if (!is_end) {
				++(*this); // Load the first value
			}
		}

		// Dereference to get the current value
		const T &operator*() const {
			return current_yield.value;
		}

		// Pre-increment to get the next value
		iterator &operator++() {
			current_yield = lambda(state);
			return *this;
		}

		// Comparison operator for range-based for loop
		bool operator!=(const iterator &other) const {
			return !current_yield.done;
		}
	};

	// Begin iterator (start of the range)
	iterator begin() const {
		return iterator(lambda, state);
	}

	// End iterator (end of the range)
	iterator end() const {
		return iterator(lambda, state, true);
	}

	array<T> collect() const {
		array<T> result;
		for (auto i : *this) {
			result.push(i);
		}
		return result;
	}
};
} // namespace core
} // namespace z