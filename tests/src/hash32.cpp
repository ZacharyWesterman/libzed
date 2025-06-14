#include "../../z/core/hash32.hpp"
#include "catch/catch_amalgamated.hpp"

TEST_CASE("Compile-time CRC32 hash", "[hash32]") {
	constexpr auto crc = HASH32("Hello, World!");
	// Static assertion to ensure the hash matches the expected value at compile time.
	static_assert(crc == 0xec4ac3d0, "CRC32 hash does not match expected value");
	REQUIRE(crc == 0xec4ac3d0);

	constexpr auto long_crc = HASH32("This is a longer string to test the CRC32 calculation.");
	static_assert(long_crc == 0x3e88d69e, "CRC32 hash of the longer string does not match expected value");
	REQUIRE(long_crc == 0x3e88d69e);
}
