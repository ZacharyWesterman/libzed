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
#ifndef WRITER_H_INCLUDED
#define WRITER_H_INCLUDED

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
        /**
         * \brief A class for iterative file writing.
         *
         * The file::reader class writes as much as
         * possible to a file before the specified timeout.
         * If timeout is reached, closes the file. Then
         * attempts to reopen file and continue writing at
         * each subsequent call.
         *
         * Note that this class is only compatible with types
         * \b char and \b wchar_t.
         *
         */
        template<typename CHAR>
        class writer
        {
        public:
            char* file_name;
            CHAR* data;

            int current_index;

            bool done;

            bool always_append;

        public:
            writer(bool append = false);

            writer(const core::string<char>&,
                   const core::string<CHAR>&,
                   bool append = false);

            ~writer();


            void set(const core::string<char>&,
                     const core::string<CHAR>&,
                     bool append = false);


            void clear();


            int write(const core::timeout& time = core::timeout(-1));
        };


        template <typename CHAR>
        writer<CHAR>::writer(bool append)
        {
            always_append = append;

            file_name = null;
            data = null;

            done = true;
        }

        template <typename CHAR>
        writer<CHAR>::writer(const core::string<char>& fileName,
                             const core::string<CHAR>& fileData,
                             bool append)
        {
            file_name = new char[fileName.length()+1];
            for(int i=0; i<fileName.length()+1; i++)
                file_name[i] = fileName[i];

            data = new CHAR[fileData.length()+1];
            for(int i=0; i<fileData.length()+1; i++)
                data[i] = fileData[i];

            always_append = append;

            current_index = 0;
            done = false;
        }


        template <typename CHAR>
        writer<CHAR>::~writer()
        {
            if (file_name)
                delete[] file_name;

            if (data)
                delete[] data;
        }

        ///set the file name and data to write
        template <typename CHAR>
        void writer<CHAR>::set(const core::string<char>& fileName,
                               const core::string<CHAR>& fileData,
                               bool append)
        {
            if (file_name)
                delete[] file_name;

            file_name = new char[fileName.length()+1];
            for(int i=0; i<fileName.length()+1; i++)
                file_name[i] = fileName[i];

            if (data)
                delete[] data;

            data = new CHAR[fileData.length()+1];
            for(int i=0; i<fileData.length()+1; i++)
                data[i] = fileData[i];

            always_append = append;

            current_index = 0;
            done = false;
        }


        ///clear all data and the file name
        template <typename CHAR>
        void writer<CHAR>::clear()
        {
            if (file_name)
                delete[] file_name;
            file_name = null;

            if (data)
                delete[] data;
            data = null;

            done = true;
        }


        ///iterative write() function for narrow characters
        template<>
        int writer<char>::write(const core::timeout& time)
        {
            if (done)
                return 1;

            std::ofstream file;
            if (current_index || always_append)
                file.open(file_name, std::ios::app);
            else
                file.open(file_name);

            if (!file.good())
                return -1;

            do //append at least 1 character each call.
            {
                file.put(data[current_index]);
                current_index++;
            } while (!time.timedOut() && data[current_index]);

            //we are done with this iteration
            file.close();


            done = !data[current_index];

            return done;
        }

        ///iterative write() function for wide characters
        template<typename CHAR>
        int writer<CHAR>::write(const core::timeout& time)
        {
            if (done)
                return 1;

            std::locale::global(std::locale(""));
            std::wofstream file;

            if (current_index || always_append)
                file.open(file_name, std::ios::app);
            else
                file.open(file_name);


            if (!file.good())
                return -1;

            do //append at least 1 character each call.
            {
                file.put(data[current_index]);
                current_index++;
            } while (!time.timedOut() && (wchar_t)data[current_index]);


            //we are done with this iteration
            file.close();


            done = !data[current_index];

            return done;
        }
    }
}

#endif // WRITER_H_INCLUDED
