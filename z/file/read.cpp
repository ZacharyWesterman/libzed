#include "read.hpp"
#include "exceptions.hpp"

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