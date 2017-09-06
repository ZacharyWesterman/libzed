#pragma once
#ifndef READFILETIMEOUT_H_INCLUDED
#define READFILETIMEOUT_H_INCLUDED

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
            core::string<char> file_name;

            CHAR* contents_buffer;
            long bufsiz;

            long current_index;

            bool done;

        public:
            reader()
            {
                contents_buffer = null;
                current_index = 0;
                bufsiz = 0;
                done = true;
            }

            ~reader()
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


            /////iterative load function prototype
            int read(const core::timeout& time = core::timeout(-1));


            ///return a pointer to the contents buffer
            const CHAR* getContents() const
            {
                return contents_buffer;
            }
        };



        /**
         * \brief Default loading function for narrow character set.
         */
        template<>
        int reader<char>::read(const core::timeout& time)
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


        /**
         * \brief Default loading function for wide character set.
         */
        template<>
        int reader<wchar_t>::read(const core::timeout& time)
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

#endif // READFILETIMEOUT_H_INCLUDED
