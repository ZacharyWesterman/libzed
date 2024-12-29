#pragma once

#include <functional>
#include <map>
#include <tuple>

namespace z {
namespace core {

template <typename T>
class memoize;

/**
 * @brief A memoization class that caches the results of a function.
 *
 * This class is a function wrapper that caches the results of the function
 * based on the arguments passed to it. If the function is called with the same
 * arguments again, the cached result is returned instead of recalculating it.
 *
 * @note Recursive functions will not have every call memoized, only the initial call.
 * E.g. calling a recursive Fibonacci with fib(5) will memoize fib(5), but not fib(4), fib(3), etc.
 * As well, the initial call to fib(5) will do the full calculation, including all redundant recursive calls.
 *
 * @tparam R The return type of the function.
 * @tparam Args The argument types of the function.
 */
template <typename R, typename... Args>
class memoize<R(Args...)> {
private:
	std::function<R(Args...)> lambda;
	std::map<std::tuple<Args...>, R> cache;

public:
	/**
	 * @brief Constructor with a function.
	 * @param lambda The function to memoize.
	 */
	memoize(std::function<R(Args...)> &&lambda) : lambda(lambda) {}

	/**
	 * @brief Assignment operator with a function.
	 * @param lambda The function to memoize.
	 * @return This memoize object after the operation.
	 */
	memoize &operator=(std::function<R(Args...)> &&lambda) {
		this->lambda = lambda;
		cache.clear();
		return *this;
	}

	/**
	 * @brief Function call operator.
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
};

} // namespace core
} // namespace z