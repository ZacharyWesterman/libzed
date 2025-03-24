#include "../../z/core/range.hpp"
#include "catch/catch_amalgamated.hpp"

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

TEST_CASE("Range, take", "[range]") {
	auto r = z::core::range(10).take(5);
	REQUIRE(r.length() == 5);
	for (int i = 0; i < 5; i++) {
		REQUIRE(r[i] == i);
	}
}

TEST_CASE("Range, collect", "[range]") {
	auto r = z::core::range(10).collect();
	REQUIRE(r.length() == 10);
	for (int i = 0; i < 10; i++) {
		REQUIRE(r[i] == i);
	}
}

TEST_CASE("Range, count", "[range]") {
	auto c = z::core::range(10).count();
	REQUIRE(c == 10);
}

TEST_CASE("Range, empty", "[range]") {
	auto r = z::core::range(0);
	REQUIRE(r.count() == 0);
}

TEST_CASE("Range, infinite", "[range]") {
	auto r = z::core::range(0, z::core::infinity);
	for (int i = 0; i < 5; i++) {
		REQUIRE(r.next().value == i);
	}
}

TEST_CASE("Infinite sentinel", "[sentinel]") {
	auto s = z::core::infinity;
	REQUIRE_FALSE(s(0, 1));
	REQUIRE_FALSE(s(1, 1));
	REQUIRE_FALSE(s(1, 2));
	REQUIRE_FALSE(s(1, 100));
	REQUIRE_FALSE(s(100, 100));
	REQUIRE_FALSE(s(100, 1));
}
