#pragma once

#include <z/core/string.h>

namespace z
{
	namespace file
	{
		/**
		 * \brief Shorten the given directory string.
		 *
		 * Removes any extra symbols from the given directory string.
		 * Extra slashes are removed, as well as redundant symbols
		 * like \b "/./". Unnecessary directory backtracking is
		 * also removed.
		 * <BR>(e.g. "C:/a1/b1/../b2/foo.bar" -> "C:/a1/b2/foo.bar")
		 *
		 * \param dir the given directory.
		 *
		 * \return The directory with redundant symbols removed.
		 */
		core::string<utf8> shorten(const core::string<utf8>& dir);
	}
}
