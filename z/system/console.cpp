#include "console.h"

#include <iostream>
#include <z/core/charFunctions.h>

namespace z
{
	namespace system
	{
		void console::put(uint8_t ch)
		{
			std::cout << ch;
		}

		void console::put(uint8_t* str, size_t count, encoding format)
		{
			if (!str) return;
			uint8_t c[4];

			switch (format)
			{
			case utf16:
				for (size_t i=0; i<count; i++)
				{
					size_t len = core::toUTF8(c, ((uint16_t*)str)[i]);

					for (size_t j=0; j<len; j++)
						std::cout << c[j];
				}
				break;

			case utf32:
				for (size_t i=0; i<count; i++)
				{
					size_t len = core::toUTF8(c, ((uint32_t*)str)[i]);

					for (size_t j=0; j<len; j++)
						std::cout << c[j];
				}
				break;

			default:
				for (size_t i=0; i<count; i++)
				{
					std::cout << str[i];
				}
			}
		}

		uint8_t console::get()
		{
			return std::cin.get();
		}

		uint32_t console::getChar(encoding format)
		{
			return std::cin.get();
		}

		bool console::empty()
		{
			return false;
		}

		bool console::good()
		{
			return true;
		}

		bool console::bad()
		{
			return false;
		}

		bool console::binary()
		{
			return false;
		}

		bool console::seekable()
		{
			return false;
		}

		void console::seek(size_t index)
		{

		}

		size_t console::tell()
		{
			return 0;
		}

		size_t console::end()
		{
			return 0;
		}

		encoding console::format()
		{
			return utf8;
		}

		void console::flush()
		{
			std::cout << std::flush;
		}
	}
}
