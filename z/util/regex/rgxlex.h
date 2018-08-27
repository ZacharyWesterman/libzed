#pragma once

#include "rgxerr.h"
#include "rgxss.h"
#include "rgxll.h"

namespace z
{
	namespace util
	{
		//root is dynamically allocated
		rgxerr rgxlex(const core::array<rgxss>& input, rgxll*& root);
	}
}
