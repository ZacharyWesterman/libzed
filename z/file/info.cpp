#include "info.hpp"

namespace z
{
	namespace file
	{
		info::info(const zpath& path)
		{
			does_exist = _stat((char*)path.cstring(), &fileStat);
		}

		inline bool info::exists()
		{
			return does_exist;
		}

		time_t info::lastAccessed()
		{
			if (does_exist)
				return fileStat.st_atime;
			else
				return 0;
		}

		time_t info::lastModified()
		{
			if (does_exist)
				return fileStat.st_mtime;
			else
				return 0;
		}

		time_t info::lastStatusChange()
		{
			if (does_exist)
				return fileStat.st_ctime;
			else
				return 0;
		}

		size_t info::byteCount()
		{
			if (does_exist)
				return fileStat.st_size;
			else
				return 0;
		}

		unsigned int info::deviceNumber()
		{
			if (does_exist)
				return fileStat.st_dev;
			else
				return 0;
		}
	}
}
