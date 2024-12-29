#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * @brief Remove a file from the filesystem.
 *
 * @param filename the path of the file to remove.
 *
 * @return \b True if the file was removed successfully.
 * \b False otherwise.
 */
bool remove(const zpath &filename) noexcept;
} // namespace file
} // namespace z