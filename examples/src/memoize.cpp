// This is an example of using the z::core::memoize() function to cache the results of a function.
// This is useful for functions that are expensive to calculate, and are called with the same arguments multiple times.

#include <iostream>
#include <z/all.hpp>

using z::core::memoize;

// This is the original function that we want to memoize. Calling this directly will recalculate the value each time.
int add(int a, int b) noexcept {
	std::cout << "Calculating value, this should only happen once." << std::endl;
	z::system::pause(1000);
	return a + b;
}

int main() {
	// Create a memoized version of the function.
	memoize<int(int, int)> sum = add;

	// Call the function multiple times with the same arguments, and see that the value is only calculated once.
	// If either argument is changed, the value will be recalculated.
	for (int i = 0; i < 10; ++i) {
		std::cout << sum(10, 20) << std::endl;
	}

	return 0;
}