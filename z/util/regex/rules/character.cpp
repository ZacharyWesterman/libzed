#include "character.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			character::character(uint32_t value, bool insensitive, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), value(value), insensitive(insensitive) {}

			bool character::match(uint32_t current) const noexcept
			{
				if (insensitive)
					return core::toUpper(current) == core::toUpper(value);
				else
					return current == value;
			}

#			ifdef DEBUG
			void character::print(core::outputStream& stream, int level) noexcept
			{
				zpath s;
				s.padLeftIn(" ",(level)<<1);
				if ((value<'!')||(value>'~'))
					s = s + "x" + (int)value;
				else
					s += value;
				if (insensitive) s += " i";
				(s+meta()).writeln(stream);
			}
#			endif
		}
	}
}
