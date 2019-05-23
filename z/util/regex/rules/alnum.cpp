#include "alnum.h"
#include <z/core/charFunctions.h>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			alnum::alnum(bool negate, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), negate(negate) {}

			bool alnum::match(uint32_t current) const
			{
				return negate ^ core::isAlphaNumeric(current);
			}

#			ifdef DEBUG
			void alnum::print(core::outputStream& stream, int level)
			{
				(zpath().padLeft(" ",(level)<<1)+(negate?"\\L":"\\l")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
