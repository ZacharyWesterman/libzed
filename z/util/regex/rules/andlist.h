#pragma once
#include "compound.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class andlist : public compound
			{
			public:
				bool match(core::inputStream& stream) const;
			};
		}
	}
}
