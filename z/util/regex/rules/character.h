#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class character : public rule
			{
			public:
				character(uint32_t value, bool insensitive=false, size_t min=1, size_t max=1, bool greedy=true);

				bool match(uint32_t current) const;

				uint32_t value;
				bool insensitive;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
