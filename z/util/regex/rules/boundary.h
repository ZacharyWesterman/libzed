#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class boundary : public rule
			{
			public:
				bool match(core::inputStream& stream) const;
				bool base() const;
			};
		}
	}
}
