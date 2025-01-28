#include "../../z/core.hpp"
#include "catch.hpp"

#define STRTEST(a) \
	SECTION("ASCII") { \
		const auto enc = z::ascii; \
		z::core::string<enc> string; \
		a; \
	} \
	SECTION("UTF-8") { \
		const auto enc = z::utf8; \
		z::core::string<enc> string; \
		a; \
	} \
	SECTION("UTF16") { \
		const auto enc = z::utf16; \
		z::core::string<enc> string; \
		a; \
	} \
	SECTION("UTF32") { \
		const auto enc = z::utf32; \
		z::core::string<enc> string; \
		a; \
	}

TEST_CASE("Constructing strings from various constants", "[string]") {
	STRTEST(
		THEN("Constructing from a wide string") {
			string = L"Test value";
			REQUIRE(string == "Test value");
		}
		THEN("Constructing from an integer") {
			string = 12345;
			REQUIRE(string == "12345");
		}
		THEN("Constructing from a float") {
			string = -123.45;
			REQUIRE(string == "-123.45");
		}
		THEN("Constructing from a complex number") {
			string = std::complex<double>(10, 2.5);
			REQUIRE(string == "10+2.5i");
		}
		THEN("Constructing from a wide character") {
			string = L'A';
			REQUIRE(string == 'A');
		}
		THEN("Constructing from an integer, using a different base") {
			string = z::core::string<enc>(255, 16, 4);
			REQUIRE(string == "00FF");
		}
		THEN("Constructing from a float, using scientific notation") {
			string = z::core::string<enc>(100000000.0);
			REQUIRE(string == "1e8");
		}
		THEN("Constructing from an integer, with a specific precision") {
			string = z::core::string<enc>::precision(10, 5);
			REQUIRE(string == "10.00000");
		}
		THEN("Constructing from a float, with a specific precision") {
			string = z::core::string<enc>::precision(10.3, 2);
			REQUIRE(string == "10.30");
		}
		THEN("Constructing from a float, with a very small fractional part that should be ignored") {
			string = z::core::string<enc>::precision(10.0004, 2);
			REQUIRE(string == "10.00");
		}
	);

	SECTION("ASCII") {
		z::core::string<z::ascii> string;
		string = L"Āā";
		REQUIRE(string == "??");
	}
}

TEST_CASE("Converting strings to various numeric types", "[string]") {
	STRTEST(
		THEN("Integer strings are detected correctly") {
			string = "12345";
			REQUIRE(string.type() == z::core::zstr::integer);
			REQUIRE(string.complex() == std::complex<double>(12345, 0));
			REQUIRE(string.floating() == 12345.0);
			REQUIRE(string.integer() == 12345);
		}
		THEN("Floating point strings are detected correctly") {
			string = "-123.45";
			REQUIRE(string.type() == z::core::zstr::floating);
			REQUIRE(string.complex() == std::complex<double>(-123.45, 0)); //Complex casting only gets a real part
			REQUIRE(string.floating() == -123.45);
			REQUIRE(string.integer() == -123); // Fractional part gets truncated
		}
		THEN("Floating point (scientific notation) strings are detected correctly") {
			string = "1.2345e3";
			REQUIRE(string.type() == z::core::zstr::floating);
			REQUIRE(string.complex() == std::complex<double>(1234.5, 0)); //Complex casting only gets a real part
			REQUIRE(string.floating() == 1234.5);
			REQUIRE(string.integer() == 1); // Anything after decimal point gets truncated.
		}
		THEN("Complex strings are detected correctly") {
			string = "-5+2i";
			REQUIRE(string.type() == z::core::zstr::complex);
			REQUIRE(string.complex() == std::complex<double>(-5, 2));
			//Non-complex casting only gets the real value
			REQUIRE(string.floating() == 0.0); //We can't cast this to a float, it's too complicated.
			REQUIRE(string.integer() == 0); //We can't cast this to an integer, it's too complicated.
		}
		THEN("Complex floating point strings are detected correctly") {
			string = "0.005e3-2.4i";
			REQUIRE(string.type() == z::core::zstr::complex);
			REQUIRE(string.complex() == std::complex<double>(5, -2.4));
			//Non-complex casting does not work
			REQUIRE(string.floating() == 0.0); //We can't cast this to a float, it's too complicated.
			REQUIRE(string.integer() == 0); //We can't cast this to an integer, it's too complicated.
		}
		THEN("Non-numeric strings are detected correctly") {
			string = "beans";
			REQUIRE(string.type() == z::core::zstr::string);
			REQUIRE(string.complex() == std::complex<double>(0,0));
			REQUIRE(string.floating() == 0.0);
			REQUIRE(string.integer() == 0);
		}
		THEN("Make sure boolean casting is equivalent to checking string length") {
			REQUIRE((bool)zstring("test") == true);
			REQUIRE((bool)zstring(" ") == true);
			REQUIRE((bool)zstring("0") == true);
			REQUIRE((bool)zstring("123") == true);
			REQUIRE((bool)zstring() == false);
		}
	);
}

