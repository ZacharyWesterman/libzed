#include "../../z/core/range.hpp"
#include "catch.hpp"

TEST_CASE("Range, forward iteration", "[range]") {
	long x = 0;
	for (auto i : z::core::range(16)) {
		REQUIRE(i == x++);
	}

	REQUIRE(x == 16);
}

TEST_CASE("Range, reverse iteration", "[range]") {
	long x = 16;
	for (auto i : z::core::range(16, 0, -1)) {
		REQUIRE(i == x--);
	}

	REQUIRE(x == 0);
}

TEST_CASE("Range, map", "[range]") {
	int x = 0;
	for (auto i : z::core::range(5).map<int>([](auto i) { return i * 2; })) {
		REQUIRE(i == x);
		x += 2;
	}
}

TEST_CASE("Range, filter", "[range]") {
	for (auto i : z::core::range(50).filter([](auto i) { return i % 5 == 0; })) {
		REQUIRE(i % 5 == 0);
	}
}

TEST_CASE("Range, reduce", "[range]") {
	auto sum = z::core::range(10).reduce(0, [](auto a, auto b) { return a + b; });
	REQUIRE(sum == 45);

	auto mult = z::core::range(1, 10).reduce(1, [](auto a, auto b) { return a * b; });
	REQUIRE(sum == 45);
}