#include "exists.h"
#include <sys/stat.h>

namespace z
{
	namespace file
	{
		bool exists(const core::string<utf8>& pathname)
		{
			struct stat info;

			if (stat((char*)pathname.cstring(), &info) != 0)
				return true; //directory item exists
			else
				return false; //does not exist
		}
	}
}
