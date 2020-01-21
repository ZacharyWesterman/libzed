#include "console.hpp"

#include <iostream>
#include <z/core/charFunctions.hpp>

#ifdef _WIN32
#include <io.h>
#define STDOUT_TTY _isatty(_fileno(stdout))
#else
#include <unistd.h>
#define STDOUT_TTY isatty(fileno(stdout))
#endif

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
			if (std::cin.eof()) return 0;
			return std::cin.get();
		}

		uint32_t console::getChar()
		{
			if (std::cin.eof()) return 0;

			uint8_t buf[4];
			buf[0] = std::cin.get();
			int len = core::lenFromUTF8(buf);
			for (int i=1; i<len; i++)
				buf[i] = std::cin.get();

			return core::fromUTF8(buf);
		}

		bool console::empty()
		{
			if (std::cin.eof())
			{
				return true;
			}
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
			(void)index;
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

		void console::setFormat(encoding enc, bool force)
		{
			(void)enc;
			(void)force;
		}

		void console::flush()
		{
			std::cout << std::flush;
			std::cin.clear();
		}

		bool console::istty() const
		{
			return STDOUT_TTY;
		}
	}
}
