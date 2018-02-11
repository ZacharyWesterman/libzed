#pragma once
#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <z/core/stream.h>
#include <iostream>

namespace z
{
    namespace system
    {
        class console : public core::stream<char>
        {
        public:
            char get();
            void put(char);

            core::string<char> read(char);
            void write(const core::string<char>&);

            bool empty();
        };

        char console::get()
        {
            return std::cin.get();
        }

        void console::put(char c)
        {
            std::cout << c;
        }

        core::string<char> console::read(char delim)
        {
            char tmp[128];
            std::cin >> tmp;

            return core::string<char>(tmp);
        }

        void console::write(const core::string<char>& input)
        {
            std::cout << input.str();
        }

        bool console::empty() {return false;}
    }
}

#endif // CONSOLE_H_INCLUDED
