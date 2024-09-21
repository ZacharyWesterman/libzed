#include <iostream>
#include <z/core.hpp>

using z::core::array;
using z::core::join;

int main() {
	array<zstring> strings = {"123", "abc", "11", "-400"};

	// Convert all the string values to integers.
	// Note that non-numeric strings all go to zero.
	auto numbers = strings.map<int>([](auto item) { return item.integer(); });

	// Sum all the numbers
	auto total_sum = numbers.reduce(0, [](auto a, auto b) { return a + b; });

	// Sum only the non-negative numbers
	auto nonneg_sum = numbers.filter([](auto item) { return item >= 0; }).reduce(0, [](auto a, auto b) { return a + b; });

	// Print output
	("Number list = ["_zs + join(numbers, ", ") + "]\n" + "Sum of all numbers = " + total_sum + "\n" + "Sum of non-negative numbers = " + nonneg_sum + "\n").write(std::cout);
}