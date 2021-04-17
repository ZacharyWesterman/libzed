#include "space.hpp"
#include "../../../core/charFunctions.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			space::space(bool negate, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), negate(negate) {}

			bool space::match(uint32_t current) const noexcept
			{
				return negate ^ core::isWhiteSpace(current);
			}

#			ifdef DEBUG
			void space::print(core::outputStream& stream, int level) noexcept
			{
				(zstring().repeat(" ",level*2)+"\\s"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
