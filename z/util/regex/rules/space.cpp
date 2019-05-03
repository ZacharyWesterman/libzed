#include "space.h"
#include <z/core/charFunctions.h>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool space::match(uint32_t current) const
			{
				return core::isWhiteSpace(current);
			}
		}
	}
}
