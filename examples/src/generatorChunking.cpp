// This example shows off generators' ability to un-chunk chunked data.

#include <iostream>
#include <z/all.hpp>

using z::core::generatorFrom;
using z::core::join;
using z::core::range;

int main() {
	std::cout << "Chunking a generator:" << std::endl;
	for (auto chunk : range(10).chunk(5)) {
		std::cout << '[' << join(chunk, ",") << ']' << std::endl;
	}

	std::cout << "Unchunking from a chunked generator:" << std::endl;
	auto gen = range(10).chunk(5);
	for (auto item : gen.unchunk()) {
		std::cout << item << ", ";
	}
	std::cout << std::endl;

	// Technically, the generator doesn't need to have been chunked
	// in order to be unchunked; it just needs to yield objects with an iterator.
	// In this example, the yield from notChunked would be pairs of numbers,
	// but we can call `unchunk()` to just yield individual numbers from the list.
	std::vector<std::vector<int>> listOfLists{{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}};
	auto notChunked = generatorFrom(listOfLists);

	std::cout << "Unchunking from an arbitrary iterator:" << std::endl;
	for (auto item : notChunked.unchunk()) {
		std::cout << item << ", ";
	}
	std::cout << std::endl;
}
