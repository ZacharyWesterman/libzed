#include "write.hpp"

namespace z {
namespace file {

void write(const zstring &contents, const zpath &filename, bool append) {
	std::ofstream file(filename.cstring(), append ? std::ios_base::app : std::ios_base::out);

	if (!file) {
		throw unwritable(filename);
	}

	contents.write(file);
}

} // namespace file
} // namespace z