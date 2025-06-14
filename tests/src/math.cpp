#include "catch/catch_amalgamated.hpp"

#include "../../z/math.hpp"

using z::math::levenschtein;

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
