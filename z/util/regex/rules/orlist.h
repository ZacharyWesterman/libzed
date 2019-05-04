#pragma once
#include "compound.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class orlist : public compound
			{
			public:
				orlist(bool negate=false, size_t min=1, size_t max=1, bool greedy=true);
				bool match(core::inputStream& stream) const;

				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
