// This example shows off generators' ability to un-chunk chunked data.

/**
 * TODO: accept un-chunking these types:
 * - std::function<iterable<T>()> // calling the functions spits out an array, generate items from it, then when ran out, call the function again until it returns an empty list.
 * - generator<iterable<T>, generator> // Just flatten it out again.
 */

#include <iostream>
#include <z/all.hpp>

using z::core::join;
using z::core::range;

int main() {
	for (auto chunk : range(10).chunk(5)) {
		std::cout << '[' << join(chunk, ",") << ']' << std::endl;
	}

	auto gen = range(10).chunk(5);
	for (auto item : gen.unchunk()) {
		std::cout << item << std::endl;
	}
}
