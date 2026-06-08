#include "chdir.hpp"

#include <stdio.h>
#if defined(_WIN32)
#include <direct.h>
#elif defined(__linux__)
#include <unistd.h>
#define _chdir ::chdir
#else
#warning file::chdir is incompatible with target OS! Please create a pull request or open an issue on GitHub.
#endif

namespace z {
namespace file {
bool chdir(const zpath &path) noexcept {
#if defined(_WIN32) || defined(__linux__)
	return !_chdir((const char *)path.cstring());
#else
	return false;
#endif
}
} // namespace file
} // namespace z
