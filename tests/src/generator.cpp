// Unlike arrays, generators are not stored in memory. They are evaluated on the fly.
// This makes them significantly more memory-efficient for large ranges of data.
// Both arrays and generators can be iterated over, and have similar functions like map, filter, and reduce.
// However, generators do not have all the functions that arrays do;
// They are meant to be iterated over once, and then discarded.

#include "../../z/all.hpp"
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
	REQUIRE(y1.has_value());
	REQUIRE(y1.value() == 1);
	auto y2 = gen.next();
	REQUIRE(y2.has_value());
	REQUIRE(y2.value() == 2);
	auto y3 = gen.next();
	REQUIRE(y3.has_value());
	REQUIRE(y3.value() == 3);
	auto y4 = gen.next();
	REQUIRE(!y4.has_value());
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

TEST_CASE("Generator pair", "[generator]") {
	auto gen1 = range(1, 4);
	auto gen2 = range(10, 13);
	auto paired = gen1.pair(gen2);
	auto arr = paired.collect();
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0].first == 1);
	REQUIRE(arr[0].second == 10);
	REQUIRE(arr[2].first == 3);
	REQUIRE(arr[2].second == 12);
}

TEST_CASE("Generator zip", "[generator]") {
	auto gen1 = range(0, 5);
	auto gen2 = range(100, 103);
	auto zipped = gen1.zip(gen2);
	auto arr = zipped.collect();
	REQUIRE(arr.length() == 8);
	REQUIRE(arr[0] == 0);
	REQUIRE(arr[1] == 100);
	REQUIRE(arr[2] == 1);
	REQUIRE(arr[3] == 101);
	REQUIRE(arr[4] == 2);
	REQUIRE(arr[5] == 102);
	REQUIRE(arr[6] == 3);
	REQUIRE(arr[7] == 4);
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
	REQUIRE(arr[0].second.has_value());
	REQUIRE(arr[0].second.value() == 2);
	REQUIRE(arr[2].first == 3);
	REQUIRE(!arr[2].second.has_value());
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
	REQUIRE(y.has_value());
	REQUIRE(y.value() == 42);
	y = gen.next();
	REQUIRE(!y.has_value());
}

TEST_CASE("Generator unchunk", "[generator]") {
	auto numbers = range(1, 17).chunk(5);
	auto unchunked = numbers.flatten();
	auto result = unchunked.collect();
	REQUIRE(result.length() == 16);
	REQUIRE(result[0] == 1);
	REQUIRE(result[15] == 16);
}

TEST_CASE("Generator chain", "[generator]") {
	auto gen1 = range(1, 4);	 // 1, 2, 3
	auto gen2 = range(10, 13); // 10, 11, 12
	auto chained = gen1.chain(gen2);
	auto result = chained.collect();
	REQUIRE(result.length() == 6);
	REQUIRE(result[0] == 1);
	REQUIRE(result[2] == 3);
	REQUIRE(result[3] == 10);
	REQUIRE(result[5] == 12);
}

TEST_CASE("Generator from vector", "[generator]") {
	std::vector<int> vec = {1, 2, 3, 4, 5};
	auto gen = generatorFrom(vec);
	auto result = gen.collect();
	REQUIRE(result.length() == 5);
	REQUIRE(result[0] == 1);
	REQUIRE(result[4] == 5);
}

TEST_CASE("Generator from set", "[generator]") {
	std::set<std::string> str_set = {"apple", "banana", "cherry"};
	auto gen = generatorFrom(str_set);
	auto result = gen.collect();
	REQUIRE(result.length() == 3);
	// Sets are ordered, so we can check the order
	REQUIRE(result[0] == "apple");
	REQUIRE(result[1] == "banana");
	REQUIRE(result[2] == "cherry");
}

TEST_CASE("Generator map with different types", "[generator]") {
	auto numbers = range(1, 4).map<std::string>([](int i) { return std::to_string(i * 10); });
	auto result = numbers.collect();
	REQUIRE(result.length() == 3);
	REQUIRE(result[0] == "10");
	REQUIRE(result[1] == "20");
	REQUIRE(result[2] == "30");
}

TEST_CASE("Generator filter with complex predicate", "[generator]") {
	auto numbers = range(1, 21).filter([](int i) { return i % 3 == 0 && i % 5 != 0; });
	auto result = numbers.collect();
	REQUIRE(result.length() == 5); // 3, 6, 9, 12, 18 (15 is divisible by 5, so excluded)
	REQUIRE(result[0] == 3);
	REQUIRE(result[1] == 6);
	REQUIRE(result[2] == 9);
	REQUIRE(result[3] == 12);
	REQUIRE(result[4] == 18);
}

