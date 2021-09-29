#include "catch.hpp"
#include "../../z/core.hpp"

#define STRTEST(a)\
SECTION("ASCII"){z::core::string<z::ascii> string; a;}\
SECTION("UTF-8"){z::core::string<z::utf8> string; a;}\
SECTION("UTF16"){z::core::string<z::utf16> string; a;}\
SECTION("UTF32"){z::core::string<z::utf32> string; a;}

TEST_CASE("Constructing strings from various constants", "[z::core::string]")
{
	STRTEST(
		string = L"Test value";
		REQUIRE(string == "Test value");
		string = 12345;
		REQUIRE(string == "12345");
		string = -123.45;
		REQUIRE(string == "-123.45");
		string = std::complex<double>(10, 2.5);
		REQUIRE(string == "10+2.5i");
	);

	SECTION("ASCII")
	{
		z::core::string<z::ascii> string;
		string = L"Āā";
		REQUIRE(string == "??");
	}
}

TEST_CASE("Converting strings to various numeric types", "[z::core::string]")
{
	STRTEST(
		string = "12345";
		REQUIRE(string.integer() == 12345);
		REQUIRE(string.type() == z::core::zstr::integer);
		string = "-123.45";
		REQUIRE(string.floating() == -123.45);
		REQUIRE(string.type() == z::core::zstr::floating);
		string = "1.2345e3";
		REQUIRE(string.floating() == 1234.5);
		REQUIRE(string.type() == z::core::zstr::floating);
		string = "-5+2i";
		REQUIRE(string.complex() == std::complex<double>(-5, 2));
		REQUIRE(string.type() == z::core::zstr::complex);
		string = "0.005e3-2.4i";
		REQUIRE(string.complex() == std::complex<double>(5, -2.4));
		REQUIRE(string.type() == z::core::zstr::complex);
		string = "beans";
		REQUIRE(string.type() == z::core::zstr::string);
	);
}

TEST_CASE("Sub-strings", "[z::core::string]")
{
	STRTEST(
		string = "This is an example string.";
		REQUIRE(string.substr(11, 7) == "example");
		REQUIRE(string.substr(17, -7) == "elpmaxe");
		REQUIRE(string.substr(-7, 7) == "string.");
		REQUIRE(string.substr(-7, 1000) == "string.");
		REQUIRE(string.substr(-1000, 4) == "This");
	);
}

TEST_CASE("String iterators", "[z::core::string]")
{
	const char text[] = "Example string";
	STRTEST(
		string = text;
		int i = 0;
		for (char chr : string)
		{
			REQUIRE(chr == text[i]);
			++i;
		}
	);
}
