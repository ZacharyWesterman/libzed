#include "../../z/file.hpp"
#include "catch.hpp"

TEST_CASE("Validate basename", "[file]") {
	REQUIRE(z::file::basename("/this/is/a/test") == "test");
	REQUIRE(z::file::basename("/filename") == "filename");
	REQUIRE(z::file::basename("/") == "/");
}

TEST_CASE("Validate dirname", "[file]") {
	REQUIRE(z::file::dirname("/this/is/a/test") == "/this/is/a");
	REQUIRE(z::file::dirname("/filename") == "/");
	REQUIRE(z::file::dirname("/") == "/");
}

TEST_CASE("Make sure we can get the current working directory", "[file]") {
	REQUIRE(z::file::dir() != "");
}

TEST_CASE("Make sure we can get the filename of the executable", "[file]") {
	REQUIRE(z::file::basename(z::file::executable()) == "run_tests");
}

TEST_CASE("Make sure we can check for file existence", "[file]") {
	REQUIRE(z::file::exists(z::file::executable()) == true);
	REQUIRE(z::file::exists("THIS FILE DOES NOT EXIST") == false);
}