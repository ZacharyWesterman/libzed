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
         * <BR/>
         * <B> This class is deprecated. Use file::stream instead.</B>
         *
         * \see writer
         */
        template<typename CHAR>
        class reader
        {
            char* file_name;

            CHAR* contents_buffer;
            long bufsiz;

            long current_index;

            bool is_done;

        public:
            reader();
            reader(const core::string<char>&);
            ~reader();


            void set(const core::string<char>& fileName);

            void clear();

            CHAR get();

            /**
             * \brief Load the file contents.
             *
             * Attempts to load as much of the file into memory as
             * possible. If the timer runs out before that, returns
             * \b 0 to indicate that it should be called again.
             * Continues loading file contents on each subsequent
             * call. If no parameter is given, then the entire file
             * will be loaded into memory before returning.
             *
             * \param time an optional parameter to pause loading
             * after a timeout.
             *
             * \return \b -1 if unable to open the file at any point,<BR>
             * \b 0 if open successful but not done loading, and <BR>
             * \b 1 if done loading the file.
             */
            int read(const core::timeout& time = core::timeout(-1));

            inline bool done();

            inline const CHAR* getContents() const;
        };

        /**
         * \brief Default constructor.
         */
        template <typename CHAR>
        reader<CHAR>::reader()
        {
            file_name = null;

            contents_buffer = null;
            current_index = 0;
            bufsiz = 0;
            is_done = true;
        }

        /**
         * \brief Constructor with a file name.
         */
        template <typename CHAR>
        reader<CHAR>::reader(const core::string<char>& fileName)
        {
            file_name = new char[fileName.length() + 1];
            for (int i=0; i<fileName.length()+1; i++)
                file_name[i] = fileName[i];

            contents_buffer = null;
            current_index = 0;
            bufsiz = 0;
            is_done = false;
        }

        /**
         * \brief Destructor.
         */
        template <typename CHAR>
        reader<CHAR>::~reader()
        {
            if (contents_buffer)
                delete[] contents_buffer;

            if (file_name)
                delete[] file_name;
        }


        /**
         * \brief Clear all buffer contents as well as the file name.
         */
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

            is_done = true;
        }


        /**
         * \brief Set the file name and clear the buffer contents.
         */
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

            is_done = false;
        }

        /**
         * \brief Get a single (narrow) character from the file stream.
         *
         * Does not store the character in the buffer and any
         * subsequent calls to read() will not put this character
         * in the buffer.
         *
         * \return The next character in the file.
         */
        template<>
        char reader<char>::get()
        {
            if (is_done)
                return null;

            std::ifstream file;
            file.open(file_name);

            if (!file.good() || file.eof())
            {
                is_done = true;
                return null;
            }


            file.seekg(0, std::ios_base::end);

            int filesiz = file.tellg();

            if (filesiz == current_index+2)
                is_done = true;
            else if (filesiz < current_index+2)
            {
                is_done = true;
                return null;
            }


            //continue where we last left off
            file.seekg(std::ios_base::beg + current_index);

            char character = file.get();

            file.close();

            current_index++;

            return character;
        }

        /**
         * \brief Get a single character from the file stream.
         *
         * Does not store the character in the buffer and any
         * subsequent calls to read() will not put this character
         * in the buffer.
         *
         * \return The next character in the file.
         */
        template<typename CHAR>
        CHAR reader<CHAR>::get()
        {
            if (is_done)
                return null;

            std::wifstream file;
            file.open(file_name);

            if (!file.good() || file.eof())
            {
                is_done = true;
                return null;
            }

            //continue where we last left off
            file.seekg(std::ios_base::beg + current_index);

            CHAR character = file.get();

            if (file.eof())
            {
                is_done = true;
            }

            file.close();

            current_index++;

            return character;
        }

        /**
         * \brief Default loading function for narrow character set.
         */
        template<>
        int reader<char>::read(const core::timeout& time)
        {
            if (is_done)
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
                is_done = true;

                if (current_index)
                    contents_buffer[current_index-1] = null;
            }
            else
                is_done = !time.timedOut();

            //we are done with this iteration
            file.close();

            return is_done;
        }


        /**
         * \brief Default loading function for wide character set.
         */
        template<typename CHAR>
        int reader<CHAR>::read(const core::timeout& time)
        {
            if (is_done)
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
                contents_buffer = new CHAR[bufsiz+1];
                contents_buffer[bufsiz] = null;

                file.seekg(0, std::ios_base::beg);
            }

            //continue where we last left off
            file.seekg(std::ios_base::beg + current_index);

            while (!time.timedOut() && (current_index < bufsiz) && !file.eof())
            {
                contents_buffer[current_index] = (CHAR)file.get();
                current_index++;
            }
            contents_buffer[current_index] = null;


            //we are done with this iteration
            file.close();


            if ((current_index >= bufsiz) || file.eof())
                is_done = true;
            else
                is_done = !time.timedOut();

            return is_done;
        }


        /**
         * \brief Get whether all contents of the file have been read.
         *
         * \return \b True if we have reached the end of the file.
         * \b False otherwise.
         */
        template <typename CHAR>
        inline bool reader<CHAR>::done()
        {
            return is_done;
        }

        /**
         * \brief Get a pointer to the contents buffer.
         */
        template <typename CHAR>
        inline const CHAR* reader<CHAR>::getContents() const
        {
            return contents_buffer;
        }
    }
}

#endif // READER_H_INCLUDED
