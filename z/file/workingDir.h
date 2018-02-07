#pragma once
#ifndef WORKINGDIR_H_INCLUDED
#define WORKINGDIR_H_INCLUDED

#include <stdio.h>

#include <z/core/string.h>

#ifdef _WIN32
    #include <direct.h>
    #define getCurrDir _getcwd
#elif __linux__
    #include <unistd.h>
    #define getCurrDir getcwd
#else
    #error "file::workingDir is incompatible with the target OS."
#endif // _WIN32

namespace z
{
    namespace file
    {
        core::string<char> workingDir()
        {
            char tmp[FILENAME_MAX];

            getCurrDir(tmp, FILENAME_MAX);

            return core::string<char>(tmp);
        }
    }
}

#endif // WORKINGDIR_H_INCLUDED
