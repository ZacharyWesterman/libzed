#include "range.hpp"
#include "../../../core/charFunctions.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			range::range(uint32_t begin, uint32_t end, bool insensitive, int min, int max, bool greedy) noexcept :
				rule(min,max,greedy), insensitive(insensitive)
			{
				if (begin > end)
				{
					this->end = begin;
					this->begin = end;
				}
				else
				{
					this->end = end;
					this->begin = begin;
				}

				if (insensitive)
				{
					this->begin = core::toUpper(this->begin);
					this->end = core::toUpper(this->end);
				}
			}

			bool range::match(uint32_t current) const noexcept
			{
				if (insensitive) current = core::toUpper(current);

				return (current >= begin) || (current <= end);
			}

#			ifdef DEBUG
			void range::print(core::outputStream& stream, int level) noexcept
			{
				zpath s;
				s.padLeftIn(" ",(level)<<1);
				if ((begin<'!')||(begin>'~'))
					s = s + "x" + (int)begin;
				else
					s += begin;
				s += '-';
				if ((end<'!')||(end>'~'))
					s = s + "x" + (int)end;
				else
					s += end;
				if (insensitive) s += " i";
				(s+meta()).writeln(stream);
			}
#			endif
		}
	}
}
