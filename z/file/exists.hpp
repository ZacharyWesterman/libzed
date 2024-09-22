#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * @brief Check whether a file or directory with
 * the given name exists.
 *
 * @param pathname the relative or absolute path
 * of the file or directory.
 *
 * @return \b True if the file or directory exists.
 * @b False otherwise.
 */
bool exists(const zpath &pathname) noexcept;
} // namespace file
} // namespace z
