#include "../../z/all.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::sizable;

class test_class : public sizable {
public:
	size_t size() const noexcept override {
		return 123;
	}
};

TEST_CASE("Sizable interface tests", "[sizable]") {
	test_class test;
	size_t bytes;

	REQUIRE(test.size() == 123);

	z::core::size(test, bytes);
	REQUIRE(bytes == 123);

	int value;
	z::core::size(value, bytes);
	REQUIRE(bytes == sizeof(value));
}
