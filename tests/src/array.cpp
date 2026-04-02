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

TEST_CASE("Array equality", "[array]") {
	array<int> arr1 = {1, 2, 3};
	array<int> arr2 = {1, 2, 3};

	REQUIRE(arr1 == arr2);

	array<int> arr3 = {4, 5, 6};
	REQUIRE_FALSE(arr1 == arr3);
}

TEST_CASE("Add elements to array", "[array]") {
	array<int> arr;
	REQUIRE(arr.length() == 0);

	int index = arr.add(5);
	REQUIRE(index == 0);
	REQUIRE(arr.length() == 1);
	REQUIRE(arr[0] == 5);

	arr.add(10);
	REQUIRE(arr.length() == 2);
	REQUIRE(arr[1] == 10);
}

TEST_CASE("Push elements to array", "[array]") {
	array<int> arr;
	arr.push(1);
	arr.push(2);
	REQUIRE(arr.length() == 2);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 2);
}

TEST_CASE("Append elements to array", "[array]") {
	array<int> arr = {1, 2};
	arr.append(3);
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[2] == 3);
}

TEST_CASE("Pop elements from array", "[array]") {
	array<int> arr = {1, 2, 3};
	int popped = arr.pop();
	REQUIRE(popped == 3);
	REQUIRE(arr.length() == 2);
	REQUIRE(arr[1] == 2);
}

TEST_CASE("Pop from empty array", "[array]") {
	array<int> arr;
	REQUIRE_THROWS_AS(arr.pop(), std::out_of_range);
}

TEST_CASE("Insert elements into array", "[array]") {
	array<int> arr = {1, 3};
	arr.insert(2, 1);
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 2);
	REQUIRE(arr[2] == 3);
}

TEST_CASE("Insert at negative index", "[array]") {
	array<int> arr = {1, 3};
	arr.insert(2, -1);
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 3);
	REQUIRE(arr[2] == 2);
}

TEST_CASE("Remove elements from array", "[array]") {
	array<int> arr = {1, 2, 3, 4};
	arr.remove(1);
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 3);
	REQUIRE(arr[2] == 4);
}

TEST_CASE("Remove range from array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	arr.remove(1, 2);
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 4);
	REQUIRE(arr[2] == 5);
}

TEST_CASE("Replace elements in array", "[array]") {
	array<int> arr = {1, 2, 3, 4};
	arr.replace(1, 2, 99);
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 99);
	REQUIRE(arr[2] == 4);
}

TEST_CASE("Replace with array", "[array]") {
	array<int> arr = {1, 2, 3, 4};
	array<int> replacement = {10, 20};
	arr.replace(1, 2, replacement);
	REQUIRE(arr.length() == 4);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 10);
	REQUIRE(arr[2] == 20);
	REQUIRE(arr[3] == 4);
}

TEST_CASE("Subset of array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto sub = arr.subset(1, 3);
	REQUIRE(sub.length() == 3);
	REQUIRE(sub[0] == 2);
	REQUIRE(sub[1] == 3);
	REQUIRE(sub[2] == 4);
}

TEST_CASE("Subset with negative count", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto sub = arr.subset(3, -2);
	REQUIRE(sub.length() == 2);
	REQUIRE(sub[0] == 3);
	REQUIRE(sub[1] == 4);
}

TEST_CASE("Sort array", "[array]") {
	array<int> arr = {3, 1, 4, 1, 5};
	arr.sort();
	REQUIRE(arr.length() == 5);
	REQUIRE(arr[0] == 5);
	REQUIRE(arr[1] == 4);
	REQUIRE(arr[2] == 3);
	REQUIRE(arr[3] == 1);
	REQUIRE(arr[4] == 1);
}

TEST_CASE("Sort array with lambda", "[array]") {
	array<int> arr = {3, 1, 4, 1, 5};
	arr.sort([](const int &a, const int &b) { return a > b; });
	REQUIRE(arr.length() == 5);
	REQUIRE(arr[0] == 5);
	REQUIRE(arr[1] == 4);
	REQUIRE(arr[2] == 3);
	REQUIRE(arr[3] == 1);
	REQUIRE(arr[4] == 1);
}

TEST_CASE("Sorted array", "[array]") {
	array<int> arr = {3, 1, 4, 1, 5};
	auto sorted_arr = arr.sorted();
	REQUIRE(sorted_arr.length() == 5);
	REQUIRE(sorted_arr[0] == 5);
	REQUIRE(sorted_arr[1] == 4);
	REQUIRE(sorted_arr[2] == 3);
	REQUIRE(sorted_arr[3] == 1);
	REQUIRE(sorted_arr[4] == 1);
	REQUIRE(arr.length() == 5); // Original unchanged
}

TEST_CASE("Shuffle array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	arr.shuffle();
	REQUIRE(arr.length() == 5);
	// Note: Hard to test randomness without determinism
}

