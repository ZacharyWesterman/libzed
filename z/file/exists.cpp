#include "exists.h"
#include <sys/stat.h>

namespace z
{
	namespace file
	{
		bool exists(const zpath& pathname)
		{
			struct stat info;

			if (stat((char*)pathname.cstring(), &info) == 0)
				return true; //directory item exists
			else
				return false; //does not exist
		}
	}
}
