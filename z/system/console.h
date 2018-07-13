#pragma once

#include <iostream>
#include <z/core/stream.h>

namespace z
{
    namespace system
    {
		/**
		 * \brief Class for interacting with the console.
		 *
		 * This class provides an implementation of core::stream.
		 * All read-related methods read from stdin, and all
		 * write-related methods write to stdout. Note that
		 * because of how stdin and stdout behave, you cannot
		 * unget characters or seek a position in the stream;
		 * those are just dummy methods.
		 * As a result, this class will not behave nicely with the
		 * util::regex class.
		 */
		 class console : public core::stream
 		{
 		public:
 			void put(uint8_t ch);
 			void put(uint8_t* str, size_t count, encoding format = ascii);

 			uint8_t get();
 			uint32_t getChar(encoding format = ascii);
 			size_t get(size_t count, uint8_t* buf, encoding format = ascii);

 			bool empty();
			bool good();
			bool bad();
			bool binary();

 			void seek(size_t index);
 			size_t tell();
 			size_t end();
 		};

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

		size_t console::get(size_t count, uint8_t* buf, encoding format)
		{
			return 0;
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
    }
}
