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

TEST_CASE("Generator next", "[generator]") {
	auto gen = range(1, 4);
	auto y1 = gen.next();
	REQUIRE(!y1.done);
	REQUIRE(y1.value == 1);
	auto y2 = gen.next();
	REQUIRE(!y2.done);
	REQUIRE(y2.value == 2);
	auto y3 = gen.next();
	REQUIRE(!y3.done);
	REQUIRE(y3.value == 3);
	auto y4 = gen.next();
	REQUIRE(y4.done);
}

TEST_CASE("Generator collect", "[generator]") {
	auto gen = range(1, 6);
	auto arr = gen.collect();
	REQUIRE(arr.length() == 5);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[4] == 5);
}

TEST_CASE("Generator consume", "[generator]") {
	auto gen = range(1, 11);
	auto count = gen.consume();
	REQUIRE(count == 10);
}

TEST_CASE("Generator forEach", "[generator]") {
	array<int> result;
	// Make sure to call collect() to actually run the generator!
	range(1, 6).forEach([&result](int i) { result.push(i * 2); }).collect();
	REQUIRE(result.length() == 5);
	REQUIRE(result[0] == 2);
	REQUIRE(result[4] == 10);
}

TEST_CASE("Generator zip", "[generator]") {
	auto gen1 = range(1, 4);
	auto gen2 = range(10, 13);
	auto zipped = gen1.zip(gen2);
	auto arr = zipped.collect();
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0].first == 1);
	REQUIRE(arr[0].second == 10);
	REQUIRE(arr[2].first == 3);
	REQUIRE(arr[2].second == 12);
}

TEST_CASE("Generator enumerate", "[generator]") {
	auto gen = range(1, 4);
	auto enumerated = gen.enumerate();
	auto arr = enumerated.collect();
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0].first == 0);
	REQUIRE(arr[0].second == 1);
	REQUIRE(arr[2].first == 2);
	REQUIRE(arr[2].second == 3);
}

TEST_CASE("Generator diff", "[generator]") {
	auto gen1 = generatorFrom({1, 2, 3, 4, 5});
	auto gen2 = generatorFrom({2, 3, 6});
	auto diff_gen = gen1.diff(gen2);
	auto arr = diff_gen.collect();
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 4);
	REQUIRE(arr[2] == 5);
}

TEST_CASE("Generator peek", "[generator]") {
	auto gen = range(1, 4);
	auto peeked = gen.peek();
	auto arr = peeked.collect();
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0].first == 1);
	REQUIRE(!arr[0].second.done);
	REQUIRE(arr[0].second.value == 2);
	REQUIRE(arr[2].first == 3);
	REQUIRE(arr[2].second.done);
}

TEST_CASE("Generator from array lvalue", "[generator]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto gen = generatorFrom(arr);
	auto collected = gen.collect();
	REQUIRE(collected.length() == 5);
	REQUIRE(collected[0] == 1);
	REQUIRE(collected[4] == 5);
}

TEST_CASE("Generator from array rvalue", "[generator]") {
	auto gen = generatorFrom(array<int>{1, 2, 3});
	auto collected = gen.collect();
	REQUIRE(collected.length() == 3);
	REQUIRE(collected[0] == 1);
	REQUIRE(collected[2] == 3);
}

TEST_CASE("Generator empty", "[generator]") {
	auto gen = range(0);
	REQUIRE(gen.count() == 0);
	auto arr = gen.collect();
	REQUIRE(arr.length() == 0);
}

TEST_CASE("Generator single item", "[generator]") {
	auto gen = generatorFrom({42});
	auto y = gen.next();
	REQUIRE(!y.done);
	REQUIRE(y.value == 42);
	y = gen.next();
	REQUIRE(y.done);
}
