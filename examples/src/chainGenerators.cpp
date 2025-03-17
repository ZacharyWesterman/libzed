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

	for (auto &&i : chain(gen3, gen4)) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	return 0;
}
