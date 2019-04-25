#include "character.h"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			character::character(uint32_t value, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), value(value){}

			bool character::match(uint32_t current) const
			{
				return current == value;
			}
		}
	}
}
