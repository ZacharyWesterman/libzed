#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class begin : public rule
			{
			public:
				begin(bool newline=false, size_t min=1, size_t max=1, bool greedy=true);

				bool match(core::inputStream& stream) const;
				bool base() const;

				bool newline;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
