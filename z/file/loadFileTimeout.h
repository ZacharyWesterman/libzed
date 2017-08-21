/**
 * File:            loadFileTimeout.h
 * Namespace:       z::file
 * Description:     The file::loader class loads as much of
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
 * Last modified:   14 May 2017
**/

#pragma once
#ifndef LOADFILETIMEOUT_H_INCLUDED
#define LOADFILETIMEOUT_H_INCLUDED

#include <z/core/string.h>
#include <z/core/timeout.h>

#include <fstream>
//#include <stdio.h>

#ifndef null
    #define null 0
#endif //null

namespace z
{
    namespace file
    {
        template<typename CHAR>
        class loader
        {
            core::string<char> file_name;

            CHAR* contents_buffer;
            long bufsiz;

            long current_index;

            bool done;

        public:
            loader()
            {
                contents_buffer = null;
                current_index = 0;
                bufsiz = 0;
                done = true;
            }

            ~loader()
            {
                if (contents_buffer)
                    delete[] contents_buffer;
            }

            ///set the file name and clear the buffer contents
            void setFileName(const core::string<char>& fileName)
            {
                file_name = fileName;
                current_index = 0;

                if (contents_buffer)
                {
                    delete[] contents_buffer;
                    contents_buffer = null;
                    bufsiz = 0;
                }

                done = false;
            }


            ///clear all buffer contents as well as the file name
            void clear()
            {
                file_name.clear();
                current_index = 0;

                if (contents_buffer)
                {
                    delete[] contents_buffer;
                    contents_buffer = null;
                    bufsiz = 0;
                }

                done = true;
            }


            int load(const core::timeout&); //iterative load function prototype


            ///return a pointer to the contents buffer
            const CHAR* getContents() const
            {
                return contents_buffer;
            }
        };



        ///iterative load() function for narrow characters
        template<>
        int loader<char>::load(const core::timeout& time)
        {
            if (done)
                return 1;

            std::ifstream file;
            file.open(file_name.str());

            if (!file.good())
                return -1;


            if (!contents_buffer) //we haven't started loading the file yet
            {
                file.seekg(0, std::ios_base::end);

                //allocate memory for the file
                bufsiz = file.tellg();
                contents_buffer = new char[bufsiz+1];
                contents_buffer[bufsiz] = null;

                file.seekg(0, std::ios_base::beg);
            }

            //continue where we last left off
            file.seekg(std::ios_base::beg + current_index);


            do
            {
                contents_buffer[current_index] = file.get();
                current_index++;
            }
            while (!time.timedOut() && (current_index < bufsiz) && !file.eof());



            if ((current_index >= bufsiz) || file.eof())
            {
                done = true;

                if (current_index)
                    contents_buffer[current_index-1] = null;
            }
            else
                done = !time.timedOut();

            //we are done with this iteration
            file.close();

            return done;
        }


        ///iterative load function for wide characters
        template<>
        int loader<wchar_t>::load(const core::timeout& time)
        {
            if (done)
                return 1;

            std::wifstream file;
            file.open(file_name.str());

            if (!file)
                return -1;


            if (!contents_buffer) //we haven't started loading the file yet
            {
                file.seekg(0, std::ios_base::end);

                //allocate memory for the file
                bufsiz = file.tellg();
                contents_buffer = new wchar_t[bufsiz+1];
                contents_buffer[bufsiz] = null;

                file.seekg(0, std::ios_base::beg);
            }

            //continue where we last left off
            file.seekg(std::ios_base::beg + current_index);

            while (!time.timedOut() && (current_index < bufsiz) && !file.eof())
            {
                contents_buffer[current_index] = file.get();
                current_index++;
            }
            contents_buffer[current_index] = null;


            //we are done with this iteration
            file.close();


            if ((current_index >= bufsiz) || file.eof())
                done = true;
            else
                done = !time.timedOut();

            return done;
        }
    }
}

#endif // LOADFILETIMEOUT_H_INCLUDED
