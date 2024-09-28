#include "write.hpp"

namespace z {
namespace file {

void write(const zstring &contents, const zpath &filename) {
	std::ofstream file(filename.cstring());

	if (!file) {
		throw unwritable(filename);
	}

	contents.write(file);
}

} // namespace file
} // namespace z