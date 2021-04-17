#include "lookbehind.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			lookbehind::lookbehind(bool negate, int min, int max, bool greedy) noexcept :
				compound(min,max,greedy), negate(negate), width(0){}

			bool lookbehind::match(core::inputStream& stream) const noexcept
			{
				size_t pos = stream.tell();

				size_t charw;
				switch (stream.format())
				{
					case utf32:
						charw = 4;
						break;
					case utf16:
						charw = 2;
						break;
					default:
						charw = 1;
				}

				charw *= width;
				if (pos < charw) return negate;
				stream.seek(charw);

				for (auto& child : children)
				{
					//Lookbehind is assumed to have constant width, so only match minimum.
					if (!matchMin(child, stream))
					{
						stream.seek(pos);
						return negate;
					}
				}

				stream.seek(pos);
				return !negate;
			}

#			ifdef DEBUG
			void lookbehind::print(core::outputStream& stream, int level) noexcept
			{
				zstring s;
				s.repeat(" ",level*2);
				s += negate ? "?<!" : "?<=";
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
