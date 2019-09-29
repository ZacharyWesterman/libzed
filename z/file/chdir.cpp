#include "chdir.h"

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#elif __linux__
#include <unistd.h>
#define _chdir chdir
#else
#warning file::path is incompatible with target OS!
#endif

namespace z
{
	namespace file
	{
		bool chdir(const zpath& path)
		{
#			if defined(_WIN32) || defined(__linux__)
			return !_chdir((const char*)path.cstring());
#			else
			return false;
#			endif
		}
	}
}
