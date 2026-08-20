#include "../../z/all.hpp"
#include "catch/catch_amalgamated.hpp"

using z::core::indexable;

class test_indexable : public indexable<int> {
public:
	int at(int index) const override {
		return index * 2;
	}
};

TEST_CASE("Indexable interface tests", "[indexable]") {
	test_indexable test;

	REQUIRE(test.at(3) == 6);
	REQUIRE(test[4] == 8);
}
