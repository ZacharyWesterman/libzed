#include "catch.hpp"

#include "../../z/math.hpp"

using z::math::factorial;
using z::math::fibonacci;
using z::math::levenschtein;

TEST_CASE("Computing Fibonacci numbers", "[math]") {
	REQUIRE(fibonacci(0) == 0);
	REQUIRE(fibonacci(1) == 1);
	REQUIRE(fibonacci(2) == 1);
	REQUIRE(fibonacci(3) == 2);
	REQUIRE(fibonacci(4) == 3);
	REQUIRE(fibonacci(10) == 55);
	REQUIRE(fibonacci(15) == 610);
	REQUIRE(fibonacci(-4) == -2);
}

TEST_CASE("Calculate factorials", "[math]") {
	REQUIRE(factorial(0) == 1);
	REQUIRE(factorial(1) == 1);
	REQUIRE(factorial(2) == 2);
	REQUIRE(factorial(3) == 6);
	REQUIRE(factorial(4) == 24);
	REQUIRE(factorial(5) == 120);
	REQUIRE(factorial(6) == 720);
	REQUIRE(factorial(7) == 5040);
	REQUIRE(factorial(8) == 40320);
	REQUIRE(factorial(9) == 362880);
	REQUIRE(factorial(10) == 3628800);
	REQUIRE(factorial(11) == 39916800);
	REQUIRE(factorial(20) == 2432902008176640000);
}

TEST_CASE("Check for factorial overflow", "[math]") {
	bool overflowed = false;

	factorial(200, overflowed);
	REQUIRE(overflowed == true);

	factorial(20, overflowed);
	REQUIRE(overflowed == false);
}

TEST_CASE("Check complex number remainder calculation", "[math]") {
	std::complex<long> a(1, 2);
	std::complex<long> b(3, 4);

	REQUIRE((a % b) == std::complex<long>(1, 2));
	REQUIRE((b % a) == std::complex<long>(1, 0));
}

TEST_CASE("Calculate Levenshtein distance", "[math]") {
	REQUIRE(levenschtein("kitten"_zs, "sitting"_zs) == 3);
	REQUIRE(levenschtein("sitting"_zs, "kitten"_zs) == 3);
	REQUIRE(levenschtein("kitten"_zs, "kitten"_zs) == 0);
	REQUIRE(levenschtein("kitten"_zs, "kittens"_zs) == 1);
	REQUIRE(levenschtein("horse"_zs, "ros"_zs) == 3);

	// By default, if the distance exceeds 16, it will return the maximum integer value (completely different strings)
	REQUIRE(levenschtein("this is a long string"_zs, "golf ball"_zs) == std::numeric_limits<int>::max());
	// If we set the max to the maximum integer value, it will return the actual distance. This can be slow for very long strings, hence the default threshold.
	REQUIRE(levenschtein("this is a long string"_zs, "golf ball"_zs, std::numeric_limits<int>::max()) == 18);
}
