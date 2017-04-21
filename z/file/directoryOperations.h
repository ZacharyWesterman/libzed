/**
 * File:            directoryOperations.h
 * Namespace:       z::file
 * Description:     Provides methods for directory
 *                  shortening, and separation of
 *                  file name and directory.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   29 Mar. 2017
**/

#pragma once
#ifndef DIRECTORYOPERATIONS_H_INCLUDED
#define DIRECTORYOPERATIONS_H_INCLUDED

#include <z/core/string.h>

namespace z
{
    namespace file
    { /*
        ///Function for directory shortening function,
        ///which removed extra symbols from a directory string.
        ///(e.g. "C:\a1\b1\..\b2\foo.bar" -> "C:\a1\b2\foo.bar")
        core::string<char> shorten(const core::string<char>& input)
        {
            core::string<char> output = input;

            int last_slash = -1;

            for (int i=0; i<output.length(); i++)
            {
                if ((output[i] == '\\') ||
                    (output[i] == '/'))
                {
                    if (last_slash == i-1)
                    {
                        output.remove(i,i);
                        i--;
                    }

                    last_slash = i;
                }
                else if (output.foundAt(i, ".."))
                {
                    output.
                }
            }

            return output;
        }*/
    }
}

#endif // DIRECTORYOPERATIONS_H_INCLUDED
