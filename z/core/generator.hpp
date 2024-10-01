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
	generator(const S &initial, std::function<const yield<T>(S &)> lambda) : state(initial), lambda(lambda) {}

	/// Custom iterator for generators to allow for range-based for loops.
	class iterator {
		std::function<const yield<T>(S &)> lambda;
		yield<T> current_yield;
		S *state;

	public:
		/**
		 * @brief Constructor.
		 * @param lambda The generator function.
		 * @param state The state data that may be mutated by the generator function.
		 * @param dummy If true, do not generate data. This is here just so range-based loop syntax will work.
		 */
		explicit iterator(std::function<const yield<T>(S &)> lambda, S *state, bool dummy = false) : lambda(lambda), state(state), current_yield({false, {}}) {
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
			current_yield = lambda(*state);
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
	iterator begin() {
		return iterator(lambda, &state);
	}

	/**
	 * @brief End iterator (end of the range)
	 * @return A dummy iterator indicating the end of the range.
	 */
	iterator end() {
		return iterator(lambda, &state, true);
	}

	/**
	 * @brief Get the next item from the generator.
	 *
	 * If there are no more items, the `.done` field of this yield object will be true.
	 *
	 * @return A yield object containing the next value, if any.
	 */
	inline yield<T> next() {
		return lambda(state);
	}

	/**
	 * @brief Consume all items from the generator, getting the total count.
	 * @return The number of items that were generated.
	 */
	inline long count() {
		return this
			->map<long>([](const T &state) {
				(void)state;
				return 1;
			})
			.reduce(0, std::plus());
	}

	/**
	 * @brief Concatenate all generator elements into an array.
	 * @return An array containing all elements from the generator.
	 */
	array<T> collect() {
		array<T> result;
		for (auto i : *this) {
			result.push(i);
		}
		return result;
	}

	/**
	 * @brief Applies a transformation function to each item that comes out of the generator.
	 *
	 * This function wraps the existing generator in another generator, effectively transforming
	 * each item on-the-fly as it's generated.
	 *
	 * @tparam U The type of items that this new generator yields.
	 * @param mapLambda A function that takes a constant reference to an element of type `T` and returns an element of type `U`.
	 * @return A new generator that yields the transformed elements.
	 */
	template <typename U>
	generator<U, S> map(std::function<U(const T &)> mapLambda) {
		auto lambda = this->lambda;

		return generator<U, S>(state, [lambda, mapLambda](S &state) {
			auto item = lambda(state);
			if (item.done) {
				return yield<U>{true};
			} else {
				return yield<U>{false, mapLambda(item.value)};
			}
		});
	}

	/**
	 * @brief Filters the generatred items based on a predicate and returns a new generator that yields only the items that satisfy the predicate.
	 *
	 * This function wraps the existing generator in another generator, and as each item is generated,
	 * applies the given lambda function as a predicate, and only yields items that satisfy the predicate.
	 *
	 * @param filterLambda A function that takes a constant reference to an item of type `T` and returns a boolean indicating whether
	 * the item should be yielded.
	 * @return A new generator that yields only items that satisfy the predicate.
	 */
	generator filter(std::function<T(const T &)> filterLambda) {
		auto lambda = this->lambda;

		return generator(state, [lambda, filterLambda](S &state) {
			auto val = lambda(state);
			while (!val.done) {
				if (filterLambda(val.value)) {
					return val;
				}
				val = lambda(state);
			}

			return val;
		});
	}

	/**
	 * @brief Reduces the generator to a single value by applying a binary operation cumulatively to all yielded values.
	 *
	 * This function applies a binary operation (provided as a lambda) to combine all yielded items into a single value.
	 * If the generator doesn yield anything, the provided default value is returned.
	 *
	 * @note Especially for long lists of items, this is significantly more memory-efficient than calling collect()and then
	 * reduce() on the resulting array, as an intermediate array does not need to be constructed.
	 *
	 * @param defaultValue The value to return if the array is empty.
	 * @param reduceLambda A function that takes two elements of type `T` and returns their combined result of type `T`.
	 * @return The result of the reduction operation.
	 */
	T reduce(const T &defaultValue, std::function<T(const T &, const T &)> reduceLambda) {
		auto result = lambda(state);

		if (result.done) {
			return defaultValue;
		}

		auto value = result.value;

		while (true) {
			result = lambda(state);
			if (result.done) {
				break;
			}
			value = reduceLambda(value, result.value);
		}

		return value;
	}
};

} // namespace core
} // namespace z