#include "digit.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool digit::match(uint32_t current) const
			{
				return core::isNumeric(current);
			}

#			ifdef DEBUG
			void digit::print(core::outputStream& stream, int level)
			{
				(zpath().padLeft(" ",(level)<<1)+"\\d"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
