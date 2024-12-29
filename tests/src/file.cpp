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

TEST_CASE("Get the current working directory", "[file]") {
	REQUIRE(z::file::dir() != "");
}

TEST_CASE("Get the filename of the executable", "[file]") {
	REQUIRE(z::file::basename(z::file::executable()) == "run_tests");
}

TEST_CASE("Check for file existence", "[file]") {
	REQUIRE(z::file::exists(z::file::executable()) == true);
	REQUIRE(z::file::exists("THIS FILE DOES NOT EXIST") == false);
}

TEST_CASE("Read file contents", "[file]") {
	const zstring expectedContents = "This is a test file.\nIt contains multiple lines.";
	const zstring realContents = z::file::read(z::file::execdir() + "/../data/file1.txt");

	REQUIRE(realContents == expectedContents);
}

TEST_CASE("Check file size", "[file]") {
	const zpath filename = z::file::execdir() + "/../data/file1.txt";

	// Reading an existing file works, and gives the right file size.
	REQUIRE(z::file::size(filename) == 48);

	// Reading a nonexistent file throws the correct exception.
	REQUIRE_THROWS_AS(z::file::size("THIS FILE DOES NOT EXIST"), z::file::unreadable);
}

TEST_CASE("Iterate over lines of a file with a generator", "[file]") {
	const zpath filename = z::file::execdir() + "/../data/file2.txt";

	int total = 0;
	for (auto line : z::file::lines(filename)) {
		total++;
		REQUIRE(line == "test");
	}

	REQUIRE(total == 5);
	REQUIRE(z::file::lines(filename).count() == 5);
}

TEST_CASE("Copy a file and then remove it", "[file]") {
	const zpath filename = z::file::execdir() + "/../data/file1.txt";
	const zpath newFilename = z::file::execdir() + "/../data/file1_copy.txt";

	z::file::copy(filename, newFilename);
	REQUIRE(z::file::exists(newFilename) == true);

	// Clean up
	z::file::remove(newFilename);
	REQUIRE(z::file::exists(newFilename) == false);
}

TEST_CASE("Load a dynamic library", "[file]") {
	z::file::library lib;
	REQUIRE(lib.load(z::file::execdir() + "/libtest") == true);

	REQUIRE(lib.good() == true);
	REQUIRE(lib.bad() == false);

	auto value_ptr = lib.symbol<int>("value");
	auto add = lib.function<int(int, int)>("add");

	REQUIRE(add != nullptr);
	REQUIRE(value_ptr != nullptr);
	REQUIRE(*value_ptr == 42);

	REQUIRE(add(30, 12) == 42);
}
