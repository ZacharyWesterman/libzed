#include "character.h"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			character::character(uint32_t value, bool insensitive, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), value(value), insensitive(insensitive) {}

			bool character::match(uint32_t current) const
			{
				return current == value;
			}

#			ifdef DEBUG
			void character::print(core::outputStream& stream, int level)
			{
				(zpath().padLeft(" ",(level)<<1)+(uint32_t)value).writeln(stream);
			}
#			endif
		}
	}
}
