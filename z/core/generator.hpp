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
			.reduce(0, std::plus<long>());
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
	 * @brief Consume and discard all items from the generator.
	 * @note This function will consume the generator, and it will not be able to be used again.
	 * @return The number of items that were generated.
	 */
	long consume() {
		long count = 0;
		for (auto _ : *this) {
			count++;
		}
		return count;
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
	 * @note This function does not consume the generator directly, it just runs the given function each time an item is generated.
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

	/**
	 * @brief Zip this generator with another generator.
	 *
	 * This function combines two generators into a single generator that yields pairs of items from both generators.
	 * If one generator runs out of items, the resulting generator will stop yielding items.
	 *
	 * @param other The other generator to zip with.
	 * @return A new generator that yields pairs of items from both generators.
	 */
	template <typename U, typename S2>
	generator<std::pair<T, U>, generator<U, S2>> zip(generator<U, S2> &other) {
		typedef std::pair<T, U> pair_type;

		return generator<pair_type, generator<U, S2>>(other, [this](generator<U, S2> &otherGen) {
			auto item1 = next();
			if (item1.done) {
				return yield<pair_type>{true};
			}

			auto item2 = otherGen.next();
			if (item2.done) {
				return yield<pair_type>{true};
			}

			return yield<pair_type>{false, {item1.value, item2.value}};
		});
	}

	/**
	 * @brief Enumerate the items in this generator.
	 *
	 * This function wraps the existing generator in another generator that yields pairs of indices and items.
	 * The first item will have index 0, the second item will have index 1, and so on.
	 *
	 * @return A new generator that yields pairs of indices and items.
	 */
	generator<std::pair<long, T>, std::pair<long, generator<T, S>>> enumerate() {
		return generator<std::pair<long, T>, std::pair<long, generator<T, S>>>({0, *this}, [](std::pair<long, generator<T, S>> &state) {
			auto item = state.second.next();
			if (item.done) {
				return yield<std::pair<long, T>>{true};
			}
			return yield<std::pair<long, T>>{false, {state.first++, item.value}};
		});
	}

	/**
	 * @brief List the items in this generator which differ from another generator.
	 *
	 * This function is only useful for generators that yield items of the same type.
	 * (if the generators yielded different types, ALL items would be considered different!)
	 *
	 * As an example of how this works, if you have two generators that yield strings,
	 * generator1 yields "apple", "banana", "cherry", "melon",
	 * and generator2 yields "banana", "cherry", "date", "fig",
	 * then calling generator1.diff(generator2) will yield "apple" and "melon".
	 *
	 * @note If the other generator runs out of items, the rest of the items from this generator will be yielded,
	 * as they are considered different from nothing.
	 *
	 * @param other The other generator to compare against.
	 * @return A new generator that yields only items that are different from the items in the other generator.
	 */
	generator<T, std::pair<generator, yield<T>>> diff(generator &other) {
		return generator<T, std::pair<generator, yield<T>>>({other, other.next()}, [this](std::pair<generator, yield<T>> &state) {
			while (true) {
				auto item1 = next();
				if (item1.done) {
					return yield<T>{true};
				}

				if (state.second.done) {
					return yield<T>{false, item1.value}; // Yield the item from this generator, as the other generator is done
				}

				if (item1.value != state.second.value) {
					return yield<T>{false, item1.value}; // Yield the item from this generator, as it is different
				}

				// Move to the next item in the other generator
				state.second = state.first.next();
			}
		});
	}

	/**
	 * @brief Get chunks of items from the generator.
	 *
	 * This function will yield chunks of items from the generator,
	 * where each chunk is an array of items of *at most* the specified size.
	 * If the generator runs out of items, the last chunk may contain fewer items.
	 *
	 * @param chunkSize The size of each chunk.
	 * @return A new generator that yields arrays of items, each of *at most* the specified size.
	 */
	generator<array<T>, generator> chunk(long chunkSize) {
		return generator<array<T>, generator>(*this, [chunkSize](generator &state) {
			array<T> chunk;
			for (long i = 0; i < chunkSize; i++) {
				auto item = state.next();
				if (item.done) {
					if (chunk.length() == 0) {
						return yield<array<T>>{true}; // No more items, end the generator
					}
					break; // The chunk has data, yield it
				}
				chunk.push(item.value);
			}
			return yield<array<T>>{false, chunk}; // Return the current chunk
		});
	}
};

/**
 * @brief The type of the dereferenced value from an iterable.
 * This is used to determine the type of value that the generator will yield.
 * @tparam T
 */
template <typename T>
using deref_type = std::remove_const_t<std::remove_reference_t<decltype(*std::declval<decltype(std::declval<T>().begin())>())>>;

/**
 * @brief The type of the iterator for an iterable.
 * This is used to determine the type of iterator that the generator will use.
 * @tparam T
 */
template <typename T>
using iter_type = std::remove_const_t<decltype(std::declval<T>().begin())>;

/**
 * @brief Create a generator from an arbitrary iterable.
 * @tparam T The type of value that the iterable returns.
 * @param list The iterable to create a generator from.
 * @return A generator that will yield the items from the iterable.
 */
template <typename T>
generator<deref_type<T>, iter_type<T>> generatorFrom(const T &list) {
	return generator<deref_type<T>, iter_type<T>>(list.begin(), [&list](iter_type<T> &iter) {
		if (iter != list.end()) {
			auto ret = yield<deref_type<T>>{false, *iter};
			++iter; // Move to the next item
			return ret;
		}

		return yield<deref_type<T>>{true};
	});
}

/**
 * @brief Create a generator from an arbitrary temporary iterable.
 * @tparam T The type of value that the iterable returns.
 * @param list The iterable to create a generator from.
 * @return A generator that will yield the items from the iterable.
 */
template <typename T>
generator<deref_type<T>, std::pair<T, iter_type<T>>> generatorFrom(const T &&list) {
	return generator<deref_type<T>, std::pair<T, iter_type<T>>>({list, list.begin()}, [](std::pair<T, iter_type<T>> &state) {
		if (state.second != state.first.end()) {
			auto ret = yield<deref_type<T>>{false, *state.second};
			++state.second; // Move to the next item
			return ret;
		}

		return yield<deref_type<T>>{true};
	});
}

/**
 * @brief Create a generator from an initializer list.
 * @tparam T The type of value that the iterable returns.
 * @param list The initializer list to create a generator from.
 * @return A generator that will yield the items from the initializer list.
 */
template <typename T>
generator<T, std::pair<array<T>, long>> generatorFrom(std::initializer_list<T> list) {
	return generator<T, std::pair<array<T>, long>>({list, 0}, [&list](std::pair<array<T>, long> &state) {
		if (state.second < state.first.length()) {
			return yield<T>{false, state.first[state.second++]};
		}

		return yield<T>{true};
	});
}

} // namespace core
} // namespace z
