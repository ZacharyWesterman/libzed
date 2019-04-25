#pragma once
#include "rule.h"
#include <z/core/array.h>

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class andlist : public rule
			{
			public:
				~andlist();

				bool match(core::inputStream& stream) const;
				bool base() const;


				core::array<rule*> children;
			};
		}
	}
}
