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
	std::error_code err{};
	try {
		return std::filesystem::file_size((const char *)path.cstring());
	} catch (const std::filesystem::filesystem_error &e) {
		throw unreadable(path);
	}
}
} // namespace file
} // namespace z
