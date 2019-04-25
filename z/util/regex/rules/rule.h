#pragma once
#include <z/core/stream.h>

namespace z
{
	namespace util
	{
		namespace rgx
		{
			class rule
			{
			public:
				rule(size_t min=1, size_t max=1, bool greedy=true): min(min), max(max), greedy(greedy){}
				virtual ~rule(){}

				virtual bool match(uint32_t) const {return false;}
				virtual bool match(core::inputStream&) const {return false;}
				virtual bool base() const {return true;}

				size_t min;
				size_t max;
				bool greedy;
			};

		}
	}
}
