#pragma once

#include "rgxll.h"
#include "rgxmatcher.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Match a regex pattern according to a matcher
		 */
		bool rgxmatch(rgxmatcher* matcher);
	}
}
