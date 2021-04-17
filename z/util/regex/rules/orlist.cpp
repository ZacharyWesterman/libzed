#include "orlist.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			orlist::orlist(bool negate, int min, int max, bool greedy) noexcept :
				compound(min,max,greedy), negate(negate){}

			bool orlist::match(core::inputStream& stream) const noexcept
			{
				auto pos = stream.tell();
				for (auto& child : children)
				{
					if (matchRule(child, stream)) return !negate;
					stream.seek(pos);
				}
				return negate;
			}

#			ifdef DEBUG
			void orlist::print(core::outputStream& stream, int level) noexcept
			{
				auto s = zstring().repeat(" ",level*2)+"[]";
				if (negate) s += " ~";
				(s+meta()).writeln(stream);
				for (auto& child : children)
				{
					child->print(stream,level+1);
				}
			}
#			endif
		}
	}
}
