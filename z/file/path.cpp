#include "path.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <limits.h>
#include <stdlib.h>
#else
#warning file::path is incompatible with target OS!
#endif

namespace z
{
	namespace file
	{
		core::string<utf8> path(const core::string<utf8>& filePath)
		{
#			ifdef _WIN32
			TCHAR fullPath[MAX_PATH];
			GetFullPathName(_T((const char*)filePath.cstring()), MAX_PATH, fullPath, NULL);
#			elif __linux__
			char fullPath[PATH_MAX];
			realpath((const char*)filePath.cstring(), fullPath);
#			else
			auto fullPath = filePath;
#			endif

			return fullPath;
		}
	}
}
