#include "join.hpp"

namespace z {
namespace core {

zstring join(const std::vector<std::string> &list, const zstring &delim) noexcept {
	zstring result;

	for (auto &item : list) {
		if (result.length()) {
			result += delim;
		}
		result += item;
	}

	return result;
}

} // namespace core
} // namespace z