#include "catch/catch_amalgamated.hpp"

#include "../../z/all.hpp"

struct OrderedValue {
	int value;

	bool operator==(const OrderedValue &other) const noexcept {
		return value == other.value;
	}

	bool operator<(const OrderedValue &other) const noexcept {
		return value < other.value;
	}

	bool operator>(const OrderedValue &other) const noexcept {
		return value > other.value;
	}
};

struct RawValue {
	std::uint8_t first;
	std::uint8_t second;
};

TEST_CASE("Compare values with native operators", "[compare]") {
	OrderedValue a{1};
	OrderedValue b{2};
	OrderedValue c{1};

	REQUIRE(z::core::equals(a, a) == true);
	REQUIRE(z::core::equals(a, b) == false);
	REQUIRE(z::core::equals(a, c) == true);

	REQUIRE(z::core::greater(b, a) == true);
	REQUIRE(z::core::greater(a, b) == false);
	REQUIRE(z::core::greater(a, c) == false);

	REQUIRE(z::core::lesser(a, b) == true);
	REQUIRE(z::core::lesser(b, a) == false);
	REQUIRE(z::core::lesser(a, c) == false);
}

TEST_CASE("Compare raw POD values via memcmp fallback", "[compare]") {
	RawValue a{1, 0};
	RawValue b{2, 0};
	RawValue c{1, 0};

	REQUIRE(z::core::equals(a, a) == true);
	REQUIRE(z::core::equals(a, b) == false);
	REQUIRE(z::core::equals(a, c) == true);

	REQUIRE(z::core::greater(b, a) == true);
	REQUIRE(z::core::greater(a, b) == false);
	REQUIRE(z::core::greater(a, c) == false);

	REQUIRE(z::core::lesser(a, b) == true);
	REQUIRE(z::core::lesser(b, a) == false);
	REQUIRE(z::core::lesser(a, c) == false);
}
