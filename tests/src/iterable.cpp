#include "../../z/all.hpp"
#include "catch/catch_amalgamated.hpp"

#include <exception>

using z::core::iterable;

class test_iterable : public iterable<const int *> {
private:
	int values[3] = {1, 2, 3};

public:
	const int *begin() const noexcept override {
		return values;
	}

	const int *end() const noexcept override {
		return values + 3;
	}
};

TEST_CASE("Iterable interface tests", "[iterable]") {
	test_iterable test;

	REQUIRE(*test.begin() == 1);
	REQUIRE(test.end() - test.begin() == 3);

	int sum = 0;
	for (int value : test) {
		sum += value;
	}
	REQUIRE(sum == 6);
}
