#include <iostream>
#include <z/util/dictionary.hpp>

int main(int argc, char **argv) {
	std::cout << "Loading..." << std::flush;

	// Load the dictionary, pausing every 1 second to do other processes
	z::util::dictionary dict;
	while (dict.read("/usr/share/dict/words", 1'000'000)) {
		std::cout << '.' << std::flush;
	}

	std::cout << " Loaded " << dict.length() << " words." << std::endl;

	std::cout << "Filtering out words that begin with A... " << std::flush;
	dict = dict.filter([](auto item) { return z::core::toLower(item[0]) != 'a'; });
	std::cout << "we now only have " << dict.length() << " words." << std::endl;

	for (int i = 1; i < argc; i++) {
		('"'_zs + argv[i] + "\" is " + (dict.isWord(argv[i]) ? "" : "NOT ") + "a word.").writeln(std::cout);
	}
}
