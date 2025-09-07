#include <iostream>
#include <z/util/dictionary.hpp>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "This program finds valid dictionary words from a list of scrambled words." << std::endl;
		std::cerr << "Usage: " << argv[0] << " <scrambled_word_1> <scrambled_word_2> ..." << std::endl;
		return 1;
	}

	std::cerr << "Loading dictionary... ";
	z::util::dictionary dict;
	if (dict.read("/usr/share/dict/words")) {
		std::cerr << "ERROR: Failed to read dictionary file!" << std::endl;
		return 1;
	} else {
		std::cout << zstring::numberFormat(dict.length()) << " words.\n";
	}

	for (int i = 1; i < argc; i++) {
		// Find all valid permutations of the given scrambled word
		// (filter out the trivial word itself if it exists)
		const auto validWords = dict.findPermutations(argv[i]).filter([&](const z::core::string<> &word) { return word != argv[i]; });

		std::cout << "❬" << argv[i] << "❭ Found " << validWords.length() << " permutation" << (validWords.length() == 1 ? "" : "s") << "." << std::endl;

		for (int j = 0; j < validWords.length(); j++) {
			std::cout << (j == validWords.length() - 1 ? "└─ " : "├─ ") << validWords[j] << std::endl;
		}
	}

	return 0;
}
