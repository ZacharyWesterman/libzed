#pragma once

#include <z/core/array.h>
#include "rules/compound.h"

#include "rgxss.h"
#include "rgxerr.h"

namespace z
{
	namespace util
	{
		rgxerr rgxlex(const core::array<rgxss>& input, rgx::compound** nodeOut, size_t& position, int inType=0, int flags=0);
	}
}
