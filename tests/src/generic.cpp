#include "catch.hpp"

#include "../../z/util/generic.hpp"

// Only run these tests if std::variant is available.
#if __cplusplus >= 201703L

TEST_CASE("Validate generic type detection", "[generic]") {
	z::util::generic g;
	REQUIRE(g.is(z::util::generic::VOID) == true);

	g = 5;
	REQUIRE(g.is(z::util::generic::INT) == true);

	g = 5.0;
	REQUIRE(g.is(z::util::generic::FLOAT) == true);

	g = z::cplx(5.0, 5.0);
	REQUIRE(g.is(z::util::generic::COMPLEX) == true);

	g = "test";
	REQUIRE(g.is(z::util::generic::STRING) == true);

	g = {1, 2, 3};
	REQUIRE(g.is(z::util::generic::ARRAY) == true);
}

TEST_CASE("Validate generic type promotion", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};
	z::util::generic f;

	// Numbers can be promoted up to any numeric type, or up to a string.
	REQUIRE(a.promote(&b) == true);
	REQUIRE(a.promote(&c) == true);
	REQUIRE(a.promote(&d) == true);
	REQUIRE(b.promote(&c) == true);
	REQUIRE(b.promote(&d) == true);
	REQUIRE(c.promote(&d) == true);

	// Non-array types cannot be promoted to arrays.
	REQUIRE(a.promote(&e) == false);
	REQUIRE(b.promote(&e) == false);
	REQUIRE(c.promote(&e) == false);
	REQUIRE(d.promote(&e) == false);

	// Void types cannot be promoted.
	REQUIRE(a.promote(&f) == false);
	REQUIRE(b.promote(&f) == false);
	REQUIRE(c.promote(&f) == false);
	REQUIRE(d.promote(&f) == false);
	REQUIRE(e.promote(&f) == false);
}

TEST_CASE("Validate generic type reduction", "[generic]") {
	z::util::generic i = 5;
	z::util::generic f1 = 5.0;
	z::util::generic f2 = 5.1;
	z::util::generic c1 = z::cplx(5.0, 0.0);
	z::util::generic c2 = z::cplx(5.1, 0.0);
	z::util::generic c3 = z::cplx(5.0, 5.0);
	z::util::generic s1 = "5";
	z::util::generic s2 = "5.0";
	z::util::generic s3 = "5+i";
	z::util::generic s4 = "test";

	z::util::generic a = {1, 2, 3};
	z::util::generic v;

	REQUIRE(i.reduce() == false);	 // Integers are already the smallest type, so cannot be reduced.
	REQUIRE(f1.reduce() == true);	 // Floats can be reduced to integers.
	REQUIRE(f2.reduce() == false); // Floats that are not integers cannot be reduced.
	REQUIRE(c1.reduce() == true);	 // Complex numbers can be reduced to integers.
	REQUIRE(c2.reduce() == true);	 // Complex numbers may also be reduce to floats.
	REQUIRE(c3.reduce() == false); // Complex numbers that are not real cannot be reduced.

	// By default, strings are not reduced to numbers.
	REQUIRE(s1.reduce() == false);

	// However, if we specify that strings SHOULD be reduced to numbers, then they can be.
	REQUIRE(s1.reduce(true) == true);	 // Strings can be reduced to integers.
	REQUIRE(s2.reduce(true) == true);	 // Strings can be reduced to floats.
	REQUIRE(s3.reduce(true) == true);	 // Strings can be reduced to complex numbers.
	REQUIRE(s4.reduce(true) == false); // Strings that are not numbers cannot be reduced.

	// Arrays and void cannot be reduced.
	REQUIRE(a.reduce() == false);
	REQUIRE(v.reduce() == false);
}

#endif