TEST_CASE("Shuffled array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto shuffled_arr = arr.shuffled();
	REQUIRE(shuffled_arr.length() == 5);
	REQUIRE(arr.length() == 5); // Original unchanged
}

TEST_CASE("Reverse array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	arr.reverse();
	REQUIRE(arr.length() == 5);
	REQUIRE(arr[0] == 5);
	REQUIRE(arr[1] == 4);
	REQUIRE(arr[2] == 3);
	REQUIRE(arr[3] == 2);
	REQUIRE(arr[4] == 1);
}

TEST_CASE("Reversed array", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	auto reversed_arr = arr.reversed();
	REQUIRE(reversed_arr.length() == 5);
	REQUIRE(reversed_arr[0] == 5);
	REQUIRE(reversed_arr[1] == 4);
	REQUIRE(reversed_arr[2] == 3);
	REQUIRE(reversed_arr[3] == 2);
	REQUIRE(reversed_arr[4] == 1);
	REQUIRE(arr.length() == 5); // Original unchanged
}

TEST_CASE("Array comparison operators", "[array]") {
	array<int> arr1 = {1, 2, 3};
	array<int> arr2 = {1, 2, 3};
	array<int> arr3 = {1, 2, 4};
	array<int> arr4 = {1, 2};
	array<int> arr5 = {1, 2, 3, 4};

	REQUIRE(arr1 == arr2);
	REQUIRE(arr1 != arr3);
	REQUIRE(arr1 < arr3); // 3 < 4 at position 2
	REQUIRE(arr3 > arr1);
	REQUIRE(arr1 <= arr2);
	REQUIRE(arr1 >= arr2);
	REQUIRE(arr4 < arr1); // Shorter length
	REQUIRE(arr5 > arr1); // Longer length
}

TEST_CASE("isValid index", "[array]") {
	array<int> arr = {1, 2, 3};
	REQUIRE(arr.isValid(0));
	REQUIRE(arr.isValid(1));
	REQUIRE(arr.isValid(2));
	REQUIRE_FALSE(arr.isValid(3));
	REQUIRE_FALSE(arr.isValid(-4));
	REQUIRE(arr.isValid(-1)); // Last element
	REQUIRE(arr.isValid(-3)); // First element
}

TEST_CASE("Contains element", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	REQUIRE(arr.contains(3));
	REQUIRE_FALSE(arr.contains(6));
}

TEST_CASE("Find element", "[array]") {
	array<int> arr = {1, 2, 3, 4, 5};
	REQUIRE(arr.find(3) == 2);
	REQUIRE(arr.find(6) == -1);
}

TEST_CASE("At method", "[array]") {
	array<int> arr = {1, 2, 3};
	REQUIRE(arr.at(0) == 1);
	REQUIRE(arr.at(2) == 3);
	REQUIRE_THROWS_AS(arr.at(3), std::out_of_range);
	REQUIRE_THROWS_AS(arr.at(-4), std::out_of_range);
}

TEST_CASE("Operator[]", "[array]") {
	array<int> arr = {1, 2, 3};
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[2] == 3);
	REQUIRE_THROWS_AS(arr[3], std::out_of_range);
}

TEST_CASE("Size method", "[array]") {
	array<int> arr = {1, 2, 3};
	REQUIRE(arr.size() > 0); // Size in bytes, depends on int size
}

TEST_CASE("Clear array", "[array]") {
	array<int> arr = {1, 2, 3};
	arr.clear();
	REQUIRE(arr.length() == 0);
}

TEST_CASE("Increase capacity", "[array]") {
	array<int> arr;
	arr.increase(10);
	REQUIRE(arr.length() == 0); // Length unchanged, capacity increased
	arr.add(1);
	REQUIRE(arr.length() == 1);
}

TEST_CASE("Copy constructor", "[array]") {
	array<int> arr1 = {1, 2, 3};
	array<int> arr2 = arr1;
	REQUIRE(arr2.length() == 3);
	REQUIRE(arr2[0] == 1);
	REQUIRE(arr2[1] == 2);
	REQUIRE(arr2[2] == 3);
	REQUIRE(arr1 == arr2);
}

TEST_CASE("Assignment operator", "[array]") {
	array<int> arr1 = {1, 2, 3};
	array<int> arr2;
	arr2 = arr1;
	REQUIRE(arr2.length() == 3);
	REQUIRE(arr2[0] == 1);
	REQUIRE(arr2[1] == 2);
	REQUIRE(arr2[2] == 3);
	REQUIRE(arr1 == arr2);
}

TEST_CASE("Initializer list assignment", "[array]") {
	array<int> arr;
	arr = {1, 2, 3};
	REQUIRE(arr.length() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 2);
	REQUIRE(arr[2] == 3);
}

TEST_CASE("Range-based for loop", "[array]") {
	array<int> arr = {1, 2, 3};
	int sum = 0;
	for (auto &val : arr) {
		sum += val;
	}
	REQUIRE(sum == 6);
}
