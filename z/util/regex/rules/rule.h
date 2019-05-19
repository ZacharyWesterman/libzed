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
			private:

			public:
				rule(size_t min=1, size_t max=1, bool greedy=true): min(min), max(max), greedy(greedy){}
				virtual ~rule(){}

				virtual bool match(uint32_t) const {return false;}
				virtual bool match(core::inputStream&) const {return false;}
				virtual bool base() const {return true;}
				virtual bool parent() const {return false;}
				virtual bool isOr() const {return false;}

				size_t min;
				size_t max;
				bool greedy;

#			ifdef DEBUG
				virtual void print(core::outputStream& stream, int level=0)=0;

				zpath meta()
				{
					zpath s = ' ';
					if ((min != 1) || (max != 1))
					{
						s += '{';
						s += min;
						s += ',';
						s += (int)max;
						s += '}';
					}
					if (!greedy)
					{
						s += ' ';
						s += '?';
					}
					return s;
				}
#			endif
			};

		}
	}
}
