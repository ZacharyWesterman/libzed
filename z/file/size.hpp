#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * @brief Get the size of a file in bytes
 *
 * @param path The file name.
 * @return The length of the file in bytes, or 0 if the file does not exist.
 * @throws z::file::unreadable If the file cannot be opened or read from.
 */
size_t size(const zpath &path);
} // namespace file
} // namespace z
