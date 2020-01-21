#include "copy.hpp"
#include <fstream>

namespace z
{
	namespace file
	{
		bool copy(const zpath& fileInput, const zpath& fileOutput)
		{
			std::ifstream src((char*)fileInput.cstring(), std::ios::binary);
			std::ofstream dest((char*)fileOutput.cstring(), std::ios::binary);
			dest << src.rdbuf();
			return src && dest;
		}
	}
}
