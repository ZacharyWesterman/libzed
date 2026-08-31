// This example demonstrates how to use operators to chain, map, filter and reduce generators.

#include <algorithm>
#include <iostream>
#include <z/core.hpp>

using z::core::infinity;
using z::core::range;

auto factorial(long n) -> long {
	long result = 1;
	for (long i = 2; i <= n; i++) {
		result *= i;
	}
	return result;
}

int main() {
	const std::function<long(long)> plus2 = [](auto i) { return i + 2; };
	const std::function<char(long)> tochar = [](auto i) { return '*'; };

	auto is_even = [](long a) { return a % 2 == 0; };
	auto is_below_10 = [](long a) { return a < 10; };

	auto multiply = [](long a, long b) { return a * b; };

	// Instead of calling `range(0,5).chain(range(5,10)).chain(range(10,15))`,
	// you can just put a plus `+` between the generators.
	// Like with .chain(), the generators must yield the same type.
	auto chained = range(0, 5) + range(5, 10) + range(10, 15);

	// Instead of calling
	// `range(5).map(plus2).map(factorial).map(tochar)`,
	// you can just put a vertical bar `|` between map functions.
	// NOTE that these must either be a plain function, or a `std::function`;
	// you cannot pass a lambda directly to the pipeline.
	auto mapped_generator = range(5) | plus2 | factorial | tochar;

	// Instead of calling `range(50).filter(is_even).filter(is_below_10)`,
	// you can just put an ampersand `&&` or `and` between filter functions.
	// Unlike `|`, these can be lambdas, functions or `std::function`s.
	auto evens = range(50) && is_even && is_below_10;

	// Instead of calling `range(2,6).reduce({}, multiply)`,
	// you can put `>>` in between the generator and the reduction function.
	// NOTE that this uses the *default constructor* for the generator return type,
	// if the generator doesn't yield any items (for integers that's `0`!).
	// So if you need something like `range(2,6).reduce(-1, multiply)`,
	// you cannot use this operator.
	auto factorial = range(2, 6) >> multiply;

	// Instead of calling `range(0, infinity).until(50)`,
	// you can use `!=` to stop a generator once either a value (50 in this case) is found,
	// or a predicate function returns true.
	auto limited_generator = range(0, infinity) != 50;
}
