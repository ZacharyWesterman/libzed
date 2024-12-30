#include "copy.hpp"
#include "exceptions.hpp"
#include <fstream>

namespace z {
namespace file {
void copy(const zpath &fileInput, const zpath &fileOutput) {
	std::ifstream src((char *)fileInput.cstring(), std::ios::binary);
	if (!src) {
		throw unreadable(fileInput);
	}

	std::ofstream dest((char *)fileOutput.cstring(), std::ios::binary);
	if (!dest) {
		throw unwritable(fileOutput);
	}

	dest << src.rdbuf();
}
} // namespace file
} // namespace z
