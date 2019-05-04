#pragma once
#include "compound.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class lookbehind : public compound
			{
			public:
				lookbehind(size_t width, bool negate=false, size_t min=1, size_t max=1, bool greedy=true);
				bool match(core::inputStream& stream) const;

				bool negate;
				size_t width;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
