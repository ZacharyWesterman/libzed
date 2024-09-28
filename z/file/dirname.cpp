#include "dirname.hpp"

namespace z {
namespace file {
/**
 * @brief Get the directory part of a file name.
 * @return A string containing the directory above a given path.
 * @threadsafe_function_yes
 */
zpath dirname(const zpath &path) noexcept {
	zpath dir = path;
	dir.replace("\\", "/").trimRightIn("/");

	if (!dir.length()) {
		return "/";
	}

	int slash = dir.findLast("/");
	if (slash > -1) {
		dir.truncate(slash);
	}

	return dir.length() ? dir : "/";
}
} // namespace file
} // namespace z