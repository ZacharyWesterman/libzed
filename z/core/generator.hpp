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

/// Custom iterator for generators to allow for range-based for loops.
template <typename T, typename S>
class generatorIter {
	std::function<const yield<T>(S &)> lambda;
	S state;
	yield<T> current_yield;

public:
	/**
	 * @brief Constructor.
	 * @param lambda The generator function.
	 * @param state The state data that may be mutated by the generator function.
	 * @param dummy If true, do not generate data. This is here just so range-based loop syntax will work.
	 */
	explicit generatorIter(std::function<const yield<T>(S &)> lambda, const S &state, bool dummy = false) : lambda(lambda), state(state), current_yield({false, {}}) {
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
	generatorIter &operator++() {
		current_yield = lambda(state);
		return *this;
	}

	/**
	 * @brief Check if the generator can get more data.
	 * @return \b false if the generator is finished, \b true otherwise.
	 */
	bool operator!=(const generatorIter &other) const {
		(void)other;
		return !current_yield.done;
	}
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
class generator : public iterable<generatorIter<T, S>> {
	S state;
	std::function<const yield<T>(S &)> lambda;

	struct countedState {
		long count;
		S state;
	};

public:
	/**
	 * @brief Constructor with an initial state.
	 * @param initial The initial value of the generator's state.
	 * @param lambda The generator function.
	 */
	generator(const S &initial, std::function<const yield<T>(S &)> lambda) : state(initial), lambda(lambda) {}

	/**
	 * @brief Begin iterator (start of the range)
	 * @return An iterator that will give the first value in the generator.
	 */
	generatorIter<T, S> begin() const noexcept override {
		return generatorIter<T, S>(lambda, state);
	}

	/**
	 * @brief End iterator (end of the range)
	 * @return A dummy iterator indicating the end of the range.
	 */
	generatorIter<T, S> end() const noexcept override {
		return generatorIter<T, S>(lambda, state, true);
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
	 * @brief Get the total count of items that will be generated.
	 * @warning This function will consume the generator, and it will not be able to be used again.
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
	 * @note This function will consume the generator, and it will not be able to be used again.
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
	 * @brief Take a certain number of items from the generator.
	 *
	 * This function will take a certain number of items from the generator, or all items if there are fewer than the requested count.
	 *
	 * @param count The number of items to take.
	 * @return An array containing the taken items.
	 */
	array<T> take(int count) {
		array<T> result;
		result.increase(count);

		for (int i = 0; i < count; i++) {
			auto item = next();
			if (item.done) {
				break;
			}
			result.push(item.value);
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

	/**
	 * @brief Binds a function to run each time an item comes out of the generator.
	 *
	 * This function wraps the existing generator function in another function, effectively binding extra logic to this generator.
	 * each item on-the-fly as it's generated.
	 *
	 * @param newLambda A function that takes a constant reference to an element of type `T` and returns nothing.
	 * @return A reference to this generator, with the new function bound.
	 */
	generator &forEach(std::function<void(const T &)> newLambda) {
		auto lambda = this->lambda;
		this->lambda = [lambda, newLambda](S &state) {
			auto item = lambda(state);
			if (!item.done) {
				newLambda(item.value);
			}
			return item;
		};

		return *this;
	}

	/**
	 * @brief Skips a certain number of items from the generator.
	 *
	 * This function will skip a certain number of items from the generator, or all items if there are fewer than the requested count.
	 *
	 * @note This function does not consume the generator directly, it just skips items as they are generated.
	 *
	 * @param count The number of items to skip.
	 * @return A new generator that skips the given number of items.
	 */
	generator<T, countedState> skip(long count) {
		auto lambda = this->lambda;

		return generator<T, countedState>({count, state}, [lambda](countedState &state) {
			for (long i = 0; i < state.count; i++) {
				auto item = lambda(state.state);
				if (item.done) {
					return item;
				}
			}
			state.count = 0;

			return lambda(state.state);
		});
	}

	/**
	 * @brief Limits the number of items that the generator will yield.
	 *
	 * @param count The maximum number of items to yield.
	 * @return A new generator that yields the given number of items.
	 */
	generator<T, countedState> limit(long count) {
		auto lambda = this->lambda;

		return generator<T, countedState>({count, state}, [lambda](countedState &state) {
			if (state.count <= 0) {
				return yield<T>{true};
			}

			auto item = lambda(state.state);
			if (item.done) {
				return item;
			}

			state.count--;

			return item;
		});
	}
};

} // namespace core
} // namespace z