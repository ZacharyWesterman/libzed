#include "sleep.hpp"

#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#warning system::sleep() is incompatible with this OS! Please create a pull request or open an issue on GitHub.
#endif

namespace z {
namespace system {
void sleep(double ms) noexcept {
#if defined(_WIN32)
	Sleep(ms);
#elif defined(__linux__)
	usleep(ms * 1000);
#endif
}
} // namespace system
} // namespace z
