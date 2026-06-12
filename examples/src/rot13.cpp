// Demonstrate the common "ROT13" version of the Caesar cipher.

#include <iostream>
#include <z/all.hpp>

// Caesar cipher function
template <int N>
auto rot(uint32_t chr) -> uint32_t {
	if (z::core::isLowerAlpha(chr)) {
		return ((chr - 'a') + N) % 26 + 'a';
	} else if (z::core::isUpperAlpha(chr)) {
		return ((chr - 'A') + N) % 26 + 'A';
	}

	return chr;
}

// Alias strings that can also perform rot13.
const zstring from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const zstring to = "nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM";

int main(int argc, char **argv) {
	for (int i = 1; i < argc; ++i) {
		auto str = zstring(argv[i]) + " ";

		// The easiest way is to use two sets of characters to perform the cipher.
		// These just map characters from one to the other.
		str.cipher(from, to).write(std::cout);

		// Alternatively, you can pass a function in to perform the cipher.
		// This is almost always faster than mapping with the `to`/`from` strings,
		// And allows for more dynamic ciphers.
		str.cipher(rot<13>).write(std::cout);
	}
	zstring().writeln(std::cout);
}
