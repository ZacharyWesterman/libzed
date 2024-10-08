#include "path.hpp"

#ifdef _WIN32
#include <Tchar.h>
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
	TCHAR fullPath[MAX_PATH];
	(void)GetFullPathName(_T((const char *)filePath.cstring()), MAX_PATH, fullPath, NULL);
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
