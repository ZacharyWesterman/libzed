#include "boundary.h"
#include <z/core/charFunctions.h>
namespace z
{
	namespace util
	{
		namespace rgx
		{
			boundary::boundary(bool negate, size_t min, size_t max, bool greedy):
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
					bbeg = (ch == '_') || core::isAlphaNumeric(ch);
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
					bend = (ch == '_') || core::isAlphaNumeric(ch);
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
				(zpath().padLeft(" ",(level)<<1)+"\\b").writeln(stream);
			}
#			endif
		}
	}
}
