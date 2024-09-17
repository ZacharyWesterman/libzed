#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * \brief Get the full path of the running executable.
 * \return A string containing the path of the currently running executable.
 * \threadsafe_function_yes
 */
zpath executable() noexcept;
} // namespace file
} // namespace z
