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
            core::string<char> get(int);
            void unget();
            void put(char);
            char unput();

            core::string<char> read(char delim = 0);
            void write(const core::string<char>&);

            bool empty();

            void seek(int);
            int tell() const;
        };

        char console::get()
        {
            return std::cin.get();
        }

        core::string<char> console::get(int count)
        {
            core::string<char> result;
            for (int i=0; i<count; i++)
                result += std::cin.get();

            return result;
        }

        void console::unget() {}

        void console::put(char c)
        {
            std::cout << c;
        }

        char console::unput()
        {
            return 0;
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

        void console::seek(int position) {}

        int console::tell() const
        {
            return 0;
        }
    }
}

#endif // CONSOLE_H_INCLUDED
