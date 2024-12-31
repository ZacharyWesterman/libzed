#include "../../z/util/generic.hpp"
#include "../../z/util/exceptions.hpp"
#include "catch.hpp"

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

TEST_CASE("Validate array type casting", "[generic]") {
	z::util::generic a = {1, 2, 3};
	z::util::generic b = {1.0, 2.0, 3.0};
	z::util::generic c = {"1", "2", "3"};
	z::util::generic d = {z::cplx(1.0, 0.0), z::cplx(2.0, 0.0), z::cplx(3.0, 0.0)};

	REQUIRE(a.array().length() == 3);
	REQUIRE(b.array().length() == 3);
	REQUIRE(c.array().length() == 3);
	REQUIRE(d.array().length() == 3);

	REQUIRE(a.array()[0] == 1);
	REQUIRE(b.array()[0] == 1.0);
	REQUIRE(c.array()[0] == "1");
	REQUIRE(d.array()[0] == z::cplx(1.0, 0.0));
}

TEST_CASE("Validate arrayOr method", "[generic]") {
	z::util::generic a = {1, 2, 3};
	z::util::generic b = 5; // This will get coerced to an array when passed to arrayOr.

	REQUIRE(a.arrayOr(b).length() == 3);
	REQUIRE(a.arrayOr(b)[0] == 1);
	REQUIRE(a.arrayOr(b)[1] == 2);
	REQUIRE(a.arrayOr(b)[2] == 3);

	REQUIRE(b.arrayOr(a).length() == 3);
	REQUIRE(b.arrayOr(a)[0] == 1);
	REQUIRE(b.arrayOr(a)[1] == 2);
	REQUIRE(b.arrayOr(a)[2] == 3);
}

TEST_CASE("Validate numeric type detection", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};
	z::util::generic f;

	REQUIRE(a.numeric() == true);
	REQUIRE(b.numeric() == true);
	REQUIRE(c.numeric() == true);
	REQUIRE(d.numeric() == false);
	REQUIRE(e.numeric() == false);
	REQUIRE(f.numeric() == false);
}

TEST_CASE("Validate generic type string conversion", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};
	z::util::generic f;

	REQUIRE(a.typeString() == "int");
	REQUIRE(b.typeString() == "float");
	REQUIRE(c.typeString() == "complex");
	REQUIRE(d.typeString() == "string");
	REQUIRE(e.typeString() == "array");
	REQUIRE(f.typeString() == "void");
}

TEST_CASE("Validate generic type comparison", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};
	z::util::generic f;

	REQUIRE(a == a);
	REQUIRE(b == b);
	REQUIRE(c == c);
	REQUIRE(d == d);
	REQUIRE(e == e);
	REQUIRE(f == f);

	REQUIRE(a != b);
	REQUIRE(a != c);
	REQUIRE(a != d);
	REQUIRE(a != e);
	REQUIRE(a != f);

	REQUIRE(b != a);
	REQUIRE(b != c);
	REQUIRE(b != d);
	REQUIRE(b != e);
	REQUIRE(b != f);

	REQUIRE(c != a);
	REQUIRE(c != b);
	REQUIRE(c != d);
	REQUIRE(c != e);
	REQUIRE(c != f);

	REQUIRE(d != a);
	REQUIRE(d != b);
	REQUIRE(d != c);
	REQUIRE(d != e);
	REQUIRE(d != f);

	REQUIRE(e != a);
	REQUIRE(e != b);
	REQUIRE(e != c);
	REQUIRE(e != d);
	REQUIRE(e != f);

	REQUIRE(f != a);
	REQUIRE(f != b);
	REQUIRE(f != c);
	REQUIRE(f != d);
	REQUIRE(f != e);
}

