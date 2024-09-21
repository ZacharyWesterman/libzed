#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {
/**
 * \brief Get the directory part of a file name.
 * \return A string containing the directory above a given path.
 * \threadsafe_function_yes
 */
template <encoding E>
core::string<E> dirname(const core::string<E> &path) noexcept {
	core::string<E> dir = path;
	dir.replace("\\", "/");
	dir.trimRightIn("/");

	int slash = dir.findLast("/");
	if (slash > -1) {
		dir.truncate(slash);
	}

	return dir;
}
} // namespace file
} // namespace z
