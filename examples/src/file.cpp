// Read all lines from a file, applying an ad-hoc mutation to them

#include <iostream>
#include <z/all.hpp>

using z::file::lines, z::file::execdir;

int main() {
	const auto filename = execdir() + "/../data/in1.txt";

	"Original file contents:"_zs.writeln(std::cout);
	for (auto line : lines(filename)) {
		line.writeln(std::cout);
	}

	// Create a lambda that mutates each line of the file while they're read.
	auto mutated_lines = lines(filename).map<zstring>([](auto line) {
		auto result = line;
		result.replace(' ', "˽");
		result.replace('\n', "˥\n");
		result.replace('\r', "˿");
		result.replace('\t', "˾");
		return result;
	});

	"Mutated file contents:"_zs.writeln(std::cout);
	for (auto line : mutated_lines) {
		line.writeln(std::cout);
	}

	return 0;
}
