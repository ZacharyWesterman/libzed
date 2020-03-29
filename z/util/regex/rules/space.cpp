#include "space.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			space::space(bool negate, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), negate(negate) {}

			bool space::match(uint32_t current) const
			{
				return negate ^ core::isWhiteSpace(current);
			}

#			ifdef DEBUG
			void space::print(core::outputStream& stream, int level)
			{
				(zpath().padLeftIn(" ",(level)<<1)+"\\s"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
