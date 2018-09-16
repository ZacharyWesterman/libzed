#pragma once

#include <cstdint>
#include <cstddef>

#include "rgxid.h"
#include "rgxss.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Get the appropriate regex symbol from an escape character.
		 *
		 * This is called after having read a backslash.
		 * So we get results like `'s' &rarr; RGX_SPACE`, `'b' &rarr; RGX_BREAK`, etc.
		 *
		 * \param ch The character that has been escaped.
		 *
		 * \return The regex symbol resulting from the escaped character.
		 */
		rgxss rgxsesc(uint32_t ch);
	}
}
