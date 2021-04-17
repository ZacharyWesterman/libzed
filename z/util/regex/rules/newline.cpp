#include "newline.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool newline::match(core::inputStream& stream) const noexcept
			{
				if (stream.empty()) return false;
				auto pos = stream.tell();

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

			bool newline::base() const noexcept
			{
				return false;
			}

#			ifdef DEBUG
			void newline::print(core::outputStream& stream, int level) noexcept
			{
				(zstring(" ").repeat(level*2)+"\\n"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
