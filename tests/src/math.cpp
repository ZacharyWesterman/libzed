#include "catch.hpp"

#include "../../z/math.hpp"

using z::math::factorial;
using z::math::fibonacci;

/**
 * @brief Less-than comparison for complex values.
 *
 * @param arg1 the left-hand operand.
 * @param arg2 the right-hand operand.
 *
 * @return \b True if the normalization of \b arg1 is
 * less than that of \b arg2. \b False otherwise.
 *
 * @threadsafe_function_yes
 */
bool operator<(const std::complex<long> &arg1, const std::complex<long> &arg2) {
	long norm1 = arg1.real() * arg1.real() + arg1.imag() * arg1.imag();
	long norm2 = arg2.real() * arg2.real() + arg2.imag() * arg2.imag();

	return norm1 < norm2;
}

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
