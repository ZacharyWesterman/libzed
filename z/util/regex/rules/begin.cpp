#include "begin.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool begin::match(core::inputStream& stream) const noexcept
			{
				auto pos = stream.tell();
				if (!pos) return true;

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

			bool begin::base() const noexcept
			{
				return false;
			}

#			ifdef DEBUG
			void begin::print(core::outputStream& stream, int level) noexcept
			{
				(zpath().padLeftIn(" ",(level)<<1)+"^"+meta()).writeln(stream);
			}
#			endif
		}
	}
}
