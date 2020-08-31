#include "orlist.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			orlist::orlist(bool negate, int min, int max, bool greedy):
				compound(min,max,greedy), negate(negate){}

			bool orlist::match(core::inputStream& stream) const
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
			void orlist::print(core::outputStream& stream, int level)
			{
				auto s = zpath().padLeftIn(" ",(level)<<1)+"[]";
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
