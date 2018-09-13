#pragma once

#include "rgxerr.h"
#include "rgxss.h"
#include "rgxll.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Regex pattern lexical analyzer
		 *
		 * \param input A list of scan symbols to parse into a node tree.
		 * \param root The resultant root node, returned by reference, or NULL if invalid regex syntax.
		 * Note that root is dynamically allocated, so it must be deleted after calling if not NULL.
		 *
		 * \return A flag representing the first error encountered. If no errors, returns RGX_NO_ERROR, which is equivalent to 0.
		 */
		rgxerr rgxlex(const core::array<rgxss>& input, rgxll*& root);
	}
}
