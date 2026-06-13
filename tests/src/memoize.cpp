#include "../../z/all.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::memoize;

int add(int a, int b) noexcept {
	return a + b;
}

int multiply(int a, int b) noexcept {
	return a * b;
}

TEST_CASE("Memoization with function", "[memoize]") {
	memoize<int(int, int)> sum = add;

	for (int i = 0; i < 10; ++i) {
		auto result = sum(10, 20);
		REQUIRE(result == 30);
	}
}

TEST_CASE("Memoization caches distinct argument tuples", "[memoize]") {
	int invocation_count = 0;

	auto multiply_with_counter = [&invocation_count](int a, int b) noexcept {
		++invocation_count;
		return a * b;
	};

	memoize<int(int, int)> product(multiply_with_counter);

	REQUIRE(product(2, 3) == 6);
	REQUIRE(invocation_count == 1);

	REQUIRE(product(2, 3) == 6);
	REQUIRE(invocation_count == 1);

	REQUIRE(product(3, 2) == 6);
	REQUIRE(invocation_count == 2);

	REQUIRE(product(2, 3) == 6);
	REQUIRE(invocation_count == 2);
}

TEST_CASE("Memoized clear resets cached values", "[memoize]") {
	int calls = 0;
	memoize<int(int)> square([&calls](int x) -> int {
		++calls;
		return x * x;
	});

	REQUIRE(square(5) == 25);
	REQUIRE(calls == 1);

	REQUIRE(square(5) == 25);
	REQUIRE(calls == 1);

	square.clear();

	REQUIRE(square(5) == 25);
	REQUIRE(calls == 2);
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
