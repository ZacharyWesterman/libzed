#include "../../z/all.hpp"
#include "catch/catch_amalgamated.hpp"
#include <iostream>

using z::core::array;
using z::core::generatorFrom;
using z::core::join;
using z::core::range;

TEST_CASE("Generator iterator tests", "[generator]") {
	auto gen = range(0, 10);
	auto iter = gen.begin();

	SECTION("Values are correctly yielded") {
		REQUIRE(*iter == 0);
	}

	SECTION("Increment is correct") {
		++iter;
		REQUIRE(*iter == 1);
	}

	SECTION("Correctly compares to end") {
		auto end_iter = gen.end();

		for (int i = 0; i < 10; i++) {
			REQUIRE(*iter == i);
			REQUIRE(iter != end_iter);
			++iter;
		}

		REQUIRE_FALSE(iter != end_iter);
	}
}
