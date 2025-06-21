#include <iostream>
#include <z/core/hash32.hpp>
#include <z/core/string.hpp>

int main() {
	const zstring str = "Hello, World!";
	const zstring str2 = "Goodbye, World!";

	std::cout << "String 1: " << str.cstring() << std::endl;
	std::cout << "CRC32 Hash 1: " << std::hex << str.hash() << std::dec << std::endl;
	std::cout << "Compare to constexpr: " << std::hex << HASH32("Hello, World!") << std::dec << std::endl;

	std::cout << "String 2: " << str2.cstring() << std::endl;
	std::cout << "CRC32 Hash 2: " << std::hex << str2.hash() << std::dec << std::endl;
	std::cout << "Compare to constexpr: " << std::hex << HASH32("Goodbye, World!") << std::dec << std::endl;
}
