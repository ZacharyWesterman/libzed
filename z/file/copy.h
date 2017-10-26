#pragma once
#ifndef COPY_H_INCLUDED
#define COPY_H_INCLUDED

#include <z/core/string.h>
#include <fstream>

namespace z
{
    namespace file
    {
        // copy in binary mode
        bool copy(const core::string<char>& fileInput,
                  const core::string<char>& fileOutput)
        {
            std::ifstream src(fileInput.str(), std::ios::binary);
            std::ofstream dest(fileOutput.str(), std::ios::binary);
            dest << src.rdbuf();
            return src && dest;
        }
    }
}

#endif // COPY_H_INCLUDED
