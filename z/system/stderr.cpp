#include "stderr.hpp"

#include <iostream>
#include <z/core/charFunctions.hpp>

#ifdef _WIN32
#include <io.h>
#define STDOUT_TTY _isatty(_fileno(::stderr))
#else
#include <unistd.h>
#define STDOUT_TTY isatty(fileno(::stderr))
#endif

namespace z
{
	namespace system
	{
		void stderr::put(uint8_t ch)
		{
			std::cerr << ch;
		}

		void stderr::put(uint8_t* str, size_t count, encoding format)
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
						std::cerr << c[j];
				}
				break;

			case utf32:
				for (size_t i=0; i<count; i++)
				{
					size_t len = core::toUTF8(c, ((uint32_t*)str)[i]);

					for (size_t j=0; j<len; j++)
						std::cerr << c[j];
				}
				break;

			default:
				for (size_t i=0; i<count; i++)
				{
					std::cerr << str[i];
				}
			}
		}

		bool stderr::good()
		{
			return true;
		}

		bool stderr::bad()
		{
			return false;
		}

		bool stderr::binary()
		{
			return false;
		}

		bool stderr::seekable()
		{
			return false;
		}

		void stderr::seek(size_t index)
		{
			(void)index;
		}

		size_t stderr::tell()
		{
			return 0;
		}

		size_t stderr::end()
		{
			return 0;
		}

		encoding stderr::format()
		{
			return utf8;
		}

		void stderr::setFormat(encoding enc, bool force)
		{
			(void)enc;
			(void)force;
		}

		void stderr::flush()
		{
			std::cerr << std::flush;
			std::cin.clear();
		}

		bool stderr::istty() const
		{
			return STDOUT_TTY;
		}
	}
}