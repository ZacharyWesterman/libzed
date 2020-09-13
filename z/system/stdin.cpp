#include "stdin.hpp"

#include <iostream>
#include "../core/charFunctions.hpp"

#ifdef _WIN32
#undef stdin
#endif

namespace z
{
	namespace system
	{
		uint8_t stdin::get()
		{
			if (std::cin.eof()) return 0;
			return std::cin.get();
		}

		uint8_t stdin::peek()
		{
			return std::cin.peek();
		}

		uint32_t stdin::getChar()
		{
			if (std::cin.eof()) return 0;

			uint8_t buf[4];
			buf[0] = std::cin.get();
			int len = core::lenFromUTF8(buf);
			for (int i=1; i<len; i++)
				buf[i] = std::cin.get();

			return core::fromUTF8(buf);
		}

		bool stdin::empty()
		{
			if (std::cin.eof())
			{
				return true;
			}
			return false;
		}

		bool stdin::good()
		{
			return true;
		}

		bool stdin::bad()
		{
			return false;
		}

		bool stdin::binary()
		{
			return false;
		}

		bool stdin::seekable()
		{
			return false;
		}

		void stdin::seek(size_t index)
		{
			(void)index;
		}

		size_t stdin::tell()
		{
			return 0;
		}

		size_t stdin::end()
		{
			return 0;
		}

		encoding stdin::format()
		{
			return utf8;
		}

		void stdin::setFormat(encoding enc, bool force)
		{
			(void)enc;
			(void)force;
		}
	}
}
