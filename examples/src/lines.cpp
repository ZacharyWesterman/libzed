// This is an example of using the z::file::lines() generator to read a file line-by-line.
// This is a memory-efficient way of reading large files, as the data is only loaded as needed.
// z::file::lines(zstring& filename) // z::file::lines(std::istream &stream)

#include <iostream>
#include <z/file.hpp>

using z::file::execdir;
using z::file::lines;

int main() {
	zstring filename = execdir() + "/../data/in1.txt";

	("Printing all lines in "_zs + filename).writeln(std::cout);
	for (auto line : lines(filename)) {
		line.writeln(std::cout);
	}
}