#include "word.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			word::word(bool negate, int min, int max, bool greedy):
				rule(min,max,greedy), negate(negate) {}

			bool word::match(uint32_t current) const
			{
				return negate ^ (('_' == current) || core::isAlphaNumeric(current));
			}

#			ifdef DEBUG
			void word::print(core::outputStream& stream, int level)
			{
				(zpath().padLeftIn(" ",(level)<<1)+(negate?"\\W":"\\w")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
