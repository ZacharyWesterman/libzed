#include "copy.h"
#include <fstream>

namespace z
{
	namespace file
	{
		bool copy(const core::string<utf8>& fileInput, const core::string<utf8>& fileOutput)
		{
			std::ifstream src((char*)fileInput.cstring(), std::ios::binary);
			std::ofstream dest((char*)fileOutput.cstring(), std::ios::binary);
			dest << src.rdbuf();
			return src && dest;
		}
	}
}
