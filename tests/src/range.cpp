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