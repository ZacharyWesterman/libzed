#include "../../z/core/memoize.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::memoize;

int add(int a, int b) noexcept {
	return a + b;
}

TEST_CASE("Memoization with function", "[memoize]") {
	memoize<int(int, int)> sum = add;

	for (int i = 0; i < 10; ++i) {
		auto result = sum(10, 20);
		REQUIRE(result == 30);
	}
}

TEST_CASE("Memoization with lambda", "[memoize]") {
	memoize<int(int)> fib([&fib](int n) -> int {
		if (n <= 1) {
			return n;
		}
		return fib(n - 1) + fib(n - 2);
	});

	REQUIRE(fib(10) == 55);
	REQUIRE(fib(40) == 102334155);
}
