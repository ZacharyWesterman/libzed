#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * @brief Get the directory part of a file name.
 * @return A string containing the directory above a given path.
 * @threadsafe_function_yes
 */
zpath basename(const zpath &path) noexcept;
} // namespace file
} // namespace z
