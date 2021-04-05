#include "executable.hpp"

#if defined(_WIN32)
#include <libloaderapi.h>
#elif defined(__linux__)
#include <unistd.h>
#define _getcwd getcwd
#else
#warning file::dir is incompatible with target OS!
#endif

namespace z
{
	namespace file
	{
		zpath executable() noexcept
		{
			char buf[FILENAME_MAX];
			int length = 0;
#			if defined(_WIN32)
			length = GetModuleFileNameA(nullptr, buf, FILENAME_MAX);
#			elif defined(__linux__)
			length = readlink("/proc/self/exe",buf,FILENAME_MAX);
#			endif
			buf[length] = '\0';
			return buf;
		}
	}
}
