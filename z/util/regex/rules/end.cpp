#include "end.h"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			end::end(bool newline, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), newline(newline) {}

			bool end::match(core::inputStream& stream) const
			{
				if (stream.empty()) return true;

				auto pos = stream.tell();
				auto ch = stream.getChar();
				if (stream.empty()) return true;

				if (newline)
				{
					if (ch == '\n')
					{
						pos = stream.tell();
						ch = stream.getChar();
						if (stream.empty()) return true;
						if (ch != '\r') stream.seek(pos);
						return true;
					}
					else if (ch == '\r')
					{
						pos = stream.tell();
						ch = stream.getChar();
						if (stream.empty()) return true;
						if (ch != '\n') stream.seek(pos);
						return true;
					}
				}

				stream.seek(pos);
				return false;
			}

			bool end::base() const
			{
				return false;
			}

#			ifdef DEBUG
			void end::print(core::outputStream& stream, int level)
			{
				(zpath().padLeft(" ",(level)<<1)+"$"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
