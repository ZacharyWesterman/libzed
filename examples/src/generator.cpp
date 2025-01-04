// Unlike arrays, generators are not stored in memory. They are evaluated on the fly.
// This makes them significantly more memory-efficient for large ranges of data.
// Both arrays and generators can be iterated over, and have similar functions like map, filter, and reduce.
// However, generators do not have all the functions that arrays do;
// They are meant to be iterated over once, and then discarded.

#include <iostream>
#include <z/core.hpp>

using z::core::join;
using z::core::range;

int main() {
	// Compare summing 10 numbers with an array and a generator.
	// See how the syntax is almost identical, however, for very large ranges,
	// the generator will often be faster because it does not require bulk allocation.
	// For the same reason, it will also only use a small, constant amount of memory.

	// Array
	{
		auto numbers = z::core::array<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		auto total_sum = numbers.reduce(0, [](auto a, auto b) { return a + b; });
		("Sum of all numbers = "_zs + total_sum).writeln(std::cout);
	}

	// Generator
	{
		auto numbers = range(10);
		auto total_sum = numbers.reduce(0, [](auto a, auto b) { return a + b; });
		("Sum of all numbers = "_zs + total_sum).writeln(std::cout);
	}

	// Generators do not have a length, but you can convert them to an array to get one.
	// This is not recommended for large ranges, as it will allocate memory for all the elements.
	// For small ranges, it is fine.
	{
		auto numbers = range(10);
		auto numbers_array = numbers.collect();
		("Length of numbers_array = "_zs + numbers_array.length()).writeln(std::cout);

		// You can also get the count of items in a generator.
		// This will consume the generator, so it will not be able to be used again.
		auto count = range(10).count();
		("Count of number gen = "_zs + count).writeln(std::cout);
	}

	// One of the special features of generators is that they can have arbitrary functions bound to them.
	// These functions will run each time an item is generated.
	// This is useful for logging, debugging, or other side effects.
	{
		auto numbers = range(10).forEach([](auto item) { ("Generated item: "_zs + item).writeln(std::cout); });

		// This will print each item as it is generated.
		for (auto i : numbers) {
			// Do nothing
		}
	}

	// Generators can also be limited to a certain number of items.
	// This is useful for taking a subset of a generator.
	{
		auto numbers = range(100).skip(10).limit(5);
		auto numbers_array = numbers.collect();
		("Contents of array from gen = ["_zs + join(numbers_array, ',') + ']').writeln(std::cout);
	}

	// You can also specifically take only a certain number of items from a generator.
	// Unlike limit, this does not constrain the generator, and it can continue to generate items after the take.
	{
		auto numbers = range(10);
		auto array1 = numbers.take(5);
		auto array2 = numbers.collect();
		("Taken items = ["_zs + join(array1, ',') + ']').writeln(std::cout);
		("Remaining items = ["_zs + join(array2, ',') + ']').writeln(std::cout);
	}
}