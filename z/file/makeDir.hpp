#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * \brief Make a directory with the given path.
 *
 * This function is meant as a platform-independent way
 * to create a new directory, for both Windows and Linux.
 *
 * \param dir the desired path of the directory to make.
 *
 * \return \b True if the directory was created
 * successfully. \b False otherwise.
 */
bool makeDir(const zpath &dir) noexcept;
} // namespace file
} // namespace z
