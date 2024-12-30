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
 * @throw unreadable if the input file could not be read.
 * @throw unwritable if the output file could not be written.
 */
void copy(const zpath &fileInput, const zpath &fileOutput);
} // namespace file
} // namespace z
