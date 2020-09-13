#pragma once

#include "../../core/array.hpp"
#include "rules/compound.hpp"

#include "rgxss.hpp"
#include "rgxerr.hpp"

#include <memory>

namespace z
{
	namespace util
	{
		/**
		 * \brief Parse a list of regex symbols into a rule tree.
		 * \param input The regex scan symbol list.
		 * \param parseError If an error is encountered, gives a nonzero value indicating the first error. Otherwise, gives RGX_NO_ERROR (0).
		 * \param position The index of the next scan symbol we are looking at. Should be set to 0 when calling externally.
		 * \param inType The type of compound rule we are creating. Should be set to 0 when calling externally.
		 * \param flags Regex flags for the current level. These are defined in z/util/regex/rgxflag.hpp
		 * \return A pointer to an initialized root rule. If not null, deallocation must be handled by the caller.
		 */
		rgx::compound* rgxlex(const core::array<rgxss>& input, rgxerr& parseError, int& position, int inType=0, int flags=0) noexcept;
	}
}
