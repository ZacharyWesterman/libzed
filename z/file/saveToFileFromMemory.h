/**
 * File:            saveToFileFromMemory.h
 * Namespace:       z::file
 * Description:     The saveStringToFile() functions attempt to save the
 *                  entire contents a string to the specified file,
 *                  returning false if unable to open the file. If
 *                  successful, this will overwrite any previous contents
 *                  of the file.
 *                  The saveBinaryFile() function has limited use and
 *                  I may remove or replace it later, so avoid using it.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/

#pragma once
#ifndef SAVETOFILEFROMMEMORY_H_INCLUDED
#define SAVETOFILEFROMMEMORY_H_INCLUDED

#include <z/core/string.h>
#include <stdio.h>

namespace z
{
    namespace file
    {
        bool saveStringToFile(const core::string<char>& fileName, const core::string<wchar_t>& fileData)
        {
            std::wofstream file;
            file.open(fileName.str());

            if (file.good())
            {
                for (int i=0; i<fileData.length(); i++)
                {
                    file.put(fileData[i]);
                }

                return true;
            }
            else
            {
                return false;
            }
        }

        bool saveStringToFile(const core::string<char>& fileName, const core::string<char>& fileData)
        {
            std::ofstream file;
            file.open(fileName.str());

            if (file.good())
            {
                for (int i=0; i<fileData.length(); i++)
                {
                    file.put(fileData[i]);
                }

                return true;
            }
            else
            {
                return false;
            }
        }


        ///NOT FINISHED; DO NOT USE!
        template <typename T>
        bool saveBinaryFile(const core::string<char>& fileName, const T& object)
        {
            FILE* file = NULL;

            file = fopen(fileName.str(), "wb");

            if (file)
            {
                fwrite(&object, sizeof(T), 1, file);

                fclose(file);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

#endif // SAVETOFILEFROMMEMORY_H_INCLUDED
