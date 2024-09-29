#pragma once

#include "array.hpp"
#include <functional>

namespace z {
namespace core {

/**
 * @brief The return value for generator functions.
 *
 * Generator functions use this struct to both return values,
 * and to indicate when the generator is finished.
 *
 * @tparam T The type of data that the generator returns.
 */
template <typename T>
struct yield {
	/// Whether the generator has run out of values to return.
	bool done;

	/// The next value that the generator will return.
	T value;
};

/**
 * @brief An arbitrary generator for producing sequential results on-the-fly.
 *
 * This class encapsulates generator functionality with any state data the generator may require.
 * Some ideal uses for this class include looping over large sets of data that can't all be
 * loaded at runtime, or loading data in batches while still outputting only a small bit at a time.
 *
 * @tparam T The type of data that the generator returns.
 * @tparam S The state data for the generator.
 */
template <typename T, typename S>
class generator {
	S state;
	std::function<const yield<T>(S &)> lambda;

public:
	/**
	 * @brief Constructor with an initial state.
	 * @param initial The initial value of the generator's state.
	 * @param lambda The generator function.
	 */
	generator(const T &initial, std::function<const yield<T>(S &)> lambda) : state(initial), lambda(lambda) {}

	/// Custom iterator for generators to allow for range-based for loops.
	class iterator {
		std::function<const yield<T>(S &)> lambda;
		yield<T> current_yield;
		S state;

	public:
		/**
		 * @brief Constructor.
		 * @param lambda The generator function.
		 * @param state The state data that may be mutated by the generator function.
		 * @param dummy If true, do not generate data. This is here just so range-based loop syntax will work.
		 */
		explicit iterator(std::function<const yield<T>(S &)> lambda, const S &state, bool dummy = false) : lambda(lambda), state(state), current_yield({false, {}}) {
			if (!dummy) {
				++(*this); // Load the first value
			}
		}

		/**
		 * @brief Get the current value from the generator.
		 * @return The last value that was generated.
		 */
		const T &operator*() const {
			return current_yield.value;
		}

		/**
		 * @brief Generate the next value.
		 * @return This iterator after getting the next value from the generator.
		 */
		iterator &operator++() {
			current_yield = lambda(state);
			return *this;
		}

		/**
		 * @brief Check if the generator can get more data.
		 * @return \b false if the generator is finished, \b true otherwise.
		 */
		bool operator!=(const iterator &other) const {
			return !current_yield.done;
		}
	};

	/**
	 * @brief Begin iterator (start of the range)
	 * @return An iterator that will give the first value in the generator.
	 */
	iterator begin() const {
		return iterator(lambda, state);
	}

	/**
	 * @brief End iterator (end of the range)
	 * @return A dummy iterator indicating the end of the range.
	 */
	iterator end() const {
		return iterator(lambda, state, true);
	}

	/**
	 * @brief Concatenate all generator elements into an array.
	 * @return An array containing all elements from the generator.
	 */
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