#include "../../z/core/hash32.hpp"
#include "../../z/core/string.hpp"
#include "catch/catch_amalgamated.hpp"

TEST_CASE("Compile-time CRC32 hash", "[hash32]") {
	constexpr auto crc = HASH32("Hello, World!");
	// Static assertion to ensure the hash matches the expected value at compile time.
	static_assert(crc == 0xec4ac3d0, "CRC32 hash does not match expected value");
	REQUIRE(crc == 0xec4ac3d0);

	// Make sure the hash matches the runtime calculation
	const zstring str = "Hello, World!";
	REQUIRE(str.hash() == crc);

	// Test with a different string
	constexpr auto long_crc = HASH32("This is a longer string to test the CRC32 calculation.");
	static_assert(long_crc == 0x3e88d69e, "CRC32 hash of the longer string does not match expected value");
	REQUIRE(long_crc == 0x3e88d69e);
}

TEST_CASE("CRC32 empty and lower-case/case checks", "[hash32]") {
	constexpr auto empty_crc = HASH32("");
	REQUIRE(empty_crc == 0x00000000u);

	constexpr auto crc1 = HASH32("abc");
	constexpr auto crc2 = HASH32("ABC");
	REQUIRE(crc1 != crc2);

	std::string s = "abc";
	REQUIRE(z::core::crc32(s.c_str(), int(s.size())) == crc1);
}

TEST_CASE("CRC32 unicode and collision sanity", "[hash32]") {
	zstring str = u8"café";
	REQUIRE(str.hash() == HASH32(u8"café"));
	REQUIRE(HASH32("hello") != HASH32("hello!"));
}

TEST_CASE("CRC32 deterministic", "[hash32]") {
	for (int i = 0; i < 8; ++i) {
		REQUIRE(HASH32("repeat") == HASH32("repeat"));
	}
}
