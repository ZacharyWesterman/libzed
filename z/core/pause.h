#pragma once
#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED

#ifdef __linux__

#include <unistd.h>

namespace core
{
    void pause(double ms)
    {
        usleep(ms * 1000);
    }
}

#elif _WIN32

#include <windows.h>

namespace core
{
    void pause(double ms)
    {
        Sleep(ms);
    }
}

#else
#error "core/pause.h : Platform not supported!"
#endif

#endif // PAUSE_H_INCLUDED
