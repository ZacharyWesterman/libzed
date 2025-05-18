#include "../../z/core/array.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::array;

TEST_CASE("Create an empty array", "[array]") {
	array<int> arr;
	REQUIRE(arr.length() == 0);
}

TEST_CASE("Select a random element from an array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};

	auto randomElement = arr.randomElement();
	REQUIRE(randomElement >= 1);
	REQUIRE(randomElement <= 5);
}

TEST_CASE("Select a random element from an empty array", "[array]") {
	array<int> arr;
	REQUIRE_THROWS_AS(arr.randomElement(), std::out_of_range);
}

TEST_CASE("Select a random element from an array with one element", "[array]") {
	array<int> arr = {42};
	REQUIRE(arr.randomElement() == 42);
}

TEST_CASE("Select N random elements from an array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};

	auto randomElements = arr.randomElements(3);
	REQUIRE(randomElements.length() == 3);
	for (const auto &elem : randomElements) {
		REQUIRE(elem >= 1);
		REQUIRE(elem <= 5);
	}
}

TEST_CASE("Select N random elements from an empty array", "[array]") {
	array<int> arr;
	auto randomElements = arr.randomElements(3);
	REQUIRE(randomElements.length() == 0);
}

TEST_CASE("Select N random elements from an array with fewer elements than N", "[array]") {
	array<int> arr = {1, 2, 3};

	auto randomElements = arr.randomElements(5);
	REQUIRE(randomElements.length() == 3);
	for (const auto &elem : randomElements) {
		REQUIRE(elem >= 1);
		REQUIRE(elem <= 3);
	}
}

TEST_CASE("Sum of an array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	int sum = arr.reduce(0, [](const int &a, const int &b) { return a + b; });
	REQUIRE(sum == 15);
}

TEST_CASE("Sum of an empty array", "[array]") {
	array<int> arr;
	int sum = arr.reduce(0, [](const int &a, const int &b) { return a + b; });
	REQUIRE(sum == 0);
}

TEST_CASE("Filter an array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto filtered = arr.filter([](const int &a) { return a % 2 == 0; });
	REQUIRE(filtered.length() == 2);
	REQUIRE(filtered[0] == 2);
	REQUIRE(filtered[1] == 4);
}

TEST_CASE("Map an array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto mapped = arr.map<int>([](const int &a) { return a * 2; });
	REQUIRE(mapped.length() == 5);
	REQUIRE(mapped[0] == 2);
	REQUIRE(mapped[1] == 4);
	REQUIRE(mapped[2] == 6);
	REQUIRE(mapped[3] == 8);
	REQUIRE(mapped[4] == 10);
}

TEST_CASE("Swap elements in an array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	arr.swap(1, 3);
	REQUIRE(arr[1] == 4);
	REQUIRE(arr[3] == 2);
}

TEST_CASE("Compare arrays", "[array]") {
	array<int> arr1 = {1, 2, 3};
	array<int> arr2 = {1, 2, 3};
	array<int> arr3 = {4, 5, 6};

	REQUIRE(arr1 == arr2);
	REQUIRE_FALSE(arr1 == arr3);
}
