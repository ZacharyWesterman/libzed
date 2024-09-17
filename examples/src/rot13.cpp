// Demonstrate the common "ROT13" version of the Caesar cipher.

#include <iostream>
#include <z/all.hpp>

int main(int argc, char **argv) {
	zstring from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	zstring to = "nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM";
	for (int i = 1; i < argc; ++i) {
		// The easiest way is to use a set of characters to perform the cipher.
		(zstring(argv[i]) + " ").cipher(from, to).write(std::cout);

		// Alternatively, you can pass a function in to perform the cipher.
		// This is almost guaranteed to be faster.
		/*
		(zstring(argv[i]) + " ").cipher([](auto chr) {
			if (z::core::isLowerAlpha(chr)) {
				return ((chr - 'a') + 13) % 26 + 'a';
			} else if (z::core::isUpperAlpha(chr)) {
				return ((chr - 'A') + 13) % 26 + 'A';
			}

			return chr;
		}).write(std::cout);
		*/
	}
	zstring().writeln(std::cout);
}
