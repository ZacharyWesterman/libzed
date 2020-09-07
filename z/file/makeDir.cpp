#include "makeDir.hpp"

#ifdef __linux__
#include <sys/stat.h>
#elif _WIN32
#include <direct.h>
#else
#warning file::makeDir() is incompatible with the target OS.
#endif

namespace z
{
	namespace file
	{
		bool makeDir(const zpath& dir) noexcept
		{
#			ifdef __linux__
			return (mkdir((char*)dir.cstring(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0);
#			elif _WIN32
			return (_mkdir((char*)dir.cstring()) < 0);
#			else
			return false;
#			endif
		}
	}
}
