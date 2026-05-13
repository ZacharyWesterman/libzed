// Simulate a network fetch generator, e.g. something that gets
// a paginated list of posts/values/etc, and yields one item at a time.

#include <array>
#include <iostream>
#include <z/all.hpp>

using std::optional;
using z::core::generator;
using z::system::sleep;

int main() {
	std::array<std::array<int, 5>, 3> data;
	data[0] = {0, 1, 2, 3, 4};
	data[1] = {5, 6, 7, 8, 9};
	data[2] = {10, 11, 12, 13, 14};

	// Note how initially we have to use std::array<int> as the data type,
	// because that's what the generator *actually* produces.
	// Afterwards, we can unchunk it.
	auto getNextPage = [&data](int &page) -> optional<std::array<int, 5>> {
		std::cout << "Fetching data... " << std::flush;
		// Simulate a network fetch.
		sleep(500);

		if (page >= data.size()) {
			std::cout << "No more data!" << std::endl;
			return {};
		}

		std::cout << " Done." << std::endl;
		return data[page++];
	};

	// Build the generator and unchunk it.
	// This generator yields one post/value at a time,
	// only fetching the next page when it needs to.
	auto myGenerator = generator<std::array<int, 5>, int>(0, getNextPage).flatten();

	for (auto value : myGenerator) {
		std::cout << value << std::endl;
	}
}
