// This is an example of using the z::core::memoize() function to cache the results of a function.
// This is useful for functions that are expensive to calculate, and are called with the same arguments multiple times.

#include <iostream>
#include <z/core/memoize.hpp>
#include <z/system/sleep.hpp>

using z::core::memoize;

// This is the original function that we want to memoize.
// Calling this directly will recalculate the value each time,
// which in this case is expensive because it sleeps for 1 second.
int add(int a, int b) noexcept {
	std::cout << "Calculating value, this should only happen once." << std::endl;
	z::system::sleep(1000);
	return a + b;
}

int main() {
	// Create a memoized version of the function.
	memoize<int(int, int)> sum = add;

	// Call the function multiple times with the same arguments, and see that the value is only calculated once.
	// If either argument is changed, the value will be recalculated.
	for (int i = 0; i < 10; ++i) {
		std::cout << sum(10, 20) << ' ' << std::flush;
	}
	std::cout << std::endl;

	// Another good example is to use a lambda to implement a recursive function.
	memoize<int(int)> fib([&fib](int n) -> int {
		if (n <= 1) {
			return n;
		}
		return fib(n - 1) + fib(n - 2);
	});

	// See how, since the memoization is applied, this naive Fibonacci implementation
	// runs in linear time instead of exponential time.
	// Each value only has to be calculated once, and then it is cached.
	std::cout << "Fibonacci(10): " << fib(10) << std::endl;
	std::cout << "Fibonacci(40): " << fib(40) << std::endl;

	return 0;
}
