#include "stem.hpp"
#include "exceptions.hpp"

namespace z::file {

zpath stem(const zpath &path) {
	auto slash = path.findLast("/");
	auto dot = path.findLast(".");

	if (dot <= slash) {
		dot = path.length();
	}

	if (slash < 0) {
		return path.substr(0, dot);
	}
	return path.substr(slash + 1, dot - slash - 1);
}

} // namespace z::file
