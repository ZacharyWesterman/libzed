#include "../../z/core/memoize.hpp"
#include "../../z/system/sleep.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::memoize;
using z::system::sleep;

// Naive implementation of Fibonacci
long fib_naive(long n) {
	if (n <= 1) {
		return n;
	}
	return fib_naive(n - 1) + fib_naive(n - 2);
}

TEST_CASE("Benchmark memoization of Fibonacci vs naive implementation", "[memoization][fibonacci]") {
	const int fib_n = 30; // The Fibonacci number to compute

	memoize<long(long)> fib([&fib](long n) -> long {
		if (n <= 1) {
			return n;
		}
		return fib(n - 1) + fib(n - 2);
	});

	BENCHMARK("Repeated invocations of memoized Fibonacci") {
		return fib(fib_n);
	};

	BENCHMARK("Initial speed of the first invocation of memoized Fibonacci") {
		memoize<long(long)> fib2([&fib2](long n) -> long {
			if (n <= 1) {
				return n;
			}
			return fib2(n - 1) + fib2(n - 2);
		});

		return fib2(fib_n);
	};

	BENCHMARK("Binet's formula for Fibonacci") {
		auto fib_binet = [](long n) -> long {
			const double phi = (1 + std::sqrt(5)) / 2;
			return (std::pow(phi, n) - std::pow(-phi, -n)) / std::sqrt(5);
		};
		return fib_binet(fib_n);
	};

	BENCHMARK("Iterative Fibonacci") {
		auto fib_iterative = [](long n) {
			if (n <= 1) {
				return n;
			}
			long a = 0, b = 1;
			for (long i = 2; i <= n; ++i) {
				long temp = a + b;
				a = b;
				b = temp;
			}
			return b;
		};
		return fib_iterative(fib_n);
	};

	BENCHMARK("Naive recursive Fibonacci") {
		return fib_naive(fib_n);
	};
}

// A slow function that simulates a long computation
long slow_function(long n) {
	sleep(10);		// Simulate a slow function
	return n * n; // Just return the square of the number
}

TEST_CASE("Memoization of a very slow function", "[memoization][slow_function]") {
	memoize<long(long)> memoized_slow_function = slow_function;
	int func_param = 10; // Example parameter for the slow function

	BENCHMARK("Memoized slow function first invocation") {
		memoize<long(long)> memoized_slow_function_first = slow_function;
		return memoized_slow_function_first(func_param);
	};

	// Get the first invocation of the memoized function,
	// so that the cache is populated for subsequent benchmarks.
	memoized_slow_function(func_param);
	BENCHMARK("Repeated invocations of memoized slow function") {
		return memoized_slow_function(func_param);
	};

	BENCHMARK("Slow function without memoization") {
		return slow_function(func_param);
	};
}
