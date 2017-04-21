/**
 * File:            saveFileTimeout.h
 * Namespace:       z::file
 * Description:     The file::writer class writes as much of
 *                  a file as possible before the specified timeout.
 *                  If timeout is reached, closes the file. Then
 *                  attempts to reopen file and continue writing at
 *                  each subsequent call.
 *                  Returns -1 if unable to open the file at any point.
 *                  Returns  0 if open successful but not done writing.
 *                  Returns  1 if done writing the file.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Apr. 2017
**/

#pragma once
#ifndef SAVEFILETIMEOUT_H_INCLUDED
#define SAVEFILETIMEOUT_H_INCLUDED

#include <z/core/string.h>
#include <z/core/timeout.h>

#include <fstream>
#include <stdio.h>

#ifndef null
    #define null 0
#endif //null

namespace z
{
    namespace file
    {
        template<typename CHAR>
        class writer
        {
            core::string<char> file_name;

            core::string<CHAR> data;

            long current_index;

            bool done;

        public:
            writer()
            {
                current_index = 0;
                done = true;
            }

            ///set the file name
            void setFileName(const core::string<char>& fileName)
            {
                file_name = fileName;
                current_index = 0;

                done = false;
            }

            ///set the data to write
            void setFileData(const core::string<CHAR>& fileData)
            {
                data = fileData;
                current_index = 0;

                done = false;
            }


            ///clear all data and the file name
            void clear()
            {
                file_name.clear();
                data.clear();

                done = true;
            }


            int write(const core::timeout&); //iterative load function prototype
        };



        ///iterative write() function for narrow characters
        template<>
        int writer<char>::write(const core::timeout& time)
        {
            if (done)
                return 1;

            std::ofstream file;
            file.open(file_name.str());

            if (!file)
                return -1;


            //continue where we last left off
            file.seekp(std::ios_base::beg + current_index);

            while (!time.timedOut() && (current_index < data.length()))
            {
                file.put(data[current_index]);
                current_index++;
            }


            //we are done with this iteration
            file.close();


            done = (current_index >= data.length()) || (!time.timedOut());

            return done;
        }

        ///iterative write() function for wide characters
        template<>
        int writer<wchar_t>::write(const core::timeout& time)
        {
            if (done)
                return 1;

            std::locale::global(std::locale(""));
            std::wofstream file;

            if (current_index == 0)
                file.open(file_name.str(), std::ios::app);
            else
                file.open(file_name.str());


            if (!file.good())
                return -1;

            //continue where we last left off
            //file.seekp(std::ios_base::beg + current_index);

            while (!time.timedOut() && (current_index < data.length()))
            {
                file.put(data[current_index]);
                current_index++;
            }


            //we are done with this iteration
            file.close();


            done = (current_index >= data.length()) || (!time.timedOut());

            return done;
        }
    }
}

#endif // SAVEFILETIMEOUT_H_INCLUDED
