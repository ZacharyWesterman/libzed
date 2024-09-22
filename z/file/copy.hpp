#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * @brief Copy data from one file to another.
 *
 * @param fileInput the path of the file to copy from.
 * @param fileOutput the path of the file to copy to.
 *
 * @return \b True if the file was copied successfully.
 * @b False otherwise.
 */
bool copy(const zpath &fileInput, const zpath &fileOutput) noexcept;
} // namespace file
} // namespace z
