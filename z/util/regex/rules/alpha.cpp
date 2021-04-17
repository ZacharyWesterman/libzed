#include "alpha.hpp"
#include "../../../core/charFunctions.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			alpha::alpha(bool negate, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), negate(negate) {}

			bool alpha::match(uint32_t current) const noexcept
			{
				return negate ^ core::isAlpha(current);
			}

#			ifdef DEBUG
			void alpha::print(core::outputStream& stream, int level) noexcept
			{
				(zstring(" ").repeat(level*2)+(negate?"\\A":"\\a")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
