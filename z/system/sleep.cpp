#include "sleep.hpp"

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#else
#error "system::sleep() is incompatible with this OS! Please create a pull request or open an issue on GitHub."
#endif

namespace z {
namespace system {
void sleep(double ms) noexcept {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}
} // namespace system
} // namespace z
