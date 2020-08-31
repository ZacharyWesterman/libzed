#include "boundary.hpp"
#include <z/core/charFunctions.hpp>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			boundary::boundary(bool negate, int min, int max, bool greedy):
				rule(min,max,greedy), negate(negate) {}

			bool boundary::match(core::inputStream& stream) const
			{
				auto pos = stream.tell();
				bool bbeg, bend;

				if (pos)
				{
					switch (stream.format())
					{
						case utf32:
							stream.seek(pos-4);
							break;
						case utf16:
							stream.seek(pos-2);
							break;
						default:
							stream.seek(pos-1);
					}
					auto ch = stream.getChar();
					bbeg = !((ch == '_') || core::isAlphaNumeric(ch));
				}
				else
				{
					bbeg = true;
				}

				if (stream.empty())
				{
					bend = true;
				}
				else
				{
					auto ch = stream.getChar();
					if (stream.empty()) return stream.empty();
					bend = !((ch == '_') || core::isAlphaNumeric(ch));
				}

				stream.seek(pos);

				//word boundary at the beginning xor end, but invert if we're negating.
				return negate ^ (bbeg ^ bend);
			}

			bool boundary::base() const
			{
				return false;
			}

#			ifdef DEBUG
			void boundary::print(core::outputStream& stream, int level)
			{
				(zpath().padLeftIn(" ",(level)<<1)+"\\b"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
