#include "../core/string.hpp"
#include "exceptions.hpp"
#include <fstream>

namespace z {
namespace file {

zstring read(const zpath &filename) {
	std::ifstream file(filename.cstring());

	if (!file) {
		throw unreadable(filename);
	}

	return zstring().readall(file).trimIn('\n');
}

} // namespace file
} // namespace z