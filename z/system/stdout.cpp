#include "stdout.hpp"

#include <iostream>
#include "../core/charFunctions.hpp"

#ifdef _WIN32
#define STDOUT_TTY false
#undef stdout
#else
#include <unistd.h>
#define STDOUT_TTY isatty(fileno(::stdout))
#endif

namespace z
{
	namespace system
	{
		void stdout::put(uint8_t ch)
		{
			std::cout << ch;
		}

		void stdout::put(uint8_t* str, int count, encoding format)
		{
			if (!str) return;
			uint8_t c[4];

			switch (format)
			{
			case utf16:
				for (int i=0; i<count; i++)
				{
					int len = core::toUTF8(c, ((uint16_t*)str)[i]);

					for (int j=0; j<len; j++)
						std::cout << c[j];
				}
				break;

			case utf32:
				for (int i=0; i<(count << 2); i++)
				{
					// int len = core::toUTF8(c, ((uint32_t*)str)[i]);
					// std::cout << ":= " << (int)((uint32_t*)str)[i] << std::endl;
					std::cout << (char)str[i] << ' ';
					if (i % 4 == 3) std::cout << std::endl;

					// for (int j=0; j<len; j++)
						// std::cout << c[j];
				}
				std::cout << std::endl;
				break;

			default:
				for (int i=0; i<count; i++)
				{
					std::cout << str[i];
				}
			}
		}

		bool stdout::good()
		{
			return true;
		}

		bool stdout::bad()
		{
			return false;
		}

		bool stdout::binary()
		{
			return false;
		}

		bool stdout::seekable()
		{
			return false;
		}

		void stdout::seek(size_t index)
		{
			(void)index;
		}

		size_t stdout::tell()
		{
			return 0;
		}

		size_t stdout::end()
		{
			return 0;
		}

		encoding stdout::format()
		{
			return utf8;
		}

		void stdout::setFormat(encoding enc, bool force)
		{
			(void)enc;
			(void)force;
		}

		void stdout::flush()
		{
			std::cout << std::flush;
			std::cin.clear();
		}

		bool stdout::istty() const
		{
			return STDOUT_TTY;
		}
	}
}
