#include "alnum.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			alnum::alnum(bool negate, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), negate(negate) {}

			bool alnum::match(uint32_t current) const noexcept
			{
				return negate ^ core::isAlphaNumeric(current);
			}

#			ifdef DEBUG
			void alnum::print(core::outputStream& stream, int level) noexcept
			{
				(zpath().padLeftIn(" ",(level)<<1)+(negate?"\\L":"\\l")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
