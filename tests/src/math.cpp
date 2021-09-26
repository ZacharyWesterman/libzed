#include "catch.hpp"

#include "../../z/math.hpp"

TEST_CASE("Computing Fibonacci numbers", "[z::math::fibonacci]")
{
	REQUIRE(z::math::fibonacci(0) == 0);
	REQUIRE(z::math::fibonacci(1) == 1);
	REQUIRE(z::math::fibonacci(2) == 1);
	REQUIRE(z::math::fibonacci(3) == 2);
	REQUIRE(z::math::fibonacci(4) == 3);
	REQUIRE(z::math::fibonacci(10) == 55);
	REQUIRE(z::math::fibonacci(15) == 610);
	REQUIRE(z::math::fibonacci(-4) == -2);
}
