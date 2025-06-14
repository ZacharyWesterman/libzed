#include "../../z/core/memoize.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::memoize;

// Naive implementation of Fibonacci
long fib_naive(long n) {
	if (n <= 1) {
		return n;
	}
	return fib_naive(n - 1) + fib_naive(n - 2);
}

TEST_CASE("Benchmark memoization of Fibonacci vs naive implementation", "[!benchmark]") {
	const int fib_n = 40; // The Fibonacci number to compute

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
