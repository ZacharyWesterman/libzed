#include "size.hpp"

#if __cplusplus == 201703L
#include <filesystem>
#else
#include <fstream>
#endif

namespace z
{
	namespace file
	{
		size_t size(const zpath& path) noexcept
		{
#			if __cplusplus == 201703L
			//If C++17, we can get file size faster than using fstream's seek() method.
			std::error_code err{};
			const auto len = std::filesystem::file_size((const char*)path.cstring());
			return (err == std::error_code{}) ? 0 : len;
#			else
			//Otherwise default to fstream version.
			std::ifstream in((const char*)path.cstring(), std::ios::binary);
			if (in.bad()) return 0;
			const auto begin = in.tellg();
			in.seekg(0,std::ios::end);
			const auto end = in.tellg();
			in.close();
			return end - begin;
#			endif
		}
	}
}
