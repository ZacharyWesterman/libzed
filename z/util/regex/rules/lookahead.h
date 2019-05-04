#pragma once
#include "compound.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class lookahead : public compound
			{
			public:
				lookahead(bool negate=false, size_t min=1, size_t max=1, bool greedy=true);
				bool match(core::inputStream& stream) const;

				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif

			};
		}
	}
}
