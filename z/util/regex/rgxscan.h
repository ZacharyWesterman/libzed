#pragma once

#include <z/core/string.h>
#include "rgxss.h"
#include "rgxerr.h"

namespace z
{
	namespace util
	{
		rgxerr rgxscan(const core::string<utf8>& pattern, core::array<rgxss>& output);
	}
}
