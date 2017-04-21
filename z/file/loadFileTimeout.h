/**
 * File:            loadFileTimeout.h
 * Namespace:       z::file
 * Description:     The loadFileTimeout() class loads as much of
 *                  a file as possible before the specified timeout.
 *                  If timeout is reached, closes the file. Then
 *                  attempts to reopen file and continue loading at
 *                  each subsequent call.
 *                  Returns -1 if unable to open the file at any point.
 *                  Returns  0 if open successful but not done loading.
 *                  Returns  1 if done loading the file.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/

#pragma once
#ifndef LOADFILETIMEOUT_H_INCLUDED
#define LOADFILETIMEOUT_H_INCLUDED

#include <z/core/string.h>
#include <z/core/timeout.h>

#include <fstream>
#include <stdio.h>

namespace z
{
    namespace file
    {
        class loader
        {
            core::string<char> file_name;

            core::string<char> file_data;

        public:



        };
    }
}

#endif // LOADFILETIMEOUT_H_INCLUDED
