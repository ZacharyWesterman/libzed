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
				bool match(core::inputStream& stream) const;
				bool base() const;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
