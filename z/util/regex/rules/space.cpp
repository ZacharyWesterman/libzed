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

#			ifdef DEBUG
			void space::print(core::outputStream& stream, int level)
			{
				(zpath().padLeft(" ",(level)<<1)+"\\s").writeln(stream);
			}
#			endif
		}
	}
}
