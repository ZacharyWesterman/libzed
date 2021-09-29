#include "catch.hpp"
#include "../../z/core.hpp"

#define STRTEST(a)\
SECTION("ASCII"){z::core::string<z::ascii> string; a;}\
SECTION("UTF-8"){z::core::string<z::utf8> string; a;}\
SECTION("UTF16"){z::core::string<z::utf16> string; a;}\
SECTION("UTF32"){z::core::string<z::utf32> string; a;}

TEST_CASE("Constructing strings from various constants", "[z::core::string]")
{
	#define STRTEST1()\
	string = L"Test value";\
	REQUIRE(string == "Test value");\
	string = 12345;\
	REQUIRE(string == "12345");\
	string = -123.45;\
	REQUIRE(string == "-123.45");\
	string = std::complex<double>(10, 2.5);\
	REQUIRE(string == "10+2.5i");\

	SECTION("ASCII")
	{
		z::core::string<z::ascii> string;
		STRTEST1();
		string = L"Āā";
		REQUIRE(string == "??");
	}

	SECTION("UTF-8")
	{
		z::core::string<z::utf8> string;
		STRTEST1();
		string = L"Āā";
		REQUIRE(string == u8"Āā");
	}

	SECTION("UTF16")
	{
		z::core::string<z::utf16> string;
		STRTEST1();
	}

	SECTION("UTF32")
	{
		z::core::string<z::utf32> string;
		STRTEST1();
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
