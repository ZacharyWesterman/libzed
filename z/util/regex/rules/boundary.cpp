#include "boundary.h"
#include <z/core/charFunctions.h>
namespace z
{
	namespace util
	{
		namespace rgx
		{
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

				return bbeg ^ bend; //word boundary at the beginning xor end.
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
