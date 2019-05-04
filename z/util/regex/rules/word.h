#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class word : public rule
			{
			public:
				bool match(uint32_t current) const;
#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
