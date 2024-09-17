/*
 * A short example to demonstrate stream IO and regex pattern matching.
 */

#include <iostream>
#include <regex>
#include <z/core/string.hpp>

int main() {
	std::regex regex;
	zstring pattern;

	while (!std::cin.eof()) {
		// Get regex pattern to match against, exiting if user hits ctrl-D.
		pattern.readln(std::cin);
		if (std::cin.eof() && !pattern.length()) {
			break;
		}

		// If pattern is invalid, print a short description of the error.
		try {
			regex.assign(pattern.str());
		} catch (const std::regex_error &err) {
			zstring(err.what()).writeln(std::cout);
			continue;
		}

		// Prompt for the string to match to the regex pattern.
		// Note that zstring is equivalent to z::core::string<z::utf32>
		zstring(":").write(std::cout);
		zstring text;
		text.readln(std::cin);

		// Show what substring the regex matched, if anything.
		auto match = std::regex_search(text.str(), regex);
		if (std::regex_search(text.str(), regex)) {
			(text + " matches!").writeln(std::cout);
		} else {
			zstring("does not match.").writeln(std::cout);
		}
	}

	return 0;
}
