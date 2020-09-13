#include "digit.hpp"
#include "../../../core/charFunctions.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool digit::match(uint32_t current) const noexcept
			{
				return core::isNumeric(current);
			}

#			ifdef DEBUG
			void digit::print(core::outputStream& stream, int level) noexcept
			{
				(zpath().padLeftIn(" ",(level)<<1)+"\\d"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
