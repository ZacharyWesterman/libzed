#include "path.hpp"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <limits.h>
#include <stdlib.h>
#else
#warning file::path is incompatible with target OS!
#endif

namespace z {
namespace file {
zpath path(const zpath &filePath) noexcept {
#ifdef _WIN32
	char fullPath[MAX_PATH];

	(void)GetFullPathName((const char *)filePath.cstring(), MAX_PATH, fullPath, NULL);
#elif __linux__
	char fullPath[PATH_MAX];
	char *ignored __attribute__((unused));
	ignored = realpath((const char *)filePath.cstring(), fullPath);
#else
	auto fullPath = filePath;
#endif

	return fullPath;
}
} // namespace file
} // namespace z
