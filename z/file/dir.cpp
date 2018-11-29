#include "dir.h"

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#elif __linux__
#include <unistd.h>
#define _getcwd getcwd
#else
#warning file::path is incompatible with target OS!
#endif

namespace z
{
	namespace file
	{
		zpath dir()
		{
#			if defined(_WIN32) || defined(__linux__)
			char buf[FILENAME_MAX];
			_getcwd(buf, FILENAME_MAX);
			return buf;
#			else
			return ".";
#			endif
		}
	}
}
