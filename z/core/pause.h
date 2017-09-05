/**
 * File:            pause.h
 * Namespace:       z::core
 * Description:     The pause() function is meant to be a
 *                  platform-independent way of allowing the user
 *                  to put the program to sleep for a little while.
 *                  Ideally, this could be used to keep the program
 *                  from taking up too much CPU time when it's not
 *                  doing anything important.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   1 March. 2017
**/

#pragma once
#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED

#include <z/float.h>

#ifdef __linux__

#include <unistd.h>

namespace z
{
    namespace core
    {
        inline void pause(Float ms)
        {
            usleep(ms * 1000);
        }
    }
}

#elif _WIN32

#include <windows.h>

namespace z
{
    namespace core
    {
        inline void pause(Float ms)
        {
            Sleep(ms);
        }
    }
}

#else
#error "core/pause.h : Platform not supported!"
#endif

#endif // PAUSE_H_INCLUDED