TEST_CASE("Validate generic type equivalence", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};
	z::util::generic f;

	REQUIRE(a.equivalent(a) == true);
	REQUIRE(b.equivalent(b) == true);
	REQUIRE(c.equivalent(c) == true);
	REQUIRE(d.equivalent(d) == true);
	REQUIRE(e.equivalent(e) == true);
	REQUIRE(f.equivalent(f) == true);

	REQUIRE(a.equivalent(b) == true);
	REQUIRE(a.equivalent(c) == true);
	REQUIRE(a.equivalent(d) == true);
	REQUIRE(a.equivalent(e) == false);
	REQUIRE(a.equivalent(f) == false);

	REQUIRE(b.equivalent(a) == true);
	REQUIRE(b.equivalent(c) == true);
	REQUIRE(b.equivalent(d) == true);
	REQUIRE(b.equivalent(e) == false);
	REQUIRE(b.equivalent(f) == false);

	REQUIRE(c.equivalent(a) == true);
	REQUIRE(c.equivalent(b) == true);
	REQUIRE(c.equivalent(d) == true);
	REQUIRE(c.equivalent(e) == false);
	REQUIRE(c.equivalent(f) == false);

	REQUIRE(d.equivalent(a) == true);
	REQUIRE(d.equivalent(b) == true);
	REQUIRE(d.equivalent(c) == true);
	REQUIRE(d.equivalent(e) == false);
	REQUIRE(d.equivalent(f) == false);

	REQUIRE(e.equivalent(a) == false);
	REQUIRE(e.equivalent(b) == false);
	REQUIRE(e.equivalent(c) == false);
	REQUIRE(e.equivalent(d) == false);
	REQUIRE(e.equivalent(f) == false);

	REQUIRE(f.equivalent(a) == false);
	REQUIRE(f.equivalent(b) == false);
	REQUIRE(f.equivalent(c) == false);
	REQUIRE(f.equivalent(d) == false);
	REQUIRE(f.equivalent(e) == false);
}

TEST_CASE("Test math operations", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};

	// Addition
	REQUIRE((a + a) == 10);
	REQUIRE((a + b) == 10.0);
	REQUIRE((a + c) == z::cplx(10.0, 0.0));
	REQUIRE_THROWS_AS((a + d), z::util::nonnumeric);
	REQUIRE_THROWS_AS((a + e), z::util::nonnumeric);

	REQUIRE((b + a) == 10.0);
	REQUIRE((b + b) == 10.0);
	REQUIRE((b + c) == z::cplx(10.0, 0.0));
	REQUIRE_THROWS_AS((b + d), z::util::nonnumeric);
	REQUIRE_THROWS_AS((b + e), z::util::nonnumeric);

	REQUIRE((c + a) == z::cplx(10.0, 0.0));
	REQUIRE((c + b) == z::cplx(10.0, 0.0));
	REQUIRE((c + c) == z::cplx(10.0, 0.0));
	REQUIRE_THROWS_AS((c + d), z::util::nonnumeric);
	REQUIRE_THROWS_AS((c + e), z::util::nonnumeric);

	REQUIRE_THROWS_AS((d + a), z::util::nonnumeric);
	REQUIRE_THROWS_AS((d + b), z::util::nonnumeric);
	REQUIRE_THROWS_AS((d + c), z::util::nonnumeric);
	REQUIRE((d + d) == "55");
	REQUIRE_THROWS_AS((d + e), z::util::nonnumeric);

	REQUIRE_THROWS_AS((e + a), z::util::nonnumeric);
	REQUIRE_THROWS_AS((e + b), z::util::nonnumeric);
	REQUIRE_THROWS_AS((e + c), z::util::nonnumeric);
	REQUIRE_THROWS_AS((e + d), z::util::nonnumeric);
	REQUIRE_THROWS_AS((e + e), z::util::nonnumeric);

	// Subtraction
	REQUIRE((a - a) == 0);
	REQUIRE((a - b) == 0.0);
	REQUIRE((a - c) == z::cplx(0.0, 0));
}

TEST_CASE("Test generic type casting", "[generic]") {
	z::util::generic a = 5;
	z::util::generic b = 5.0;
	z::util::generic c = z::cplx(5.0, 0.0);
	z::util::generic d = "5";
	z::util::generic e = {1, 2, 3};

	REQUIRE(a.integer() == 5);
	REQUIRE(a.floating() == 5.0);
	REQUIRE(a.complex() == z::cplx(5.0, 0.0));
	REQUIRE(a.toString() == "5");

	REQUIRE(b.integer() == 5);
	REQUIRE(b.floating() == 5.0);
	REQUIRE(b.complex() == z::cplx(5.0, 0.0));
	REQUIRE(b.toString() == "5");

	REQUIRE(c.integer() == 5);
	REQUIRE(c.floating() == 5.0);
	REQUIRE(c.complex() == z::cplx(5.0, 0.0));
	REQUIRE(c.toString() == "5");

	REQUIRE(d.integer() == 5);
	REQUIRE(d.floating() == 5.0);
	REQUIRE(d.complex() == z::cplx(5.0, 0.0));
	REQUIRE(d.toString() == "5");

	REQUIRE(e.integer() == 0);
	REQUIRE(e.floating() == 0.0);
	REQUIRE(e.complex() == z::cplx(0.0, 0.0));
	REQUIRE(e.toString(true) == "[1,2,3]");
}

#endif