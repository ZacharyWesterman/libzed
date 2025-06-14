// Unlike arrays, generators are not stored in memory. They are evaluated on the fly.
// This makes them significantly more memory-efficient for large ranges of data.
// Both arrays and generators can be iterated over, and have similar functions like map, filter, and reduce.
// However, generators do not have all the functions that arrays do;
// They are meant to be iterated over once, and then discarded.

#include "../../z/core/join.hpp"
#include "../../z/core/range.hpp"
#include "catch/catch_amalgamated.hpp"
#include <iostream>

using z::core::array;
using z::core::generatorFrom;
using z::core::join;
using z::core::range;

TEST_CASE("Generator reduce", "[generator]") {
	auto sum10 = range(1, 11).reduce(0, [](auto a, auto b) { return a + b; });
	REQUIRE(sum10 == 55);

	auto mult10 = range(1, 11).reduce(1, [](auto a, auto b) { return a * b; });
	REQUIRE(mult10 == 3628800);

	auto max = range(1, 11).reduce(0, [](auto a, auto b) { return a > b ? a : b; });
	REQUIRE(max == 10);

	auto min = range(1, 11).reduce(100, [](auto a, auto b) { return a < b ? a : b; });
	REQUIRE(min == 1);
}

TEST_CASE("Generator map", "[generator]") {
	auto numbers = range(1, 11).map<int>([](auto i) { return i * 2; });
	auto numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 10);
	REQUIRE(numbers_array[0] == 2);
	REQUIRE(numbers_array[9] == 20);
}

TEST_CASE("Generator filter", "[generator]") {
	auto numbers = range(1, 21).filter([](auto i) { return i % 2 == 0; });
	auto numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 10);
	REQUIRE(numbers_array[0] == 2);
	REQUIRE(numbers_array[9] == 20);
}

TEST_CASE("Generator from initializer list rvalue", "[generator]") {
	auto numbers = generatorFrom({1, 2, 3, 4, 5});
	auto numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 5);
	REQUIRE(numbers_array[0] == 1);
	REQUIRE(numbers_array[4] == 5);

	auto strings = generatorFrom({"one", "two", "three"});
	auto strings_array = strings.collect();
	REQUIRE(strings_array.length() == 3);
	REQUIRE(strings_array[0] == "one");
	REQUIRE(strings_array[1] == "two");
	REQUIRE(strings_array[2] == "three");
}

TEST_CASE("Generator count", "[generator]") {
	auto count = range(1, 11).count();
	REQUIRE(count == 10);

	auto empty_count = range(0).count();
	REQUIRE(empty_count == 0);

	auto single_count = generatorFrom({42}).count();
	REQUIRE(single_count == 1);
}

TEST_CASE("Generator skip", "[generator]") {
	auto numbers = range(1, 21).skip(10);
	auto numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 10);
	REQUIRE(numbers_array[0] == 11);
	REQUIRE(numbers_array[9] == 20);
}

TEST_CASE("Generator limit", "[generator]") {
	auto numbers = range(1, 21).limit(10);
	auto numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 10);
	REQUIRE(numbers_array[0] == 1);
	REQUIRE(numbers_array[9] == 10);
}

TEST_CASE("Generator take", "[generator]") {
	auto numbers = range(1, 21);
	auto numbers_array = numbers.take(10);
	REQUIRE(numbers_array.length() == 10);
	REQUIRE(numbers_array[0] == 1);
	REQUIRE(numbers_array[9] == 10);

	numbers_array = numbers.take(5);
	REQUIRE(numbers_array.length() == 5);
	REQUIRE(numbers_array[0] == 11);
	REQUIRE(numbers_array[4] == 15);

	// Taking more than available items should return all items
	numbers_array = numbers.take(30);
	REQUIRE(numbers_array.length() == 5);
	REQUIRE(numbers_array[0] == 16);
	REQUIRE(numbers_array[4] == 20);
}

TEST_CASE("Generator chunk", "[generator]") {
	auto numbers = range(1, 17).chunk(5);
	auto numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 4);
	REQUIRE(numbers_array[0].length() == 5);
	REQUIRE(numbers_array[0][0] == 1);
	REQUIRE(numbers_array[0][4] == 5);

	// The last chunk may have fewer items
	REQUIRE(numbers_array[3].length() == 1);
	REQUIRE(numbers_array[3][0] == 16);

	// Chunking with a size larger than the range
	numbers = range(1, 11).chunk(20);
	numbers_array = numbers.collect();
	REQUIRE(numbers_array.length() == 1);
	REQUIRE(numbers_array[0].length() == 10);
}

TEST_CASE("Generator from std::map", "[generator]") {
	auto my_map = std::map<int, std::string>{{1, "one"}, {2, "two"}, {3, "three"}};
	auto generator = generatorFrom(my_map);

	auto result = generator.collect();
	REQUIRE(result.length() == 3);
	REQUIRE(result[0].first == 1);
	REQUIRE(result[0].second == "one");
	REQUIRE(result[1].first == 2);
	REQUIRE(result[1].second == "two");
	REQUIRE(result[2].first == 3);
	REQUIRE(result[2].second == "three");

	// Use a map that only exists as an rvalue
	auto rvalue_map = generatorFrom(std::map<int, std::string>{{4, "four"}, {5, "five"}});
	auto rvalue_result = rvalue_map.collect();
	REQUIRE(rvalue_result.length() == 2);
	REQUIRE(rvalue_result[0].first == 4);
	REQUIRE(rvalue_result[0].second == "four");
	REQUIRE(rvalue_result[1].first == 5);
	REQUIRE(rvalue_result[1].second == "five");
}
