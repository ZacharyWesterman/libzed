#include "punct.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			punct::punct(bool negate, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), negate(negate) {}

			bool punct::match(uint32_t current) const
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
			void punct::print(core::outputStream& stream, int level)
			{
				(zpath().padLeftIn(" ",(level)<<1)+(negate?"\\P":"\\p")+meta()).writeln(stream);
			}
#			endif
		}
	}
}
