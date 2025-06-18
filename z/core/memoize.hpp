#pragma once

#include <functional>
#include <tuple>
#include <unordered_map>

namespace z {
namespace core {

// Hashing helper
namespace memoization {

/// @private
template <typename T>
inline void hash_combine(std::size_t &seed, const T &val) {
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

/// @private
template <typename tuple, std::size_t index = std::tuple_size<tuple>::value - 1>
struct tupleHasher {
	/// @private
	static void apply(std::size_t &seed, const tuple &tup) {
		tupleHasher<tuple, index - 1>::apply(seed, tup);
		hash_combine(seed, std::get<index>(tup));
	}
};

/// @private
template <typename tuple>
struct tupleHasher<tuple, 0> {
	/// @private
	static void apply(std::size_t &seed, const tuple &tup) {
		hash_combine(seed, std::get<0>(tup));
	}
};

/// @private
struct tupleHash {
	/// @private
	template <typename... T>
	std::size_t operator()(const std::tuple<T...> &t) const {
		std::size_t seed = 0;
		tupleHasher<std::tuple<T...>>::apply(seed, t);
		return seed;
	}
};
} // namespace memoization

/// @private
template <typename T>
class memoize;

/**
 * @brief A memoization class that caches the results of a function.
 *
 * This class is a function wrapper that caches the results of the function
 * based on the arguments passed to it. If the function is called with the same
 * arguments again, the cached result is returned instead of recalculating it.
 *
 * @note Naively memoizing recursive functions will not result in every call being memoized, only the top-level call.
 * In order to implement a truly memoized recursive function, you must pass the memoized function as a lambda *that captures itself*, e.g.
 * `memoize<int(int)> fib([&fib](int n) -> int { ... });`.
 *
 *
 * @tparam R The return type of the function.
 * @tparam Args The argument types of the function.
 */
template <typename R, typename... Args>
class memoize<R(Args...)> {
private:
	std::function<R(Args...)> lambda;
	std::unordered_map<std::tuple<Args...>, R, memoization::tupleHash> cache;

public:
	/**
	 * @brief Constructor with any function-like object.
	 * @param lambda The function to memoize.
	 */
	memoize(std::function<R(Args...)> &&lambda) : lambda(lambda) {}

	/**
	 * @brief Constructor with a function pointer.
	 *
	 * This constructor allows for syntax like `memoize<int(int, int)> sum = add;`.
	 *
	 * @param lambda The lambda to memoize.
	 */
	memoize(R lambda(Args...)) : lambda(lambda) {}

	memoize() = delete;

	/**
	 * @brief Function call operator.
	 *
	 * If the results for the given arguments are already cached,
	 * it returns the cached result. Otherwise, it calls the function,
	 * caches the result, and then returns it.
	 *
	 * @param args The arguments to pass to the function.
	 * @return The result of the function.
	 */
	R operator()(Args... args) {
		auto arg_list = std::make_tuple(args...);

		if (cache.find(arg_list) == cache.end()) {
			cache[arg_list] = lambda(args...);
		}
		return cache[arg_list];
	}

	/**
	 * @brief Clears the cache.
	 *
	 * This function clears the cache, allowing the memoized function to
	 * recalculate results for all arguments.
	 */
	void clear() {
		cache.clear();
	}
};

} // namespace core
} // namespace z
