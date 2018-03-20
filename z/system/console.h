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

        bool console::empty() {return true;}

        void console::seek(Int position) {}

        Int console::tell()
        {
            return 0;
        }
    }
}

#endif // CONSOLE_H_INCLUDED
