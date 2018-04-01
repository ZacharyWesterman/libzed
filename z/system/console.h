#pragma once
#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <iostream>
#include <z/core/stream.h>
#include <z/int.h>

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
        class console : public core::stream<char>
        {
        public:
            char get();
            core::string<char> get(Int);
            void unget();
            void put(char);

            core::string<char> read(char delim = 0);
            void write(const core::string<char>&);

            bool empty();

            void seek(Int);
            Int tell();
			Int end();
        };

        char console::get()
        {
            return std::cin.get();
        }

        core::string<char> console::get(Int count)
        {
            core::string<char> result;
            for (Int i=0; i<count; i++)
                result += std::cin.get();

            return result;
        }

		/**
		 * \brief Dummy method.
		 */
        void console::unget() {}

        void console::put(char c)
        {
            std::cout << c;
        }

        core::string<char> console::read(char delim)
        {
            char tmp[128];

            if (delim)
            {
                std::cin.getline(tmp, 128, delim);
            }
            else
                std::cin >> tmp;

            return core::string<char>(tmp);
        }

        void console::write(const core::string<char>& input)
        {
            std::cout << input.str();
        }

		/**
		 * \brief Dummy method.
		 *
		 * \return \b True.
		 */
        bool console::empty() {return true;}

		/**
		 * \brief Dummy method.
		 *
		 * \param position ignored.
		 */
        void console::seek(Int position) {}

		/**
		 * \brief Dummy method.
		 *
		 * \return \b 0.
		 */
        Int console::tell()
        {
            return 0;
        }

		/**
		 * \brief Dummy method.
		 *
		 * \return \b 0.
		 */
		Int console::end()
		{
			return 0;
		}
    }
}

#endif // CONSOLE_H_INCLUDED
