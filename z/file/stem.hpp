#include "../core/string.hpp"

namespace z::file {

/**
 * @brief Get the stem of a file name.
 *
 * For example, given a file `/path/to/file.ext`, this function will output `file`.
 *
 * @param path The file name.
 * @return The stem of the file name.
 */
zpath stem(const zpath &path);

} // namespace z::file
