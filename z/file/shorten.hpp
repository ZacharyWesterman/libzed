#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * @brief Shorten the given directory string.
 *
 * Removes any extra symbols from the given directory string.
 * Extra slashes are removed, as well as redundant symbols
 * like \b "/./". Unnecessary directory backtracking is
 * also removed.
 * <BR>(e.g. "C:/a1/b1/../b2/foo.bar" -> "C:/a1/b2/foo.bar")
 *
 * @param dir the given directory.
 *
 * @return The directory with redundant symbols removed.
 */
zpath shorten(const zpath &dir) noexcept;
} // namespace file
} // namespace z
