#include "dir.hpp"

#include <stdio.h>
#if defined(_WIN32)
#include <direct.h>
#elif defined(__linux__)
#include <unistd.h>
#define _getcwd getcwd
#else
#warning file::dir is incompatible with target OS! Please create a pull request or open an issue on GitHub.
#endif

namespace z {
namespace file {
zpath dir() noexcept {
#if defined(_WIN32) || defined(__linux__)
	char buf[FILENAME_MAX];
	char *ignored __attribute__((unused));
	ignored = _getcwd(buf, FILENAME_MAX);
	return buf;
#else
	return ".";
#endif
}
} // namespace file
} // namespace z
