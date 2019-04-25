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
				character(uint32_t value, size_t min=1, size_t max=1, bool greedy=true);

				bool match(uint32_t current) const;
				bool base() const;

				uint32_t value;
			};
		}
	}
}
