#pragma once

#include <z/core/string.h>
#include <z/core/array.h>

#include "rgxss.h"
#include "rgxerr.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Scan a string, creating a list of regex symbols.
		 *
		 * \param pattern The string to scan.
		 * \param output An array of regex symbols, returned by reference.
		 *
		 * \return The first error encountered in the pattern. If no error, gives RGX_NO_ERROR (0).
		 *
		 * \threadsafe_function_ref
		 */
		rgxerr rgxscan(const zstring& pattern, core::array<rgxss>& output);
	}
}
