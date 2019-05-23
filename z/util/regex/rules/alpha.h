#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class alpha : public rule
			{
			public:
				alpha(bool negate=false, size_t min=1, size_t max=1, bool greedy=true);
				bool match(uint32_t current) const;

				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