TEST_CASE("Sub-strings", "[string]") {
	STRTEST(
		string = "This is an example string.";
		REQUIRE(string.substr(11, 7) == "example");
		REQUIRE(string.substr(17, -7) == "elpmaxe");
		REQUIRE(string.substr(-7, 7) == "string.");
		REQUIRE(string.substr(-7, 1000) == "string.");
		REQUIRE(string.substr(-1000, 4) == "This");
	);
}

TEST_CASE("String iterators", "[string]") {
	const char text[] = "Example string";
	STRTEST(
		string = text;
		int i = 0;
		for (char chr : string) {
			REQUIRE(chr == text[i]);
			++i;
		}
	);
}

TEST_CASE("String replacement", "[string]") {
	const char value1[] = "The quick brown fox jumps over the lazy dog.";
	const char value2[] = u8"The˽quick˽brown˽fox˽jumps˽over˽the˽lazy˽dog.";

	STRTEST(
		GIVEN("A string with many substrings to replace") {
			string = value1;
			THEN("Replace all occurrences") {
				REQUIRE(string.replace(' ', "˽") == value2);
			}
		}

		GIVEN("A simple string") {
			string = "I like frogs";

			THEN("Replacing a substring with a string of the same length should work") {
				REQUIRE(string.replace(2, 4, "hate") == "I hate frogs");
			}
			THEN("Replacing a substring with a shorter string should work") {
				REQUIRE(string.replace(2, 4, "eat") == "I eat frogs");
			}
			THEN("Replacing a substring with a longer string should work") {
				REQUIRE(string.replace(2, 4, "consume") == "I consume frogs");
			}
		}
	);
}

TEST_CASE("String splitting", "[string]") {
	STRTEST(
		string = "This is a test string.";
		z::core::string<enc> delim = " ";
		auto parts = z::core::split(string, delim);
		REQUIRE(parts.length() == 5);
		REQUIRE(parts[0] == "This");
		REQUIRE(parts[1] == "is");
		REQUIRE(parts[2] == "a");
		REQUIRE(parts[3] == "test");
		REQUIRE(parts[4] == "string.");
	);
}

TEST_CASE("String joining", "[string]") {
	STRTEST(
		z::core::string<enc> delim = " ";
		z::core::array<z::core::string<enc>> parts ({"This", "is", "a", "test", "string."});
		REQUIRE(z::core::join(parts, delim) == "This is a test string.");
	);
}

TEST_CASE("String comparison", "[string]") {
	STRTEST(
		string = "Test string";
		REQUIRE(string == "Test string");
		REQUIRE(string != "Test string.");
		REQUIRE(string < "Test string.");
		REQUIRE(string <= "Test string.");
		REQUIRE(string <= "Test string");
		REQUIRE(string > "Test strinf");
		REQUIRE(string >= "Test strinf.");
		REQUIRE(string >= "Test strinf");
	);
}

TEST_CASE("String case conversion", "[string]") {
	STRTEST(
		string = "This is a test string.";
		REQUIRE(string.upper() == "THIS IS A TEST STRING.");
		REQUIRE(string.lower() == "this is a test string.");
	);
}

TEST_CASE("String trimming", "[string]") {
	STRTEST(
		string = "  This is a test string.  ";
		REQUIRE(string.trim() == "This is a test string.");
		REQUIRE(string.trimLeft() == "This is a test string.  ");
		REQUIRE(string.trimRight() == "  This is a test string.");
	);
}

TEST_CASE("String filtering", "[string]") {
	STRTEST(
		string = "This is a test string.";
		REQUIRE(string.filter({'a', 'z'}) == "hisisateststring");
		REQUIRE(string.filter({'a', 'z'}, true) == "T    .");
		REQUIRE(string.filter({{'A', 'Z'}, {'a', 'z'}}) == "Thisisateststring");
		REQUIRE(string.filter({{'A', 'Z'}, {'a', 'z'}}, true) == "    .");
		REQUIRE(string.filter("aeiouyAEIOUY") == "iiaei");

		auto filter_lambda = [](uint32_t ch) { return ch % 2 == 0; };
		REQUIRE(string.filter(filter_lambda) == "Th   tt trn.");
	);
}
