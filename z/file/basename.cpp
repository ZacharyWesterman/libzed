#include "basename.hpp"
#include <iostream>

namespace z {
namespace file {
zpath basename(const zpath &path) noexcept {
	zpath dir = path;
	dir.replace("\\", "/").trimRightIn("/");

	int slash = dir.findLast("/");
	if (slash > -1 && dir.length() > 1) {
		return dir.substr(slash + 1, dir.length());
	}

	return dir;
}
} // namespace file
} // namespace z