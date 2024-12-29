#include "remove.hpp"
#include <cstdio>

namespace z {
namespace file {
bool remove(const zpath &filename) noexcept {
	return std::remove(filename.cstring()) == 0;
}
} // namespace file
} // namespace z