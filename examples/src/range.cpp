// This is an example of using the z::core::range() generator to generate a list of numbers.
// Ranges are a memory-efficient way of creating and looping over a large range.
// z::core::range(begin = 0, end, step = 1)

#include <iostream>
#include <z/core.hpp>
#include <z/system/sleep.hpp>
#include <z/util/progress.hpp>

using z::core::join;
using z::core::range;
using z::system::sleep;

int main() {
	// You can iterate over ranges just like arrays.
	"short range:"_zs.writeln(std::cout);
	for (auto i : range(6)) {
		(">"_zs + i).writeln(std::cout);
	}

	// To convert to an array, use the collect() method.
	auto list = range(15, 100, 5).collect();
	("\narray: "_zs + join(list, ',')).writeln(std::cout);

	// Unlike arrays, you can create VERY long ranges with no memory impact.
	// An array of 1 trillion elements would take a long time to allocate, but the range is instant.
	"\nlong range:"_zs.writeln(std::cout);
	for (auto i : range(1'000'000'000)) {
		(">"_zs + i).writeln(std::cout);
		if (i > 3) {
			"Exiting early!"_zs.writeln(std::cout);
			break;
		}
	}

	// Skipping elements
	// Notice how the range is still instant, even though we're skipping 500,000 elements.
	"\nskipping some elements and limiting to 5 total:"_zs.writeln(std::cout);
	for (auto i : (range(1'000'000).forEach([](long) { sleep(100); })).skip(500'000).limit(5)) {
		(">"_zs + i).writeln(std::cout);
	}

	// Ranges can even be infinite!
	// This will loop forever, so best not to use it with functions that consume all values (e.g. `count()`).
	"\ninfinite range (only first 5):"_zs.writeln(std::cout);
	for (auto i : range(0, z::core::infinity).limit(5)) {
		(">"_zs + i).writeln(std::cout);
	}

	// A fun feature of generators is that they can have extra functionality bound to them.
	// Here, we are taking a long range, and binding a progress bar be updated while we iterate through it.
	z::util::progress progress;
	long max = 10'000'000;
	auto numbers = range(max).forEach([max, &progress](auto i) { progress.set(std::cout, i, max); });

	// See how here, we're not directly calling progress.set, but the progress bar displays!
	"\nSum a long range and display the progress:"_zs.writeln(std::cout);
	auto sum = numbers.reduce(0, std::plus<long>());
	("Total: "_zs + sum).writeln(std::cout);

	return 0;
}