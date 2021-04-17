#include "punct.hpp"
#include "../../../core/charFunctions.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			punct::punct(bool negate, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), negate(negate) {}

			bool punct::match(uint32_t current) const noexcept
			{
				const char chars[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

				int i = 0;
				while (chars[i])
				{
					if (current == (uint32_t)chars[i]) return !negate;
					++i;
				}

				return negate;
			}

#			ifdef DEBUG
			void punct::print(core::outputStream& stream, int level) noexcept
			{
				(zstring(" ").repeat(level*2)+(negate?"\\P":"\\p")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
