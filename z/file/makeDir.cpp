#include "makeDir.hpp"

#if defined(__linux__)
#include <sys/stat.h>
#elif defined(_WIN32)
#include <direct.h>
#else
#warning file::makeDir is incompatible with the target OS! Please create a pull request or open an issue on GitHub.
#endif

namespace z {
namespace file {
bool makeDir(const zpath &dir) noexcept {
#if defined(__linux__)
	return (mkdir((char *)dir.cstring(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0);
#elif defined(_WIN32)
	return (_mkdir((char *)dir.cstring()) < 0);
#else
	return false;
#endif
}
} // namespace file
} // namespace z
