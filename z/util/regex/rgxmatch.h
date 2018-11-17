#pragma once

#include "rgxll.h"
#include "rgxmatcher.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Match a regex pattern according to a matcher
		 *
		 * \param matcher The matcher containing info about the regex and stream status.
		 *
		 * \return True if the regex pattern matched at the stream's index, false if it did not match.
		 *
		 * \threadsafe_function_ref
		 */
		bool rgxmatch(rgxmatcher* matcher);
	}
}
