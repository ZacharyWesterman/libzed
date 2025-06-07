// This example demonstrates more advanced generator usage.
// In particular the z::core::generatorFrom() function, which allows you to create a generator from any iterable object.
// This includes arrays, strings, and anything else that C++ can treat as an iterable.

#include <iostream>
#include <z/all.hpp>

using z::core::array;
using z::core::generatorFrom;
using z::core::range;
using z::core::string;
using z::util::chain;

int main() {
	// Print the numbers from 0 to 9
	auto gen1 = range(0, 5);
	auto gen2 = range(5, 10);

	std::cout << "Numbers from 0 to 9:" << std::endl;
	for (auto &i : chain(gen1, gen2)) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	// Iterate over elements in two arrays.
	// Note how the generators do NOT reallocate memory,
	// but still allow iterating over the elements as if they were in a single array.
	array<zstring> arr1 = {"a", "b", "c", "d", "e"};
	array<zstring> arr2 = {"f", "g", "h", "i", "j"};

	auto gen3 = generatorFrom(arr1);
	auto gen4 = generatorFrom(arr2);

	std::cout << "\nLetters from a to j:" << std::endl;
	for (auto &&i : chain(gen3, gen4)) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	// You can also zip generators together to create pairs of items.
	std::cout << "\nZipped pairs:" << std::endl;

	auto gen5 = range(1, 6);
	auto gen6 = generatorFrom({"one", "two", "three", "four", "five"});
	auto zipped = gen5.zip(gen6);
	for (auto [first, second] : zipped) {
		std::cout << '(' << first << ", " << second << ") ";
	}
	std::cout << std::endl;

	// Of course, you can also use generators with strings.
	std::cout << "\nLetters in a string:" << std::endl;
	auto gen7 = generatorFrom(zstring("Hello, World!"));
	for (char chr : gen7) {
		std::cout << chr << ' ';
	}
	std::cout << std::endl;

	// Technically, you can implement your own enumerate() function,
	// but there is a built-in one that does this for you.
	auto gen8 = generatorFrom({"a", "b", "c", "d", "e"}).enumerate();
	std::cout << "\nEnumerated range:" << std::endl;
	for (auto [index, value] : gen8) {
		std::cout << '(' << index << ": " << value << ") ";
	}
	std::cout << std::endl;

	// Generators can also be used to grab distinct items from two iterables.
	auto gen9 = generatorFrom({"apple", "banana", "cherry", "melon"});
	auto gen10 = generatorFrom({"banana", "cherry", "date", "fig"});
	std::cout << "\nItems in the first generator but not the second:" << std::endl;
	for (auto item : gen9.diff(gen10)) {
		std::cout << item << ' ';
	}
	std::cout << std::endl;

	return 0;
}
