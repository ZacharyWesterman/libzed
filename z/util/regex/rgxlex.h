#pragma once

#include <z/core/array.h>
#include "rules/compound.h"

#include "rgxss.h"
#include "rgxerr.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Parse a list of regex symbols into a rule tree.
		 * \param input The regex scan symbol list.
		 * \param nodeOut Double pointer to where the root rule will be initialized. This will always be of type rgx::andlist.
		 * \param position The index of the next scan symbol we are looking at. Should be set to 0 when calling externally.
		 * \param inType The type of compound rule we are creating. Should be set to 0 when calling externally.
		 * \param flags Regex flags for the current level. These are defined in z/util/regex/rgxflag.h
		 * \return If an error is encountered, returns a nonzero value indicating the first error. Otherwise, returns RGX_NO_ERROR (0).
		 */
		rgxerr rgxlex(const core::array<rgxss>& input, rgx::compound** nodeOut, size_t& position, int inType=0, int flags=0);
	}
}
