#pragma once
#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED

#include <z/float.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#else
#error "core/pause.h : Platform not supported!"
#endif

namespace z
{
    namespace core
    {
        /**
         * \brief Delays program execution temporarily.
         *
         * This function is meant to be a
         * platform-independent way of allowing the user
         * to put the program to sleep for a little while.
         * Ideally, this could be used to keep the program
         * from taking up too much CPU time when it's not
         * doing anything important.
         *
         * \param ms the number of \b milliseconds to wait.
         */
        inline void pause(Float ms)
        {
            #ifdef _WIN32
            Sleep(ms);
            #else
            usleep(ms * 1000);
            #endif
        }
    }
}


#endif // PAUSE_H_INCLUDED
