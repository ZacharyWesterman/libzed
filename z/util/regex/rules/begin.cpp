#include "begin.h"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			begin::begin(bool newline, size_t min, size_t max, bool greedy):
				rule(min,max,greedy), newline(newline) {}

			bool begin::match(core::inputStream& stream) const
			{
				auto pos = stream.tell();
				if (!pos) return true;
				if (!newline) return false;

				auto ch = stream.getChar();
				if (stream.empty()) return false;
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

				stream.seek(pos);
				return false;
			}

			bool begin::base() const
			{
				return false;
			}

#			ifdef DEBUG
			void begin::print(core::outputStream& stream, int level)
			{
				(zpath().padLeft(" ",(level)<<1)+"^").writeln(stream);
			}
#			endif
		}
	}
}