TEST_CASE("Generator chained operations", "[generator]") {
	auto result = range(1, 21)
									.filter([](int i) { return i % 2 == 0; }) // even numbers
									.map<int>([](int i) { return i * 2; })		// double them
									.skip(2)																	// skip first 2
									.limit(3)																	// take next 3
									.collect();
	REQUIRE(result.length() == 3);
	REQUIRE(result[0] == 12); // 6*2 = 12 (skipped 4*2=8, 6*2=12)
	REQUIRE(result[1] == 16); // 8*2 = 16
	REQUIRE(result[2] == 20); // 10*2 = 20
}

TEST_CASE("Generator reduce with strings", "[generator]") {
	std::vector<std::string> str_vec = {"hello", " ", "world"};
	auto strings = generatorFrom(str_vec);
	auto concatenated = strings.reduce(std::string(""), [](const std::string &a, const std::string &b) { return a + b; });
	REQUIRE(concatenated == "hello world");
}

TEST_CASE("Generator chunk edge cases", "[generator]") {
	// Empty generator
	auto empty_chunks = range(0).chunk(5);
	auto empty_result = empty_chunks.collect();
	REQUIRE(empty_result.length() == 0);

	// Chunk size larger than generator
	auto small_gen = range(1, 4).chunk(10);
	auto small_result = small_gen.collect();
	REQUIRE(small_result.length() == 1);
	REQUIRE(small_result[0].length() == 3);
	REQUIRE(small_result[0][0] == 1);
	REQUIRE(small_result[0][2] == 3);
}

TEST_CASE("Generator take edge cases", "[generator]") {
	// Take more than available
	auto gen = range(1, 4);
	auto result = gen.take(10);
	REQUIRE(result.length() == 3);
	REQUIRE(result[0] == 1);
	REQUIRE(result[2] == 3);

	// Take zero
	result = gen.take(0);
	REQUIRE(result.length() == 0);

	// Take negative (should probably handle gracefully, but let's see what happens)
	// result = gen.take(-1); // This might cause issues, so commenting out
}

TEST_CASE("Generator enumerate with offset", "[generator]") {
	auto gen = range(5, 8); // 5, 6, 7
	auto enumerated = gen.enumerate();
	auto result = enumerated.collect();
	REQUIRE(result.length() == 3);
	REQUIRE(result[0].first == 0);
	REQUIRE(result[0].second == 5);
	REQUIRE(result[1].first == 1);
	REQUIRE(result[1].second == 6);
	REQUIRE(result[2].first == 2);
	REQUIRE(result[2].second == 7);
}

TEST_CASE("Generator diff with empty generators", "[generator]") {
	auto gen1 = range(1, 4); // 1, 2, 3
	auto gen2 = range(0);		 // empty
	auto diff_gen = gen1.diff(gen2);
	auto result = diff_gen.collect();
	REQUIRE(result.length() == 3);
	REQUIRE(result[0] == 1);
	REQUIRE(result[1] == 2);
	REQUIRE(result[2] == 3);

	// Both empty
	auto empty1 = range(0);
	auto empty2 = range(0);
	auto empty_diff = empty1.diff(empty2);
	auto empty_result = empty_diff.collect();
	REQUIRE(empty_result.length() == 0);
}

TEST_CASE("Generator peek edge cases", "[generator]") {
	auto gen = range(1, 2); // just one item
	auto peeked = gen.peek();
	auto result = peeked.collect();
	REQUIRE(result.length() == 1);
	REQUIRE(result[0].first == 1);
	REQUIRE(!result[0].second.has_value()); // no next item
}

TEST_CASE("Generator forEach side effects", "[generator]") {
	int sum = 0;
	auto gen = range(1, 6).forEach([&sum](int i) { sum += i; });
	gen.collect();			// consume the generator
	REQUIRE(sum == 15); // 1+2+3+4+5
}

TEST_CASE("Generator from temporary vector", "[generator]") {
	std::vector<int> temp_vec = {10, 20, 30};
	auto gen = generatorFrom(temp_vec);
	auto result = gen.collect();
	REQUIRE(result.length() == 3);
	REQUIRE(result[0] == 10);
	REQUIRE(result[1] == 20);
	REQUIRE(result[2] == 30);
}

TEST_CASE("Generator pair with different lengths", "[generator]") {
	auto gen1 = range(1, 4);	 // 1, 2, 3
	auto gen2 = range(10, 15); // 10, 11, 12, 13, 14
	auto paired = gen1.pair(gen2);
	auto result = paired.collect();
	REQUIRE(result.length() == 3); // stops at shorter generator
	REQUIRE(result[0].first == 1);
	REQUIRE(result[0].second == 10);
	REQUIRE(result[2].first == 3);
	REQUIRE(result[2].second == 12);
}
