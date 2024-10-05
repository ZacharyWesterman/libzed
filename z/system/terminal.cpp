#include "terminal.hpp"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#else
#warning "OS is unsupported! z::system::terminal will default to a terminal size of 80x24."
#endif

namespace z {
namespace system {

termsize terminal() noexcept {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return {
		csbi.srWindow.Right - csbi.srWindow.Left + 1,
		csbi.srWindow.Bottom - csbi.srWindow.Top + 1,
	};
#elif __linux__
	winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return {
		w.ws_col,
		w.ws_row,
	};
#else
	return {80, 24};
#endif
}
} // namespace system
} // namespace z
