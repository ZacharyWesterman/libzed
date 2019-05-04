#pragma once
#include <z/core/stream.h>

#ifdef DEBUG
#include <z/core/string.h>
#endif

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

#			ifdef DEBUG
				virtual void print(core::outputStream& stream, int level=0)=0;
#			endif
			};

		}
	}
}
