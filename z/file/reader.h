#pragma once
#ifndef READER_H_INCLUDED
#define READER_H_INCLUDED

#include <z/core/string.h>
#include <z/core/timeout.h>

#include <fstream>

#ifndef null
    #define null 0
#endif //null

namespace z
{
    namespace file
    {
        /**
         * \brief A class for iterative file loading.
         *
         * The file::loader class loads as much of
         * a file as possible before the specified timeout.
         * If timeout is reached, closes the file. Then
         * attempts to reopen file and continue loading at
         * each subsequent call.
         *
         *
 *                  Returns -1 if unable to open the file at any point.
 *                  Returns  0 if open successful but not done loading.
 *                  Returns  1 if done loading the file.
         */
        template<typename CHAR>
        class reader
        {
            char* file_name;

            CHAR* contents_buffer;
            long bufsiz;

            long current_index;

            bool done;

        public:
            reader();
            reader(const core::string<char>&);
            ~reader();


            void set(const core::string<char>& fileName);

            void clear();

            int read(const core::timeout& time = core::timeout(-1));

            inline const CHAR* getContents() const;
        };


        template <typename CHAR>
        reader<CHAR>::reader(const core::string<char>& fileName)
        {
            file_name = new char[fileName.length() + 1];
            for (int i=0; i<fileName.length()+1; i++)
                file_name[i] = fileName[i];

            contents_buffer = null;
            current_index = 0;
            bufsiz = 0;
            done = false;
        }

        template <typename CHAR>
        reader<CHAR>::reader()
        {
            file_name = null;

            contents_buffer = null;
            current_index = 0;
            bufsiz = 0;
            done = true;
        }

        template <typename CHAR>
        reader<CHAR>::~reader()
        {
            if (contents_buffer)
                delete[] contents_buffer;

            if (file_name)
                delete[] file_name;
        }


        ///clear all buffer contents as well as the file name
        template <typename CHAR>
        void reader<CHAR>::clear()
        {
            delete[] file_name;
            file_name = null;

            current_index = 0;

            if (contents_buffer)
            {
                delete[] contents_buffer;
                contents_buffer = null;
                bufsiz = 0;
            }

            done = true;
        }


        ///set the file name and clear the buffer contents
        template <typename CHAR>
        void reader<CHAR>::set(const core::string<char>& fileName)
        {
            delete[] file_name;

            file_name = new char[fileName.length() + 1];
            for (int i=0; i<fileName.length()+1; i++)
                file_name[i] = fileName[i];

            current_index = 0;

            if (contents_buffer)
            {
                delete[] contents_buffer;
                contents_buffer = null;
                bufsiz = 0;
            }

            done = false;
        }


        /**
         * \brief Default loading function for narrow character set.
         */
        template<>
        int reader<char>::read(const core::timeout& time)
        {
            if (done)
                return 1;

            std::ifstream file;
            file.open(file_name);

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


        /**
         * \brief Default loading function for wide character set.
         */
        template<>
        int reader<wchar_t>::read(const core::timeout& time)
        {
            if (done)
                return 1;

            std::wifstream file;
            file.open(file_name);

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



        ///return a pointer to the contents buffer
        template <typename CHAR>
        inline const CHAR* reader<CHAR>::getContents() const
        {
            return contents_buffer;
        }
    }
}

#endif // READER_H_INCLUDED
