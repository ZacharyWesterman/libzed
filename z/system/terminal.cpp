#include "terminal.hpp"
#include <iostream>

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

termsize terminal(const std::ostream &stream) noexcept {
	if (&stream != &std::cout && &stream != &std::cerr) {
		return {80, 24};
	}

#ifdef _WIN32
	auto FILE_HANDLE = (&stream == &std::cout) ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(FILE_HANDLE), &csbi);
	return {
		csbi.srWindow.Right - csbi.srWindow.Left + 1,
		csbi.srWindow.Bottom - csbi.srWindow.Top + 1,
	};
#elif __linux__
	auto FILE_HANDLE = (&stream == &std::cout) ? STDOUT_FILENO : STDERR_FILENO;
	winsize w;
	ioctl(FILE_HANDLE, TIOCGWINSZ, &w);
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
