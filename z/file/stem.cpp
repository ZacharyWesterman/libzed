#include "stem.hpp"
#include "exceptions.hpp"

namespace z::file {

zpath stem(const zpath &path) {
	auto slash = path.findLast("/");
	auto dot = path.findLast(".");

	if (dot < 0) {
		dot = path.length();
	}

	if (slash < 0) {
		return path.substr(slash + 1, dot - slash);
	}
	return path.substr(0, dot);
}

} // namespace z::file
