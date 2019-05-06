#include "character.h"
#include <z/core/charFunctions.h>
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
				if (insensitive)
					return core::toUpper(current) == core::toUpper(value);
				else
					return current == value;
			}

#			ifdef DEBUG
			void character::print(core::outputStream& stream, int level)
			{
				zpath s;
				s.padLeft(" ",(level)<<1);
				if ((value<'!')||(value>'~'))
					s = s + "x" + (int)value;
				else
					s += value;
				if (insensitive) s += " i";
				s.writeln(stream);
			}
#			endif
		}
	}
}
