#include "size.hpp"
#include "exceptions.hpp"

#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <fstream>
#endif

namespace z {
namespace file {
size_t size(const zpath &path) {
#if __cplusplus >= 201703L
	// If C++17, we can get file size faster than using fstream's seek() method.
	std::error_code err{};
	try {
		return std::filesystem::file_size((const char *)path.cstring());
	} catch (const std::filesystem::filesystem_error &e) {
		throw unreadable(path);
	}
#else
	// Otherwise default to fstream version.
	std::ifstream in((const char *)path.cstring(), std::ios::binary);
	if (in.bad()) {
		throw unreadable(path);
	}
	const auto begin = in.tellg();
	in.seekg(0, std::ios::end);
	const auto end = in.tellg();
	in.close();
	return end - begin;
#endif
}
} // namespace file
} // namespace z
