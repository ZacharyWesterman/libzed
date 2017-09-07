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
         * \see reader
         */
        template<typename CHAR>
        class writer
        {
        private:
            char* file_name;
            CHAR* data;

            int current_index;

            bool done;

            bool always_append;

        public:
            writer();

            writer(const core::string<char>&,
                   const core::string<CHAR>&,
                   bool append = false);

            ~writer();


            void set(const core::string<char>&,
                     const core::string<CHAR>&,
                     bool append = false);


            void clear();


            /**
             * \brief Write data to the file.
             *
             * Attempts to write as much data to the file as
             * possible. If the timer runs out before that, returns
             * \b 0 to indicate that it should be called again.
             * Continues writing data on each subsequent
             * call. If no parameter is given, then all given data
             * will be written to the file before returning.
             *
             * \param time an optional parameter to pause writing
             * after a timeout.
             *
             * \return \b -1 if unable to open the file at any point,<BR>
             * \b 0 if open successful but not done writing, and <BR>
             * \b 1 if done writing to the file.
             */
            int write(const core::timeout& time = core::timeout(-1));
        };


        /**
         * \brief Default constructor.
         */
        template <typename CHAR>
        writer<CHAR>::writer()
        {
            file_name = null;
            data = null;

            done = true;
        }

        /**
         * \brief Constructor with full data.
         *
         * \param fileName the name of the file.
         * \param fileData the data to write to the file.
         * \param append an optional parameter. If \b true,
         * the data is appended to the file.
         */
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

        /**
         * \brief Destructor.
         */
        template <typename CHAR>
        writer<CHAR>::~writer()
        {
            if (file_name)
                delete[] file_name;

            if (data)
                delete[] data;
        }

        /**
         * \brief Set the file name and the data to write.
         *
         * \param fileName the name of the file.
         * \param fileData the data to write to the file.
         * \param append an optional parameter. If \b true,
         * the data is appended to the file.
         */
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


        /**
         * \brief Clear all data as well as the file name.
         */
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


        /**
         * \brief Default writing function for narrow character set.
         */
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

        /**
         * \brief Default loading function for wide character set.
         */
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
