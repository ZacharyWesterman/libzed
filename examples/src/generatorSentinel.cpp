// This example shows usage of the range generator with a custom sentinel.

#include <iostream>
#include <z/all.hpp>

using z::core::range;
using z::core::sentinel;

int main() {
	std::cout << "Generating even numbers until they're divisible by 20" << std::endl;

	// Generate numbers until the next number would be divisible by 20 (ignoring zero).
	sentinel untilDivisibleBy20([](long value, long stepSize) -> bool { return value && value % 20 == 0; });

	auto even = range(0, untilDivisibleBy20, 2);
	for (auto i : even) {
		std::cout << i << std::endl;
	}
}
