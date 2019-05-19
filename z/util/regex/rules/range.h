#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class range : public rule
			{
			public:
				range(uint32_t begin, uint32_t end, bool insensitive=false, size_t min=1, size_t max=1, bool greedy=true);

				bool match(uint32_t current) const;

				uint32_t begin;
				uint32_t end;

				bool insensitive;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
